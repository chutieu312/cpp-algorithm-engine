#include <httplib.h>
#include "api/Router.h"
#include "db/Repository.h"
#include <cstdlib>
#include <iostream>
#include <string>

int main()
{
    const char* db_env     = std::getenv("DB_PATH");
    const char* port_env   = std::getenv("PORT");
    const char* public_env = std::getenv("PUBLIC_DIR");

    const std::string db_path  = db_env     ? db_env     : "ave.db";
    const int         port     = port_env   ? std::stoi(port_env) : 8080;
    const std::string pub_dir  = public_env ? public_env : "./public";

    try {
        Repository repo(db_path);
        Router     router(repo);

        httplib::Server svr;

        // Serve static frontend from public/
        if (!svr.set_mount_point("/", pub_dir.c_str())) {
            std::cerr << "[WARN] Could not mount public dir: " << pub_dir << "\n";
        }

        router.registerRoutes(svr);

        std::cout << "[AVE] Algorithm Validation Engine starting\n";
        std::cout << "[AVE] API:         http://localhost:" << port << "/api/algorithms\n";
        std::cout << "[AVE] Dashboard:   http://localhost:" << port << "/index.html\n";
        std::cout << "[AVE] OpenAPI:     http://localhost:" << port << "/openapi.yaml\n";
        std::cout << "[AVE] DB path:     " << db_path << "\n";

        svr.listen("0.0.0.0", port);

    } catch (const std::exception& e) {
        std::cerr << "[FATAL] " << e.what() << "\n";
        return 1;
    }
    return 0;
}
