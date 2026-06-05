# Algorithm Validation Engine (AVE)

> **Interview practice project** — Northrop Grumman Back End Application Engineer (C++)
>
> C++17 REST API implementing a discrete Kalman filter and OLS regression on Rocky Linux 9
> (binary-compatible with RHEL 9). Demonstrates MAR-specified algorithm implementation,
> GTest unit testing, CMake build system, and the DR fix workflow.

---

## Service Map

| Service | URL | Purpose |
|---|---|---|
| **C++ REST API** | http://localhost:8080 | Algorithm endpoints |
| **Dashboard** | http://localhost:8080/index.html | Visualisation UI |
| **OpenAPI spec** | http://localhost:8080/openapi.yaml | Machine-readable API definition |
| **Swagger UI** | http://localhost:8090 | Interactive API explorer |
| **Adminer** | http://localhost:8889 | SQLite database browser |

---

## Quick Start (Docker — recommended)

```bash
git clone https://github.com/your-username/cpp-algorithm-engine.git
cd cpp-algorithm-engine

# Build and start all services (API + Swagger UI + Adminer)
docker compose up --build
```

> **First build note**: CMake downloads Crow/nlohmann/SQLiteCpp/GTest via FetchContent.
> This takes ~5–10 min on first run; subsequent builds use the Docker layer cache.

---

## Native Build (Rocky Linux 9 / RHEL 9 / Fedora)

```bash
# Install toolchain
sudo dnf install -y gcc-c++ cmake make git openssl-devel

# Configure + build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target ave_server -j$(nproc)

# Run the server
./build/ave_server

# Run unit tests
ctest --test-dir build --output-on-failure
```

### Build presets (CMakePresets.json)

```bash
cmake --preset debug    # Debug, binary in build/debug/
cmake --preset release  # Optimised, binary in build/release/
cmake --preset asan     # Debug + AddressSanitizer, binary in build/asan/
```

---

## API Reference

### `POST /api/kalman/run` — Discrete Kalman Filter

```bash
curl -X POST http://localhost:8080/api/kalman/run \
  -H "Content-Type: application/json" \
  -d '{
    "x0": 0.0,
    "p0": 1.0,
    "q":  0.1,
    "r":  1.0,
    "measurements": [1.2, 2.1, 3.0, 3.8, 5.1]
  }'
```

| Field | Description |
|---|---|
| `x0` | Initial state estimate |
| `p0` | Initial state covariance (> 0) |
| `q` | Process noise covariance (≥ 0) |
| `r` | Measurement noise covariance (> 0) |
| `measurements` | Array of 1–1000 scalar observations |

### `POST /api/leastsq/run` — OLS Regression

```bash
curl -X POST http://localhost:8080/api/leastsq/run \
  -H "Content-Type: application/json" \
  -d '{
    "points": [
      {"x": 1, "y": 3},
      {"x": 2, "y": 5},
      {"x": 3, "y": 7},
      {"x": 4, "y": 9}
    ]
  }'
```

### `GET /api/runs` — Computation history

```bash
curl http://localhost:8080/api/runs
```

### `GET /health`

```bash
curl http://localhost:8080/health
# {"status":"ok","service":"algorithm-validation-engine"}
```

---

## Running Tests

```bash
# Unit tests only
ctest --test-dir build --output-on-failure

# With AddressSanitizer
cmake --preset asan
cmake --build build/asan --target ave_tests -j$(nproc)
ASAN_OPTIONS=abort_on_error=1 ./build/asan/ave_tests

# With Valgrind (Rocky Linux / RHEL)
sudo dnf install -y valgrind
valgrind --tool=memcheck --leak-check=full --error-exitcode=1 ./build/debug/ave_tests
```

---

## Adminer — Browse SQLite

1. Open http://localhost:8889
2. System: **SQLite 3**
3. Database: `/data/ave.db`
4. Leave username/password blank
5. Click **Login**

Browse the `computation_runs` table to inspect persisted algorithm outputs.

---

## Swagger UI — Try the API

1. Open http://localhost:8090
2. Select an endpoint (e.g. `POST /api/kalman/run`)
3. Click **Try it out**
4. Edit the example request body
5. Click **Execute**

---

## Project Structure

```
cpp-algorithm-engine/
├── CMakeLists.txt               # Build config: ave_lib, ave_server, ave_tests
├── CMakePresets.json            # Presets: debug, release, asan
├── Dockerfile                   # Multi-stage: rockylinux:9 builder → runtime
├── docker-compose.yml           # Services: api, swagger-ui, adminer
├── openapi.yaml                 # Hand-authored OpenAPI 3.0 spec
│
├── include/
│   ├── algorithms/
│   │   ├── KalmanFilter.h       # Discrete KF interface (MAR)
│   │   └── LeastSquares.h       # OLS interface (MAR)
│   ├── db/
│   │   └── Repository.h         # SQLite CRUD interface
│   └── api/
│       └── Router.h             # HTTP route declarations
│
├── src/
│   ├── main.cpp                 # Entry point — starts httplib server
│   ├── algorithms/
│   │   ├── KalmanFilter.cpp     # Predict + update implementation
│   │   └── LeastSquares.cpp     # Normal equations implementation
│   ├── db/
│   │   └── Repository.cpp       # SQLiteCpp persistence
│   └── api/
│       └── Router.cpp           # All route handlers
│
├── db/
│   └── schema.sql               # Schema reference (auto-created by Repository)
│
├── test/
│   ├── kalman_test.cpp          # 7 GTest cases for KalmanFilter
│   └── leastsq_test.cpp         # 8 GTest cases for LeastSquares
│
├── public/
│   └── index.html               # Static dashboard + Chart.js
│
└── docs/
    ├── debugging-guide.md       # GDB, Valgrind, ASan usage + DR workflow
    └── interview-talking-points.md
```

---

## Environment Variables

| Variable | Default | Description |
|---|---|---|
| `PORT` | `8080` | HTTP listen port |
| `DB_PATH` | `ave.db` | SQLite database file path |
| `PUBLIC_DIR` | `./public` | Directory served as static files |

---

## DR (Discrepancy Report) Practice

GitHub Issues simulate the DR process from the JD.

```bash
# 1. Open a GitHub Issue labelled 'DR' describing the bug
# 2. Create a fix branch
git checkout -b dr/01-kalman-covariance-overflow

# 3. Add a failing test that reproduces the DR
# 4. Fix the code
# 5. Verify CI passes: build + tests + Valgrind
ctest --test-dir build --output-on-failure

# 6. Open a PR — merge only when all CI jobs are green
# 7. Close the Issue, tag the commit
git tag dr-01-fixed
```

See [docs/debugging-guide.md](docs/debugging-guide.md) for the full GDB + Valgrind reference.

---

## CI/CD Pipeline

```
Push / PR → main
│
├── build-and-test   (Rocky Linux 9) — CMake Release, CTest
├── asan-check       (Rocky Linux 9) — CMake Debug + -fsanitize=address
├── valgrind-check   (Rocky Linux 9) — Valgrind memcheck, error-exitcode=1
└── docker-build     (ubuntu-latest) — multi-stage build + smoke test
```

All four jobs must pass before a PR can merge to `main`.

---

## Cloud Deployment (AWS EC2 / RHEL 9)

```bash
# Launch EC2 with Rocky Linux 9 AMI, open ports 8080, 8090, 8889
# SSH in and:
sudo dnf install -y docker docker-compose-plugin
sudo systemctl start docker

git clone https://github.com/your-username/cpp-algorithm-engine.git
cd cpp-algorithm-engine
docker compose up -d --build
```

---

*Practice project for Northrop Grumman Back End Application Engineer (C++) interview — 2026*
