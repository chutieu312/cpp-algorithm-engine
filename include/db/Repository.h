/**
 * @file Repository.h
 * @brief SQLite persistence layer for Algorithm Validation Engine computation runs.
 *
 * Each call to save() inserts one row into the computation_runs table and
 * returns the persisted record. findAll() retrieves the most recent runs.
 */
#pragma once

#include <string>
#include <vector>
#include <SQLiteCpp/SQLiteCpp.h>

/// One row from the computation_runs table.
struct ComputationRun {
    int         id;
    std::string algorithm;   ///< "kalman" or "leastsq"
    std::string input_json;
    std::string output_json;
    std::string created_at;
};

class Repository {
public:
    /// Open (or create) the SQLite database at db_path and ensure schema exists.
    explicit Repository(const std::string& db_path);

    /// Insert a new computation run and return the persisted record.
    ComputationRun save(const std::string& algorithm,
                        const std::string& input_json,
                        const std::string& output_json);

    /// Return the last @p limit runs ordered by id DESC.
    std::vector<ComputationRun> findAll(int limit = 100);

private:
    SQLite::Database db_;
    void initialize();
};
