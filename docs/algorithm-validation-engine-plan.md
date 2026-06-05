# JD Practice Plan — Back End Application Engineer (C++) | Northrop Grumman

> **Role**: Back End Application Engineer (C++)
> **Company**: Northrop Grumman
> **Location**: Redondo Beach, CA
> **Start Date**: July 25, 2026

---

## Original JD Summary

A C++ software developer role embedded in a fast-paced agile team building and maintaining software that runs on Red Hat Enterprise Linux (RHEL) VMs, servers, and workstations. Core work involves implementing new capabilities against formal Mathematical Algorithm Requirements (MARs), troubleshooting and fixing bugs logged as Discrepancy Reports (DRs), and collaborating with integrated product teams (IPTs) across multiple software subsystems.

---

## 1. JD Skill Extraction

### Required Technical Skills
- C++ software development
- Red Hat Enterprise Linux (RHEL) — VMs, servers, workstations
- Mathematical algorithm implementation (MARs)
- Unit testing of software capabilities
- Discrepancy Report (DR) identification, troubleshooting, and fix delivery
- Agile development process

### Preferred / Implied Technical Skills
- CMake or equivalent build system (standard for RHEL C++ projects)
- GDB / Valgrind (debugging and memory analysis on Linux)
- Version control (Git, likely GitLab in defense environments)
- Static analysis and code review practices
- Familiarity with numerical/scientific computing (linear algebra, signal processing)
- Inter-process or inter-subsystem communication (sockets, shared memory, or message passing)

### Soft Skills and Collaboration Expectations
- Ability to work within an Integrated Product Team (IPT) with SMEs and senior engineers
- Clear written communication (DR write-ups, design notes)
- Cross-subsystem collaboration and coordination
- Self-directed in a fast-paced, deadline-driven defense program

---

## 2. Skill Categories

| Category | Skills |
|---|---|
| **Backend** | C++17, Crow REST framework, CMake, vcpkg, RHEL/Rocky Linux |
| **Frontend** | Minimal — static HTML/JS dashboard (not a JD focus) |
| **Database** | SQLite via SQLiteCpp |
| **Cloud** | Not explicitly required — local/on-prem emphasis |
| **DevOps / CI/CD** | Docker (Rocky Linux base), GitHub Actions, CMake build pipeline |
| **Testing** | Google Test (GTest), CTest, Valgrind memory check |
| **Security** | Input validation, memory safety (smart pointers, AddressSanitizer) |
| **AI Tools / Automation** | Not mentioned |
| **Other** | Mathematical algorithms (Kalman filter, least squares), DR simulation via GitHub Issues, agile sprint via GitHub Projects |

---

## 3. Recommended Mini Project

### Algorithm Validation Engine (AVE)

A C++ backend service that implements two classic mathematical algorithms — a **discrete Kalman filter** and **ordinary least squares (OLS) regression** — exposes them over a lightweight REST API, persists computation runs to a SQLite database, and is fully containerized on a RHEL-compatible base image.

The project is:
- **Small**: ~800–1200 lines of C++, no large frameworks
- **Fast to build**: One `docker compose up` starts everything
- **Easy to run locally**: CMake + vcpkg or Docker
- **Easy to explain in an interview**: "I implemented a Kalman filter from the math spec, tested it with GTest, and exposed it as an API — just like delivering a MAR-specified capability with unit tests."
- **Domain-relevant**: Kalman filters are used extensively in aerospace/defense guidance and tracking systems (directly relevant to Northrop Grumman's work)

---

## 4. Why This Project Matches the JD

| JD Responsibility | How AVE Demonstrates It |
|---|---|
| "Design, code, and unit test new SW capabilities per MARs" | Kalman filter and OLS are implemented from their mathematical definitions (the "MAR"), with GTest unit tests validating each algorithm step |
| "RHEL VMs, servers, and workstations" | Docker image uses Rocky Linux 9 (binary-compatible RHEL 9); all build and run instructions target Linux |
| "Identify, troubleshoot, and provide SW fixes to DRs" | GitHub Issues simulate DRs; project includes intentional known-bug branches to practice the fix-and-test DR workflow |
| "Agile team environment" | GitHub Projects kanban board tracks phases as sprint stories; Git feature branches with PR reviews |
| "Collaborate with SW IPT SMEs" | OpenAPI spec and Swagger UI let team members explore the API without reading C++; inline Doxygen comments document algorithm intent |

---

## 5. Architecture Overview

```
┌─────────────────────────────────────────────────────────┐
│  Docker Compose (Rocky Linux 9 environment)              │
│                                                          │
│  ┌──────────────┐    ┌───────────────┐  ┌───────────┐  │
│  │  C++ REST API │    │  Swagger UI   │  │  Adminer  │  │
│  │  (Crow 1.x)   │◄──│  :8080/docs   │  │   :8888   │  │
│  │  :8080        │   └───────────────┘  └─────┬─────┘  │
│  └──────┬────────┘                             │        │
│         │ SQLiteCpp                            │        │
│  ┌──────▼────────┐                             │        │
│  │  SQLite DB    │◄────────────────────────────┘        │
│  │  ave.db       │                                      │
│  └───────────────┘                                      │
└─────────────────────────────────────────────────────────┘
```

- **Backend**: C++17 application using the Crow micro-framework for REST endpoints. Implements `KalmanFilter` and `LeastSquares` classes in separate modules.
- **Database**: SQLite via SQLiteCpp, storing each computation run (inputs, outputs, timestamp, algorithm type).
- **Frontend**: Static `index.html` + Vanilla JS — fetches API results and plots Kalman filter state estimates. No build step required.
- **API Explorer**: `swaggerapi/swagger-ui` Docker container pointing to `/openapi.yaml` served by the C++ app.
- **DB Admin UI**: Adminer at port 8888 for browsing `computation_runs` table.
- **CI/CD**: GitHub Actions — builds with CMake, runs GTest suite, runs Valgrind memory check.
- **Security**: Input validation at REST boundary, smart pointers throughout (no raw `new/delete`), AddressSanitizer in CI build.
- **Developer Exploration**: Swagger UI + Adminer — both auto-start with `docker compose up`.

---

## 6. Tech Stack Mapping

| JD Skill | Project Feature | How I Can Explain It in an Interview |
|---|---|---|
| C++ development | All business logic in C++17: KalmanFilter, LeastSquares, Router classes | "I wrote the core in C++17 using templates, smart pointers, and STL — the same language standards used in RHEL production systems." |
| RHEL Linux | Rocky Linux 9 Docker base image; all CMake/GDB commands target Linux | "Rocky Linux 9 is binary-compatible with RHEL 9. I built and ran the app entirely in that environment, including debugging with GDB inside the container." |
| Mathematical Algorithm Requirements (MARs) | Discrete Kalman filter (predict/update cycle) and OLS regression from first principles | "I treated the Kalman filter equations as the 'MAR' and implemented each step — state prediction, covariance update, Kalman gain — then validated the output against analytical test cases." |
| Unit testing | Google Test suite: `kalman_test.cpp`, `leastsq_test.cpp` with edge cases and numerical tolerance checks | "Each algorithm has dedicated GTest cases covering nominal runs, near-singular matrices, and zero-innovation updates — the same rigor expected for formal unit test evidence." |
| Agile / DR workflow | GitHub Issues labeled 'DR'; fix branches `dr/42-kalman-divergence`; CTest in CI gate | "I simulate the DR lifecycle: open an issue, reproduce with a failing test, fix, and close only when CI passes — that's the same workflow I'd use with a formal DR tracking system." |
| CMake build system | `CMakeLists.txt` with targets: `ave_lib`, `ave_server`, `ave_tests` | "CMake is the standard build system for C++ on Linux. I structured it with a static library target so the server and test binaries share the same compiled algorithm code." |
| Debugging on Linux | Valgrind memcheck step in CI; GDB launch config in README | "I ran Valgrind in CI to catch memory leaks before merge. Locally I use GDB — I can show stepping through the Kalman predict step and inspecting the covariance matrix." |
| Cross-subsystem collaboration | OpenAPI spec + Swagger UI; Doxygen comments on public headers | "I wrote an OpenAPI spec so any team member — even without C++ knowledge — can call the algorithm endpoints immediately. That's the same value a SW IPT member gets from clear interface documentation." |
| Docker / containerization | Multi-stage Dockerfile (build stage: Rocky Linux + CMake; runtime stage: slim Rocky Linux) | "Multi-stage build keeps the runtime image lean — only the binary and shared libs, no build toolchain. That's important for deployment to servers and VMs." |

---

## 7. Step-by-Step Build Plan

### Phase 1 — Backend Foundation (C++ REST API skeleton)
- Initialize CMake project with `vcpkg.json` or `conanfile.txt`
- Add Crow dependency (REST framework) + nlohmann/json
- Implement `GET /health` and `GET /api/algorithms` endpoints
- Write `main.cpp` that starts Crow on port 8080
- Build and run locally with CMake

### Phase 2 — Algorithm Layer + SQLite
- Implement `KalmanFilter` class: constructor, `predict()`, `update()`, `getState()`
- Implement `LeastSquares` class: constructor, `fit()`, `predict()`
- Add SQLiteCpp dependency via vcpkg
- Create `db/schema.sql` — `computation_runs` table (id, algorithm, input_json, output_json, created_at)
- Implement `POST /api/kalman/run` and `POST /api/leastsq/run` — runs algorithm and persists result
- Implement `GET /api/runs` — retrieves history from SQLite

### Phase 3 — Static Frontend Dashboard
- Create `public/index.html` — fetches `GET /api/runs` and renders a simple table
- Add a form to submit Kalman filter inputs and chart state estimates with Chart.js (CDN)
- Crow serves `public/` as static files

### Phase 4 — Testing (GTest + Valgrind)
- Write `test/kalman_test.cpp` — 5+ test cases: standard run, zero innovation, divergence prevention
- Write `test/leastsq_test.cpp` — 5+ test cases: perfect fit, noisy data, single point
- Add CTest integration in `CMakeLists.txt`
- Add CMake preset for AddressSanitizer build (`-fsanitize=address`)

### Phase 5 — Docker
- Write multi-stage `Dockerfile`:
  - Stage 1 (`builder`): `rockylinux:9` + GCC 12 + CMake + vcpkg bootstrap + build
  - Stage 2 (`runtime`): `rockylinux:9-minimal` + copy binary + set entrypoint
- Write `docker-compose.yml` with `api`, `swagger-ui`, and `adminer` services

### Phase 6 — CI/CD (GitHub Actions)
- `.github/workflows/ci.yml`:
  - Job 1: `build-and-test` — CMake configure, build, CTest
  - Job 2: `asan-check` — build with AddressSanitizer, run tests
  - Job 3: `valgrind-check` — run GTest suite under Valgrind inside Rocky Linux container
  - Job 4: `docker-build` — build Docker image, verify container starts

### Phase 7 — Cloud Deployment Plan
- Push Docker image to GitHub Container Registry (GHCR)
- Deploy to a single DigitalOcean Droplet ($6/mo) or AWS EC2 t3.micro with `docker compose up -d`
- For a more realistic RHEL-like production demo: deploy to a Rocky Linux VM on Azure or AWS

### Phase 8 — Developer Exploration Tools
- **Swagger UI**: Add `swaggerapi/swagger-ui` Docker service pointing to C++ app's `/openapi.yaml`
- **Adminer**: Add `adminer:4` Docker service for SQLite inspection
- Write `openapi.yaml` with all endpoints, request/response schemas, and example payloads
- Update README Service Map table with all tool URLs
- Document GDB and Valgrind usage in `docs/debugging-guide.md`

### Phase 9 — Interview Demo Script
- Write `docs/interview-talking-points.md`
- Practice walking through: "Here's the MAR I implemented → here's the code → here's the test → here's it running"
- Prepare explanation of Kalman filter math at a whiteboard level (predict/update)
- Rehearse DR simulation: show a GitHub Issue, the fix branch, and the CI gate

---

## 8. Developer Exploration Tools

### API Explorer — Swagger UI
- **Tool**: `swaggerapi/swagger-ui` Docker container
- **Why**: The C++ Crow framework does not auto-generate OpenAPI specs, so a hand-authored `openapi.yaml` is served as a static file by the C++ app. Swagger UI points to it.
- **Port**: 8090
- **URL**: http://localhost:8090
- **Auth**: JWT bearer token — add `Authorize` button in `openapi.yaml` with `bearerAuth` security scheme
- **docker-compose service**:
```yaml
swagger-ui:
  image: swaggerapi/swagger-ui
  ports:
    - "8090:8080"
  environment:
    URL: "http://localhost:8080/openapi.yaml"
  depends_on:
    - api
```

### Component Explorer — Storybook
- **Decision**: **Skip** — there is no React/Vue/Angular component library. The frontend is a minimal static HTML page. Not relevant to this JD.

### Database Admin UI — Adminer
- **Tool**: `adminer:4`
- **Why**: SQLite is the database engine; Adminer supports SQLite natively.
- **Port**: 8888
- **URL**: http://localhost:8888
- **Login**: System = SQLite, Server = `/data/ave.db` (Docker volume path)
- **docker-compose service**:
```yaml
adminer:
  image: adminer:4
  ports:
    - "8888:8080"
  volumes:
    - db-data:/data
  depends_on:
    - api
```

### Service Map (for README)

| Service | URL | Purpose |
|---|---|---|
| C++ REST API | http://localhost:8080 | Algorithm endpoints |
| Static Dashboard | http://localhost:8080/index.html | Visualization |
| Swagger UI | http://localhost:8090 | Interactive API explorer |
| Adminer | http://localhost:8888 | SQLite database browser |

---

## 9. Project Structure

```
cpp-algorithm-engine/
├── CMakeLists.txt               # Root build config (targets: ave_lib, ave_server, ave_tests)
├── CMakePresets.json            # Presets: debug, release, asan
├── vcpkg.json                   # Dependencies: crow, nlohmann-json, sqlitecpp, gtest
├── Dockerfile                   # Multi-stage: rockylinux:9 builder → rockylinux:9-minimal runtime
├── docker-compose.yml           # Services: api, swagger-ui, adminer
├── openapi.yaml                 # Hand-authored OpenAPI 3.0 spec
├── README.md                    # Setup, Service Map, GDB/Valgrind guide
│
├── include/
│   ├── algorithms/
│   │   ├── KalmanFilter.h       # Discrete KF interface
│   │   └── LeastSquares.h       # OLS interface
│   ├── db/
│   │   └── Repository.h         # ComputationRun CRUD
│   └── api/
│       └── Router.h             # Crow route registrations
│
├── src/
│   ├── main.cpp                 # Entry point, starts Crow on :8080
│   ├── algorithms/
│   │   ├── KalmanFilter.cpp
│   │   └── LeastSquares.cpp
│   ├── db/
│   │   └── Repository.cpp
│   └── api/
│       └── Router.cpp
│
├── db/
│   └── schema.sql               # CREATE TABLE computation_runs ...
│
├── test/
│   ├── CMakeLists.txt
│   ├── kalman_test.cpp          # GTest: predict/update, edge cases, tolerance
│   └── leastsq_test.cpp         # GTest: fit, predict, single point
│
├── public/
│   ├── index.html               # Static dashboard + Chart.js
│   └── openapi.yaml             # Served at /openapi.yaml (same file as root)
│
├── docs/
│   ├── interview-talking-points.md
│   └── debugging-guide.md       # GDB commands, Valgrind usage
│
└── .github/
    └── workflows/
        └── ci.yml               # build-and-test, asan-check, valgrind-check, docker-build
```

---

## 10. Local Setup Commands

### Option A — Docker (recommended, RHEL-compatible)

```bash
# Clone the repo
git clone https://github.com/your-username/cpp-algorithm-engine.git
cd cpp-algorithm-engine

# Start everything (API + Swagger UI + Adminer)
docker compose up --build

# Services:
#   API:         http://localhost:8080
#   Swagger UI:  http://localhost:8090
#   Adminer:     http://localhost:8888
```

### Option B — Native build (RHEL 9 / Rocky Linux 9 / Fedora)

```bash
# Install dependencies
sudo dnf install -y cmake gcc g++ git curl zip unzip tar

# Bootstrap vcpkg
git clone https://github.com/microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh

# Configure and build
cmake --preset debug -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build/debug

# Run the server
./build/debug/ave_server

# Run tests
cd build/debug && ctest --output-on-failure

# Run with AddressSanitizer
cmake --preset asan -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build/asan
cd build/asan && ctest --output-on-failure
```

### Option C — GDB debugging inside Docker

```bash
docker compose run --rm api gdb ./ave_server
# Inside GDB:
(gdb) break KalmanFilter::update
(gdb) run
(gdb) print state_
```

---

## 11. Testing Plan

### Unit Tests (Google Test / CTest)

| Test File | Test Cases | What It Validates |
|---|---|---|
| `kalman_test.cpp` | `PredictIncreasesCovariance` | After predict(), covariance P should grow by Q |
| `kalman_test.cpp` | `UpdateReducesCovariance` | After update(), covariance P should decrease |
| `kalman_test.cpp` | `KalmanGainConverges` | Over many iterations, Kalman gain K should converge |
| `kalman_test.cpp` | `ZeroInnovationNoStateChange` | If measurement equals prediction, state unchanged |
| `kalman_test.cpp` | `NumericalStabilityLargeQ` | Large process noise doesn't cause NaN/Inf |
| `leastsq_test.cpp` | `PerfectLinearFit` | OLS on y=2x+1 returns exact slope and intercept |
| `leastsq_test.cpp` | `NoisyFitWithinTolerance` | Noisy data fit is within 5% of true coefficients |
| `leastsq_test.cpp` | `SinglePointDegenerateCase` | Returns error or handles single-point input gracefully |
| `leastsq_test.cpp` | `PredictFromFittedModel` | Predict(x) matches expected y within tolerance |

**Why GTest**: Industry standard for C++, integrates with CTest and CMake, and produces JUnit-compatible XML output that GitHub Actions can parse as test results.

### Memory Safety (Valgrind)
- `valgrind --tool=memcheck --leak-check=full ./ave_tests` — run in CI
- Ensures no raw memory leaks in algorithm code
- Documents the "before fix" and "after fix" states for a simulated DR

### Integration Smoke Test
- `curl http://localhost:8080/health` returns 200 in CI docker step
- `curl -X POST http://localhost:8080/api/kalman/run -d '{...}'` returns valid JSON with state estimates

---

## 12. CI/CD Plan

### GitHub Actions — `.github/workflows/ci.yml`

```
Trigger: push to main, PR to main

Jobs (run in parallel where possible):

  build-and-test (Rocky Linux 9 container)
  ├── Checkout code
  ├── Install: cmake, gcc, vcpkg dependencies
  ├── cmake --preset release
  ├── cmake --build
  └── ctest --output-on-failure

  asan-check (Rocky Linux 9 container)
  ├── cmake --preset asan
  ├── cmake --build
  └── ctest --output-on-failure (ASAN will abort on memory errors)

  valgrind-check (Rocky Linux 9 container)
  ├── cmake --preset debug
  ├── cmake --build
  └── valgrind --error-exitcode=1 ./ave_tests

  docker-build (ubuntu-latest)
  ├── docker build -t ave:ci .
  ├── docker run --rm -d -p 8080:8080 ave:ci
  ├── curl --retry 5 http://localhost:8080/health
  └── docker stop
```

**Why this pipeline**: Mirrors a real defense software CI gate — build must compile, all unit tests must pass, no memory errors — before merge is allowed. That directly parallels a formal SW IPT code review and test evidence requirement.

---

## 13. Cloud Deployment Plan

### Option 1 — DigitalOcean Droplet (simplest, cheapest)
- Provision a Rocky Linux 9 Droplet ($6/month, 1 vCPU, 1 GB RAM)
- SSH in, install Docker and Docker Compose
- `git clone` + `docker compose up -d`
- Publicly accessible at the Droplet IP for demo purposes

### Option 2 — AWS EC2 t3.micro (most RHEL-like, free tier)
- Launch EC2 with RHEL 9 or Rocky Linux 9 AMI
- Security group: open ports 8080, 8090, 8888
- Same `docker compose up -d` deployment
- More representative of real Northrop Grumman-style infrastructure

### Option 3 — GitHub Container Registry + SSH deploy (most automated)
- GitHub Actions builds and pushes image to GHCR on merge to `main`
- SSH deploy step pulls new image and restarts containers on the server
- One-click rollback by pinning the previous image tag

**Recommendation for interview**: Deploy to AWS EC2 (RHEL 9) and say: _"I deployed on an RHEL-compatible EC2 instance to simulate the production environment described in the JD — the same image that builds in CI ships to the server."_

---

## 14. Interview Talking Points

### On C++ and the Role
- "Every capability is implemented in C++17 — I deliberately avoided Python or shell scripts for the core algorithms to stay true to the role."
- "I used smart pointers and RAII throughout. There are zero raw `new/delete` calls in the algorithm layer."
- "The CMake project has three targets: a static library for algorithms, a server binary, and a test binary — all sharing the same compiled code."

### On Mathematical Algorithm Requirements (MARs)
- "I treated the Kalman filter equations as the MAR document. I implemented each equation — the prediction step, the covariance update, the Kalman gain calculation — and then wrote GTest cases that validate each one against analytically known outputs."
- "This is the same workflow I'd use in the role: receive a mathematical spec, implement it in C++, and produce test evidence that it's correct."

### On RHEL / Linux Environment
- "My Docker base image is Rocky Linux 9, which is binary-compatible with RHEL 9. I built and ran everything inside that environment, including debugging with GDB."
- "I can demonstrate building from scratch on a clean RHEL machine using the README commands."

### On Unit Testing
- "I have nine GTest test cases covering nominal behavior, edge cases, and numerical stability. All pass in CI on every commit."
- "I also run the test suite under Valgrind in CI to catch memory issues before they become DRs."

### On the DR Workflow
- "GitHub Issues simulate the DR process. I have a practice branch `dr/01-kalman-covariance-overflow` that shows how I'd receive a DR, reproduce it with a failing test, fix the code, and close the DR only when CI goes green."

### On Collaboration and IPT
- "I wrote an OpenAPI spec so any team member — even someone who doesn't read C++ — can explore and call the algorithm endpoints through Swagger UI. That's the interface contract I'd maintain for a software IPT."
- "I added Doxygen comments on all public headers so the algorithm intent is clear to an SME reviewing the code."

### On Agile
- "I tracked each phase as a GitHub Projects card. Feature work goes in a branch, gets reviewed as a PR, and merges only when CI passes — the same lightweight agile gate I'd follow on a real program."

---

*Plan generated: 2026-06-04*
*Role: Back End Application Engineer (C++) — Northrop Grumman*
