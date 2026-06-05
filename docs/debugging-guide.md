# Debugging Guide — Algorithm Validation Engine

Quick reference for GDB and Valgrind on Rocky Linux 9 (RHEL 9 compatible).

---

## GDB — Interactive Debugger

### Start inside Docker
```bash
docker compose run --rm -it api gdb ./ave_server
```

### Common GDB commands
```
# Set a breakpoint on the Kalman update step
(gdb) break KalmanFilter::update

# Set a breakpoint by file + line
(gdb) break src/algorithms/KalmanFilter.cpp:34

# Run the server
(gdb) run

# When the breakpoint hits, inspect the state
(gdb) print x_
(gdb) print p_
(gdb) print k_

# Step through the covariance update line by line
(gdb) next
(gdb) next

# Continue to next breakpoint
(gdb) continue

# Print a struct field
(gdb) print run.algorithm

# Backtrace when a crash occurs
(gdb) backtrace

# Quit
(gdb) quit
```

### Debug the test binary instead
```bash
docker compose run --rm -it api gdb ./build/ave_tests

(gdb) break KalmanFilterTest_UpdateReducesCovariance_Test::TestBody
(gdb) run
```

---

## Valgrind — Memory Leak & Error Detector

### Run all tests under Valgrind
```bash
# Inside a debug build container
docker compose run --rm api \
  valgrind --tool=memcheck \
           --leak-check=full \
           --show-leak-kinds=all \
           --track-origins=yes \
           --error-exitcode=1 \
           ./build/ave_tests
```

### Typical clean output
```
HEAP SUMMARY:
    in use at exit: 0 bytes in 0 blocks
  total heap usage: N allocs, N frees, X bytes allocated
LEAK SUMMARY:
  definitely lost: 0 bytes in 0 blocks
ERROR SUMMARY: 0 errors from 0 contexts
```

### Interpreting errors
| Error type | Meaning |
|---|---|
| `definitely lost` | Memory allocated but never freed — memory leak |
| `Invalid read/write` | Accessing memory outside allocated bounds |
| `Use of uninitialised value` | Reading a variable before assignment |
| `Invalid free` | Double-free or freeing stack memory |

---

## AddressSanitizer — Fast Compile-Time Memory Check

### Build with ASan preset
```bash
# Native (Rocky Linux 9 / RHEL 9)
cmake --preset asan
cmake --build build/asan --target ave_tests
cd build/asan && ASAN_OPTIONS=abort_on_error=1 ./ave_tests
```

ASan runs ~2× faster than Valgrind and catches use-after-free, buffer overflows,
and stack overflows at runtime. It is the preferred tool for CI; use Valgrind for
deeper leak detection.

---

## DR (Discrepancy Report) Practice Workflow

This mirrors the formal DR process described in the Northrop Grumman JD.

1. **Open a GitHub Issue** labelled `DR` — e.g., "DR-01: Kalman covariance overflows for large Q"
2. **Create a fix branch**: `git checkout -b dr/01-kalman-covariance-overflow`
3. **Reproduce with a failing test** — add a new `TEST(...)` that fails on the current code
4. **Fix the code**
5. **Verify**: run `ctest --output-on-failure` and `valgrind ./ave_tests`
6. **Open a PR** — CI must be green before merge
7. **Close the Issue** and tag the commit: `git tag dr-01-fixed`

This is the same lifecycle you would follow with a formal DR tracking system
(e.g., Jira, ClearQuest) in a defense program.
