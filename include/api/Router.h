/**
 * @file Router.h
 * @brief HTTP route registrations for the Algorithm Validation Engine.
 *
 * Registers all REST endpoints on the provided httplib::Server instance.
 * All routes return JSON responses with CORS headers.
 */
#pragma once

#include <httplib.h>
#include "db/Repository.h"

class Router {
public:
    explicit Router(Repository& repo);

    /// Register all routes on the given server instance.
    void registerRoutes(httplib::Server& svr);

private:
    Repository& repo_;

    void handleHealth     (const httplib::Request&,        httplib::Response& res);
    void handleAlgorithms (const httplib::Request&,        httplib::Response& res);
    void handleKalmanRun  (const httplib::Request& req,    httplib::Response& res);
    void handleLsqRun     (const httplib::Request& req,    httplib::Response& res);
    void handleGetRuns    (const httplib::Request&,        httplib::Response& res);
    void handleOpenApi    (const httplib::Request&,        httplib::Response& res);

    static void setCorsHeaders(httplib::Response& res);
    static void jsonResponse(httplib::Response& res, int status, const std::string& body);
};
