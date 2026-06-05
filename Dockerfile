# ---- Stage 1: Builder (Rocky Linux 9 / RHEL 9 compatible) ----
FROM rockylinux:9 AS builder

# Install build toolchain (mirrors a real RHEL 9 dev workstation)
RUN dnf install -y \
        gcc-c++ \
        cmake \
        make \
        git \
        openssl-devel \
    && dnf clean all

WORKDIR /build

# Copy all sources — CMake FetchContent will download deps at configure time
COPY CMakeLists.txt CMakePresets.json ./
COPY include/   include/
COPY src/       src/
COPY test/      test/
COPY db/        db/
COPY public/    public/
COPY openapi.yaml .

# Configure and build release target
RUN cmake -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build --target ave_server -j"$(nproc)"

# ---- Stage 2: Runtime (minimal Rocky Linux 9) ----
FROM rockylinux:9-minimal AS runtime

RUN microdnf install -y \
        libstdc++ \
        openssl-libs \
        curl \
        shadow-utils \
    && microdnf clean all \
    # Non-root user for defence-in-depth
    && useradd -r -u 1001 -g 0 ave

WORKDIR /app

COPY --from=builder /build/build/ave_server .
COPY --from=builder /build/public/          public/
COPY --from=builder /build/openapi.yaml     .

RUN mkdir -p /data \
    && chown -R ave:0 /app /data \
    && chmod -R g=u   /app /data

USER ave
VOLUME ["/data"]

ENV PORT=8080
ENV DB_PATH=/data/ave.db
ENV PUBLIC_DIR=/app/public

EXPOSE 8080

HEALTHCHECK --interval=30s --timeout=5s --start-period=20s --retries=3 \
    CMD curl -sf http://localhost:8080/health || exit 1

CMD ["./ave_server"]
