\page testing_policy Testing Policy
# Testing Policy

| | |
|---|---|
| **Status** | Active |
| **Audience** | All contributors (core team and external) |
| **Last updated** | May 2026 |

This document describes how we test NEXO Engine. It applies to every change that
lands on `main`, whether it comes from a core team member or an external
contributor. The goal is not to add bureaucracy: it is to keep the engine stable
across the platforms we ship on (Windows/MSVC and Linux/GCC) and to make
refactors safe.

> [!NOTE]
> NEXO is an open source project. If anything in this policy is unclear or feels
> wrong for the change you are working on, open an issue or raise it in your pull
> request rather than guessing.

## Table of Contents

- [Testing Policy](#testing-policy)
  - [Table of Contents](#table-of-contents)
  - [Goals](#goals)
  - [What this document does not cover](#what-this-document-does-not-cover)
  - [What we test](#what-we-test)
    - [Unit tests](#unit-tests)
    - [Integration tests](#integration-tests)
    - [Rendering tests](#rendering-tests)
    - [Editor / manual UI tests](#editor--manual-ui-tests)
  - [Where tests live](#where-tests-live)
  - [Naming and file conventions](#naming-and-file-conventions)
  - [Tooling](#tooling)
  - [Running the tests locally](#running-the-tests-locally)
  - [Coverage](#coverage)
  - [Continuous integration](#continuous-integration)
  - [Responsibilities](#responsibilities)
  - [Exemptions](#exemptions)
  - [Writing a test: a worked example](#writing-a-test-a-worked-example)
  - [Maintaining this document](#maintaining-this-document)

## Goals

- Keep public releases stable on every supported platform/compiler pair.
- Make refactors and engine evolution possible without silent regressions.
- Keep the test suite fast and deterministic so that running it locally stays cheap.
- Give reviewers something concrete to check beyond the diff itself.

## What this document does not cover

This document does not cover Accessibility specific QA and testing, which is covered in a separate policy (see [Accessibility Policy page](accessibility_policy.md)).

## What we test

NEXO is split between a native C++20 core (`engine/`, `common/`), an editor
(`editor/`) and a C# scripting layer (`engine/src/scripting/managed/`). The bulk
of the automated suite targets the C++ core with [GoogleTest](http://google.github.io/googletest/)
and GoogleMock.

### Unit tests

Required for any self-contained logic: ECS internals, math helpers, asset name
validation, exceptions, scene bookkeeping, etc.

- A unit test should construct what it needs and assert on observable behaviour,
  not on private implementation details.
- It must not depend on a real GPU context, the filesystem (beyond the small
  fixtures committed under `tests/`), or network access.
- It should be fast. If a single test takes a noticeable amount of time,
  something is usually wrong with the setup.

The ECS is the most heavily covered area today (see `tests/ecs/`) and is a good
reference for the level of detail we expect on core systems.

### Integration tests

Required when several subsystems have to cooperate: the scene manager driving the
coordinator, the asset importer pipeline (`tests/engine/assets/`), or the physics
system stepping a world (`tests/engine/physics/`). These live next to the unit
tests in the same executables; the distinction is about scope, not tooling.

Cover the nominal path **and** the failure paths (missing asset, invalid input,
empty scene, ...). A bug that only shows up on the error path is exactly the kind
of thing a unit test never catches.

### Rendering tests

The renderer is tested against a real OpenGL context (`tests/renderer/`). Because
CI runners are headless, these run on top of a software GL implementation:
`xvfb` on Linux and Mesa on Windows. Keep this in mind when adding renderer
tests — anything that assumes a specific GPU, driver extension or exact pixel
output will be flaky.

### Editor / manual UI tests

The editor is not covered by GoogleTest. Instead, `tests/editor/` holds short
behaviour checklists (`*.test` files) that describe what each window must do, for
example:

```
# Console Window
- Should see logs
- Should be able to filter the logs
- Should be able to write in it
- Log file should be generated when program closes
```

These are walked through manually (the in-engine `TestWindow` parses and displays
them). When you add or change an editor window, update the matching checklist so
it stays an accurate description of expected behaviour.

> [!NOTE]
> Editor sources are intentionally excluded from coverage metrics (see
> [Exemptions](#exemptions)); the checklists are the agreed-upon way to keep the
> UI honest.

## Where tests live

All automated tests live under `tests/`, mirroring the source layout. They are
grouped into four GoogleTest executables, each with a matching `ctest` label:

| Directory | Executable | `ctest` label | Covers |
|---|---|---|---|
| `tests/ecs/` | `ecs_tests` | `ecs` | Coordinator, component arrays, groups, systems |
| `tests/engine/` | `engine_tests` | `engine` | Events, scenes, components, assets, physics |
| `tests/renderer/` | `renderer_tests` | `renderer` | Buffers, framebuffers, shaders, pipeline, Renderer3D |
| `tests/common/` | `common_tests` | `common` | Math, paths, light helpers, exceptions |

Tests are only built when `NEXO_BUILD_TESTS` is `ON` (the default for the
`default` and `coverage` presets; off for `minimal`). When adding a new test
file, register it in the relevant `tests/<area>/CMakeLists.txt` — `add_executable`
does not pick files up automatically.

## Naming and file conventions

- **File name:** `<Subject>.test.cpp`, where `<Subject>` matches the class or unit
  under test — e.g. `Coordinator.test.cpp`, `Camera.test.cpp`, `Matrix.test.cpp`.
- **Header banner:** every test file starts with the standard NEXO banner block
  filled in with author, date and a one-line description, like every other source
  file in the repository. Copy it from an existing test.
- **Namespace:** tests live in the namespace of the code they exercise
  (`nexo::ecs`, `nexo::renderer`, ...).
- **Test macros:** use `TEST` for free-standing cases and `TEST_F` with a fixture
  when several cases share setup. Put shared setup/teardown in `SetUp()` /
  `TearDown()`.
- **Test names:** the suite name is the unit under test; the case name describes
  the behaviour, in English, e.g. `TEST_F(CoordinatorTest, DestroyNonexistentEntity)`.

## Tooling

| Purpose | Tool |
|---|---|
| Build system | CMake (≥ 3.28) + vcpkg manifest, Ninja |
| Test framework | GoogleTest + GoogleMock (`gtest` via vcpkg) |
| Test runner | `ctest` |
| Coverage | `--coverage` (GCC), collected with `gcovr` |
| Quality gate & coverage reporting | SonarCloud |
| Static / security analysis | CodeQL |

Test discovery is handled by CMake's `gtest_discover_tests`, so every `TEST` /
`TEST_F` becomes an individual `ctest` entry automatically.

## Running the tests locally

The simplest path is the dedicated workflow preset, which configures, builds and
runs the suite in one go:

```bash
cmake --workflow --preset=test-debug
```

For a coverage-instrumented run (GCC/Clang):

```bash
cmake --workflow --preset=test-coverage
```

If you already have a build directory, run `ctest` directly:

```bash
cd build
ctest -C Debug --output-on-failure
```

You can scope a run to one area with the labels above, or to a single case with a
regex:

```bash
ctest -C Debug -L ecs                 # only the ECS suite
ctest -C Debug -R CoordinatorTest     # only matching test names
```

> [!TIP]
> On Linux, renderer tests need a GL context. If you are on a headless machine,
> run them under `xvfb-run` exactly like CI does.

## Coverage

Coverage is measured on the engine core (`engine/` and `common/`) and reported to
SonarCloud. The editor, the tests themselves and the examples are excluded from
coverage on purpose (they are validated differently or are not product code).

**The minimum required coverage is 80%.** This is enforced through the SonarCloud
quality gate: a pull request that drops coverage below the threshold fails the
gate and is not merged. The coverage badge in the project
[README](../../README.md) reflects the current state. New code is therefore
expected to come with tests that keep coverage at or above 80%.

> [!CAUTION]
> 80% is a floor, not a goal. Coverage on its own proves only that a line ran,
> not that its behaviour was checked — a green gate with shallow assertions is
> worse than the same number with tests that actually exercise behaviour.

## Continuous integration

Every push runs the `Build, test and Package` workflow. It builds across a
platform matrix (currently `windows-latest`/MSVC and `ubuntu-22.04`/GCC&nbsp;13),
runs the full `ctest` suite headless, then packages and smoke-tests the
installers.

| CI step | Blocks merge? | Notes |
|---|---|---|
| Build (all matrix legs) | Yes | Must compile on every supported platform/compiler |
| `ctest` suite | Yes | Run headless via Mesa (Windows) / xvfb (Linux) |
| SonarCloud quality gate | Yes | Coverage must stay ≥ 80%, plus code smells on new code |
| CodeQL | Yes | Security analysis |
| Installer smoke tests | Yes | NSIS (Windows) and DEB (Linux) must install and launch |

Coverage is collected only on the Linux/GCC leg (`gcovr` → `coverage.xml` →
sonar-scanner); the other legs still build and run the tests.

## Responsibilities

**Contributors**

- Add tests for the logic you introduce or change, in the same PR.
- Make sure the whole suite passes locally before opening the PR.
- If you cannot reasonably test something, say so in the PR description — the
  pull request template has a dedicated section for this, including a
  "I need help with writing tests" option. Use it; that is a valid answer.

**Core team / maintainers**

- Keep the SonarCloud quality gate green and unblock it when it flags real gaps.
- Review the *tests* in a PR, not only the production code — coverage of the right
  behaviour is part of the review.
- Add or extend integration coverage when landing major features, and keep the
  editor checklists in sync with UI changes.
- Keep this policy in step with the actual tooling.

## Exemptions

The following are not expected to carry unit-test coverage and are excluded from
coverage metrics:

- The editor (`editor/`) — validated through the manual checklists in `tests/editor/`.
- The tests and examples themselves (`tests/`, `examples/`).
- Generated or third-party code vendored under `vcpkg/` and `external/`.
- Thin, logic-free glue (entry points such as `main`, trivial getters/setters).

Anything outside this list that you think should be exempt needs a maintainer's
sign-off in the PR.

## Writing a test: a worked example

A minimal GoogleTest fixture following the conventions above. The banner header
is abbreviated here for readability — copy the full one from an existing file.

```c++
//// Coordinator.test.hpp //////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//
//  Author:      Your Name
//  Date:        DD/MM/YYYY
//  Description: Test file for the coordinator
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ecs/Coordinator.hpp"

namespace nexo::ecs {

    struct Position { float x, y, z; };

    class CoordinatorTest : public ::testing::Test {
        protected:
            void SetUp() override {
                coordinator = std::make_unique<Coordinator>();
                coordinator->init();
                coordinator->registerComponent<Position>();
            }

            std::unique_ptr<Coordinator> coordinator;
    };

    TEST_F(CoordinatorTest, AddComponentSetsSignature) {
        const Entity entity = coordinator->createEntity();
        coordinator->addComponent(entity, Position{1.0f, 2.0f, 3.0f});

        EXPECT_TRUE(coordinator->entityHasComponent<Position>(entity));
        EXPECT_FLOAT_EQ(coordinator->getComponent<Position>(entity).x, 1.0f);
    }

    TEST_F(CoordinatorTest, DestroyNonexistentEntityDoesNotThrow) {
        EXPECT_NO_THROW(coordinator->destroyEntity(99999));
    }

}
```

Then register the file in the matching `tests/<area>/CMakeLists.txt` and it will
be discovered by `ctest` automatically.

## Maintaining this document

This policy tracks the real state of the build and CI. When the tooling changes
(new compiler in the matrix, a different coverage tool, an added test target),
update this page in the same change. For questions, open an issue.
