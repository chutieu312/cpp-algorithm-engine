#include "api/Router.h"
#include "algorithms/KalmanFilter.h"
#include "algorithms/LeastSquares.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>

using json = nlohmann::json;

Router::Router(Repository& repo) : repo_(repo) {}

void Router::setCorsHeaders(httplib::Response& res)
{
    res.set_header("Access-Control-Allow-Origin",  "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
}

void Router::jsonResponse(httplib::Response& res, int status, const std::string& body)
{
    res.status = status;
    res.set_content(body, "application/json");
    setCorsHeaders(res);
}

void Router::registerRoutes(httplib::Server& svr)
{
    // CORS preflight for all paths
    svr.Options(".*", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin",  "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
        res.status = 204;
    });

    svr.Get("/health", [this](const httplib::Request& req, httplib::Response& res) {
        handleHealth(req, res);
    });

    svr.Get("/api/algorithms", [this](const httplib::Request& req, httplib::Response& res) {
        handleAlgorithms(req, res);
    });

    svr.Post("/api/kalman/run", [this](const httplib::Request& req, httplib::Response& res) {
        handleKalmanRun(req, res);
    });

    svr.Post("/api/leastsq/run", [this](const httplib::Request& req, httplib::Response& res) {
        handleLsqRun(req, res);
    });

    svr.Get("/api/runs", [this](const httplib::Request& req, httplib::Response& res) {
        handleGetRuns(req, res);
    });

    svr.Get("/openapi.yaml", [this](const httplib::Request& req, httplib::Response& res) {
        handleOpenApi(req, res);
    });
}

void Router::handleHealth(const httplib::Request&, httplib::Response& res)
{
    json resp = {{"status", "ok"}, {"service", "algorithm-validation-engine"}};
    jsonResponse(res, 200, resp.dump());
}

void Router::handleAlgorithms(const httplib::Request&, httplib::Response& res)
{
    json resp = {
        {"algorithms", json::array({
            {{"name", "kalman"},  {"endpoint", "/api/kalman/run"},
             {"description", "Discrete 1D Kalman Filter (constant-position model)"}},
            {{"name", "leastsq"}, {"endpoint", "/api/leastsq/run"},
             {"description", "Ordinary Least Squares linear regression"}}
        })}
    };
    jsonResponse(res, 200, resp.dump());
}

void Router::handleKalmanRun(const httplib::Request& req, httplib::Response& res)
{
    try {
        auto body         = json::parse(req.body);
        double x0         = body.at("x0").get<double>();
        double p0         = body.at("p0").get<double>();
        double q          = body.at("q").get<double>();
        double r          = body.at("r").get<double>();
        auto measurements = body.at("measurements").get<std::vector<double>>();

        if (measurements.empty() || measurements.size() > 1000) {
            jsonResponse(res, 400, R"({"error":"measurements must contain 1-1000 values"})");
            return;
        }

        KalmanFilter kf(x0, p0, q, r);
        json states = json::array();

        for (double z : measurements) {
            kf.predict();
            kf.update(z);
            states.push_back({
                {"state",       kf.getState()},
                {"covariance",  kf.getCovariance()},
                {"kalman_gain", kf.getKalmanGain()}
            });
        }

        json output = {{"states", states}, {"final_state", kf.getState()}};
        repo_.save("kalman", body.dump(), output.dump());
        jsonResponse(res, 200, output.dump());

    } catch (const std::invalid_argument& e) {
        jsonResponse(res, 400, json({{"error", e.what()}}).dump());
    } catch (const json::exception& e) {
        jsonResponse(res, 400, json({{"error", e.what()}}).dump());
    } catch (const std::exception& e) {
        jsonResponse(res, 500, json({{"error", e.what()}}).dump());
    }
}

void Router::handleLsqRun(const httplib::Request& req, httplib::Response& res)
{
    try {
        auto body   = json::parse(req.body);
        auto points = body.at("points").get<std::vector<json>>();

        if (points.size() < 2 || points.size() > 10000) {
            jsonResponse(res, 400, R"({"error":"points must contain 2-10000 values"})");
            return;
        }

        std::vector<double> xs, ys;
        xs.reserve(points.size());
        ys.reserve(points.size());
        for (const auto& p : points) {
            xs.push_back(p.at("x").get<double>());
            ys.push_back(p.at("y").get<double>());
        }

        LeastSquares ols;
        auto result = ols.fit(xs, ys);

        json predictions = json::array();
        for (std::size_t i = 0; i < xs.size(); ++i) {
            predictions.push_back({{"x", xs[i]}, {"y_predicted", ols.predict(xs[i])}});
        }

        json output = {
            {"slope",       result.slope},
            {"intercept",   result.intercept},
            {"r_squared",   result.r_squared},
            {"predictions", predictions}
        };
        repo_.save("leastsq", body.dump(), output.dump());
        jsonResponse(res, 200, output.dump());

    } catch (const std::invalid_argument& e) {
        jsonResponse(res, 400, json({{"error", e.what()}}).dump());
    } catch (const std::runtime_error& e) {
        jsonResponse(res, 422, json({{"error", e.what()}}).dump());
    } catch (const json::exception& e) {
        jsonResponse(res, 400, json({{"error", e.what()}}).dump());
    } catch (const std::exception& e) {
        jsonResponse(res, 500, json({{"error", e.what()}}).dump());
    }
}

void Router::handleGetRuns(const httplib::Request&, httplib::Response& res)
{
    try {
        auto runs    = repo_.findAll();
        json arr     = json::array();
        for (const auto& run : runs) {
            arr.push_back({
                {"id",          run.id},
                {"algorithm",   run.algorithm},
                {"input_json",  json::parse(run.input_json)},
                {"output_json", json::parse(run.output_json)},
                {"created_at",  run.created_at}
            });
        }
        jsonResponse(res, 200, arr.dump());
    } catch (const std::exception& e) {
        jsonResponse(res, 500, json({{"error", e.what()}}).dump());
    }
}

void Router::handleOpenApi(const httplib::Request&, httplib::Response& res)
{
    std::ifstream file("openapi.yaml");
    if (!file.is_open()) {
        res.status = 404;
        res.set_content("openapi.yaml not found", "text/plain");
        return;
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    res.set_content(ss.str(), "application/yaml");
    setCorsHeaders(res);
}
