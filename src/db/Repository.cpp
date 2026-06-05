#include "db/Repository.h"
#include <cstdint>

Repository::Repository(const std::string& db_path)
    : db_(db_path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE)
{
    initialize();
}

void Repository::initialize()
{
    db_.exec(
        "CREATE TABLE IF NOT EXISTS computation_runs ("
        "  id          INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  algorithm   TEXT    NOT NULL,"
        "  input_json  TEXT    NOT NULL,"
        "  output_json TEXT    NOT NULL,"
        "  created_at  TEXT    NOT NULL"
        "               DEFAULT (strftime('%Y-%m-%dT%H:%M:%SZ','now'))"
        ")"
    );
    db_.exec(
        "CREATE INDEX IF NOT EXISTS idx_runs_algorithm "
        "ON computation_runs(algorithm)"
    );
}

ComputationRun Repository::save(const std::string& algorithm,
                                 const std::string& input_json,
                                 const std::string& output_json)
{
    SQLite::Statement insert(db_,
        "INSERT INTO computation_runs (algorithm, input_json, output_json) "
        "VALUES (?, ?, ?)");
    insert.bind(1, algorithm);
    insert.bind(2, input_json);
    insert.bind(3, output_json);
    insert.exec();

    const long long id = db_.getLastInsertRowid();

    SQLite::Statement select(db_,
        "SELECT id, algorithm, input_json, output_json, created_at "
        "FROM computation_runs WHERE id = ?");
    select.bind(1, static_cast<std::int64_t>(id));
    select.executeStep();

    return ComputationRun{
        select.getColumn(0).getInt(),
        select.getColumn(1).getString(),
        select.getColumn(2).getString(),
        select.getColumn(3).getString(),
        select.getColumn(4).getString()
    };
}

std::vector<ComputationRun> Repository::findAll(int limit)
{
    SQLite::Statement stmt(db_,
        "SELECT id, algorithm, input_json, output_json, created_at "
        "FROM computation_runs ORDER BY id DESC LIMIT ?");
    stmt.bind(1, limit);

    std::vector<ComputationRun> runs;
    while (stmt.executeStep()) {
        runs.push_back(ComputationRun{
            stmt.getColumn(0).getInt(),
            stmt.getColumn(1).getString(),
            stmt.getColumn(2).getString(),
            stmt.getColumn(3).getString(),
            stmt.getColumn(4).getString()
        });
    }
    return runs;
}
