-- Algorithm Validation Engine — SQLite schema reference
-- The Repository class creates this table automatically on startup.
-- This file is kept as documentation and for direct SQLite inspection.

CREATE TABLE IF NOT EXISTS computation_runs (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    algorithm   TEXT    NOT NULL,           -- 'kalman' or 'leastsq'
    input_json  TEXT    NOT NULL,           -- raw request body
    output_json TEXT    NOT NULL,           -- computed result
    created_at  TEXT    NOT NULL
                DEFAULT (strftime('%Y-%m-%dT%H:%M:%SZ','now'))
);

CREATE INDEX IF NOT EXISTS idx_runs_algorithm
    ON computation_runs(algorithm);
