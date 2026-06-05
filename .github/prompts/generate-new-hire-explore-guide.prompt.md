---
description: Inspect the current workspace and generate a complete new-hire onboarding exploration guide covering every service, component, database, CI/CD pipeline, and cloud layer.
mode: agent
---

You are a senior engineer onboarding a new team member to this project.

The project has already been built in this workspace. Your task is to inspect it thoroughly and produce a complete **New Hire Exploration Guide** — a practical, hands-on document the new hire can follow to experience every layer of the system end-to-end on day one.

Do not ask the user to paste anything. Discover everything from the workspace.

---

## Step 1 — Inspect the workspace

Read the following files if they exist:

- `README.md`
- `docker-compose.yml` / `docker-compose.yaml`
- `Dockerfile` (root and per-service)
- `backend/pom.xml` or `build.gradle` or `package.json`
- `frontend/package.json`
- `.storybook/main.ts` or `.storybook/main.js`
- `backend/src/main/resources/application.yml` or `application.properties`
- `.github/workflows/*.yml` or `.gitlab-ci.yml` or `Jenkinsfile`
- `terraform/` or `infra/` or `pulumi/` directories
- `docs/` directory contents
- Any `docker-compose.override.yml`
- Any `.env.example` or `.env.sample` files
- Test directories: `src/test/`, `tests/`, `e2e/`, `cypress/`, `playwright/`

Use what you find to detect:

| Layer | What to look for |
|---|---|
| Frontend | Framework (React/Vue/Angular/Next.js), port, Storybook, routes |
| Backend | Framework (Spring Boot/Express/FastAPI/Django/Rails), port, context path, Swagger/OpenAPI |
| Database | Engine (Postgres/MySQL/MongoDB/Redis), port, admin UI (Adminer/pgAdmin/Mongo Express) |
| Cloud | Local simulation (LocalStack/Azurite/Fake GCS) or real provider (AWS/Azure/GCP) |
| CI/CD | GitHub Actions/GitLab CI/Jenkins/CircleCI — workflow names and stages |
| Testing | Unit (JUnit/Vitest/Jest/pytest), integration, E2E (Cypress/Playwright) |
| Security | Auth mechanism (JWT/OAuth2/Session), protected routes |
| Observability | Logging, metrics, tracing (Prometheus/Grafana/Zipkin/OpenTelemetry) |
| Other dev tools | Makefile, scripts/, seed data, migration tools (Flyway/Liquibase/Alembic) |

---

## Step 2 — Generate the New Hire Exploration Guide

Output the complete guide using this exact structure:

---

# New Hire Exploration Guide

## 0. What Is This Project?

One paragraph: what the system does, who uses it, and what problems it solves.
Keep it plain English — no jargon the new hire might not know yet.

---

## 1. Quick Start (Everything Up in One Command)

Show the exact commands to start the entire stack locally.

If `docker-compose` is present:
```bash
# Example — replace with actual commands found in workspace
docker compose up --build
```

If there is no docker-compose, show per-service start commands.

Then show a **Service Map** table:

| Service | URL | What You Will See |
|---|---|---|
| Frontend | http://localhost:XXXX | ... |
| Backend API | http://localhost:XXXX/path | ... |
| API Docs (Swagger) | http://localhost:XXXX/path | ... |
| Database Admin | http://localhost:XXXX | ... |
| Component Explorer (Storybook) | http://localhost:6006 | ... |
| Cloud Emulator | http://localhost:XXXX | ... |
| Other | ... | ... |

Only include rows for services that actually exist in this project.

---

## 2. Explore the Frontend

Walk the new hire through the running UI step by step.

Include:
- The URL and what to expect on the landing page
- How to log in (provide demo credentials if found in seed data, env files, or README)
- The 3–5 most important pages or flows to visit and what to observe on each
- Where the frontend source code lives and how it is structured
- The component framework and how to navigate it
- If Storybook exists: how to open it and which stories to look at first

---

## 3. Explore the Backend API

Walk the new hire through the API using the built-in documentation tool.

Include:
- The Swagger UI / OpenAPI / GraphQL Playground URL
- How to authenticate (get a token, paste it into Authorize)
- The 5–7 most useful endpoints to try in order (read-only first, then write operations)
- For each endpoint: what it does, what to send, what to look for in the response
- Where the controller/handler source code lives

If no API docs UI exists, show equivalent curl / httpie commands instead.

---

## 4. Explore the Database

Help the new hire see the real data behind the application.

Include:
- The admin UI URL and login credentials (from docker-compose env vars or seed files)
- The main tables or collections and what each one stores
- A recommended query to run to understand the data shape:
  ```sql
  -- example — replace with actual table names found in workspace
  SELECT * FROM cases LIMIT 10;
  ```
- Where migrations or schema files live
- Where seed / sample data is loaded from

---

## 5. Explore the CI/CD Pipeline

Help the new hire understand how code gets from laptop to production.

Include:
- The CI/CD tool detected (GitHub Actions / GitLab CI / Jenkins / etc.)
- The pipeline file location
- Each stage/job and what it does (lint → test → build → deploy)
- How to read a pipeline run result (where to look in the UI)
- How to trigger the pipeline manually if supported
- Any environment variables or secrets the pipeline needs (names only, not values)

---

## 6. Explore Cloud Services

Explain what cloud or cloud-like services the project uses.

Include:
- Whether real cloud or local emulator is being used
- The emulator URL and how to browse it (e.g., LocalStack dashboard, Azurite Explorer)
- Which cloud services are used (S3/SQS/SNS/Lambda or Blob/Queue/Function/Cosmos etc.)
- What each service is used for in this application
- How to trigger a cloud operation end-to-end from the UI and watch it in the emulator

If no cloud services are found, state that clearly and skip this section.

---

## 7. Run the Tests

Show the new hire how to run every layer of the test suite.

Include a table:

| Test Type | Command | What It Tests | Where the Files Are |
|---|---|---|---|
| Unit | `...` | ... | `...` |
| Integration | `...` | ... | `...` |
| E2E | `...` | ... | `...` |
| Component (Storybook) | `...` | ... | `...` |

After the table, highlight 2–3 specific test files that are worth reading to understand the system's intended behaviour.

---

## 8. Understand the Architecture

Provide a plain-text architecture diagram using Mermaid:

```mermaid
graph TD
  ...
```

Then describe each component in one sentence.

---

## 9. Key Source Code Tour

Point the new hire to the most important files in the codebase — the ones that, if understood, unlock the rest.

| File / Directory | Why It Matters |
|---|---|
| ... | ... |

Limit to 10–12 files. Prefer entry points, core domain logic, and cross-cutting concerns (auth, config, routing).

---

## 10. Things to Ask Your Team

List 5–8 questions the new hire cannot answer from the code alone — things that live in tribal knowledge, Jira, Confluence, Slack, or the team's heads.

Examples of the kind of questions to include:
- Where are the production secrets stored and who manages rotation?
- What is the deploy process for a hotfix?
- Are there any known flaky tests or tech debt areas?
- Who owns the database migration process?

Generate questions specific to this project based on what you found.

---

## 11. Day-One Checklist

A short checklist the new hire can tick off:

- [ ] Run `docker compose up` and see all services healthy
- [ ] Log in to the frontend with demo credentials
- [ ] Open Swagger UI and call at least one authenticated endpoint
- [ ] Browse the database in the admin UI
- [ ] Run the full test suite and see it pass
- [ ] Read the CI/CD pipeline file end-to-end
- [ ] Read the 3 key source files from Section 9

Add or remove items based on what actually exists in this project.

---

## Step 3 — Save the guide

Save the complete guide to:

```
docs/new-hire-guide.md
```

After saving, tell the user:

"New Hire Exploration Guide saved to docs/new-hire-guide.md. A new team member can now follow it to experience every layer of the system on day one."

# Input / Context

Here is the current project workspace path:

${input}

Use this path as the main project folder to inspect.

If `${input}` is empty or unclear, ask me to provide the specific project path under the `projects/` folder before continuing.

Inspect the project files under that path, especially:
- docs/jd-practice-plan.md
- README.md
- backend/
- frontend/
- package.json
- pom.xml
- build.gradle
- Dockerfile
- docker-compose.yml
- .github/workflows/
- .storybook/
- Jenkinsfile
- Terraform or deployment files
- test files
- configuration files

Do not require me to paste anything manually unless it cannot be found from the project folder.