# C-PersonalProjectTemplate

Welcome to **C-PersonalProjectTemplate**, a modern, cross-platform C project template. This layout is designed to scale gracefully from lightweight command-line applications to modular, production-ready static, shared, or header-only libraries. It features completely automated dependency management via Vcpkg, traditional vendor-library auto-discovery, and a C++17-powered GoogleTest environment to thoroughly exercise your C codebase.

---

## 🚀 Key Features

* **Flexible Build Strategy:** Seamlessly toggle between building an **Executable**, a **Static Library**, a **Shared Library**, or a **Header-Only Library** via simple configuration flags.
* **Multi-Compiler Support:** Effortlessly switch between `gcc` and `clang` compiler toolchains directly via the `Makefile`.
* **Zero-Setup Package Management:** Automatically downloads, bootstraps, and links an isolated, project-local instance of **Vcpkg** running in **Manifest Mode**.
* **Automatic Subdirectory Vendor Scans:** Drop pre-built binaries or external source packages into the `libs/` folder for instant header and library linkage auto-discovery.
* **Isolated Testing Framework:** Separates test build paths out into a unique `_LIB` architecture so you can unit test your backend logic using **GoogleTest (C++17)** without conflicting with your application's operational `main.c`.
* **Out-of-Source Build Guardrail:** Hard blocks dirty in-source builds to maintain a pristine workspace layout.
* **Developer Pipeline Wrapper:** Includes a production-ready `Makefile` providing quick keyboard mappings for all compilation, deployment, and testing steps.

---

## 📁 Project Structure

```text
├── cmake/
│   ├── ExternalLib.cmake       # Scans and indexes the manual 'libs/' directory
│   ├── SourcesAndHeaders.cmake # Auto-collects source and test files
│   ├── StandardSettings.cmake  # Contains compiler configurations and options toggles
│   ├── Utils.cmake             # Custom developer macros and functions
│   └── Vcpkg.cmake             # Bootstraps local vcpkg instance inside build/
├── include/                    # Public API headers (.h)
├── libs/                       # Third-party manual binary drops (.a, .so, .dll, .lib)
├── src/                        # Implementation source files (.c)
│   └── main.c                  # Executable entry point (excluded from test targets)
├── test/
│   ├── CMakeLists.txt          # Discovers and structures granular test binaries
│   └── example_suite/          # Individual test contexts written in C++
│       └── test_main.cpp       # GoogleTest execution code
├── CMakeLists.txt              # Core orchestrated CMake script
├── CMakePresets.json           # Shared build and configuration presets for IDEs and CLI
├── CMakeUserPresets.json       # Local, machine-specific configuration overrides (e.g., local VCPKG_ROOT)
├── Makefile                    # Developer shortcut command dictionary
├── vcpkg.json                  # Manifest file declaring external project dependencies (e.g., gtest)
└── vcpkg-configuration.json    # Defines registry mappings and version baselines for vcpkg
```

---

## 🛠️ Configuration Settings

You can fully customize your compilation targets using cmake/StandardSettings.cmake file. These can be explicitly injected via command line interface arguments (`-D<OPTION>=<ON|OFF>`) or modified with a visual suite like `ccmake`.

| CMake Option | Default | Purpose |
| --- | --- | --- |
| `PersonalProject_BUILD_EXECUTABLE` | `ON` | Builds the project as a runnable application using `src/main.c`.|
| `PersonalProject_BUILD_HEADERS_ONLY` | `OFF` | Configures an `INTERFACE` target ignoring all source implementations.|
| `PersonalProject_BUILD_SHARED` | `OFF` | Compiles a shared binary file (`.so`/`.dll`/`.dylib`) with hidden symbol exposure defaults.|
| `PersonalProject_ENABLE_UNIT_TESTING` | `OFF` | Enables GoogleTest discovery layers and compiles tracking test frameworks.|
| `PersonalProject_VERBOSE_OUTPUT` | `ON` | Prints full internal source, header, and path discovery lists during generation.|
| `PersonalProject_WARNINGS_AS_ERRORS` | `OFF` | Enforces strict compiler logic checking by treating warnings as errors.|
| `ENABLE_VCPKG` | `ON` | Injects local Vcpkg system automation hooks. (note this is available in cmake/Vcpkg.cmake)|

---

## 💻 Working with the Makefile

The included `Makefile` automates setting target variables for common workflows.

### General Target Build Flows

* **Compile Local App (Debug Mode):**
```bash
make build
```


* **Compile Local App (Clean Optimization Release):**
```bash
make release
```


* **Build and Instantly Launch Executable:**
```bash
make brun
```



### Library Component Target Flows

* **Compile Static Library Target (`.a` / `.lib`):**
```bash
make lib_static
```


* **Compile Shared Library Target (`.so` / `.dll` / `.dylib`):**
```bash
make lib_shared
```


* **Expose Pure Interfaced Header Framework:**
```bash
make lib_header_only
```


### Workspace Cleanliness

* **Standard Soft Clean:** Wipes basic object definitions and intermediate build artifacts but safely bypasses your locally compiled Vcpkg binaries.


```bash
make clean
```


* **Total Workspace Hard Reset:** Purges the entire build environment directory structure including Vcpkg tools.


```bash
make clean_all
```

### Selecting a Compiler

* **Quick Test:** By default, the pipeline utilizes gcc. You can explicitly swap the underlying compiler toolchain by passing the COMPILER variable to any build or library target:


```bash
make build COMPILER=clang
make build COMPILER=gcc
```


* **Setting the default:** You can set the default compiler by modifying the `COMPILER` variable in the Makefile.


```Makefile
# Select Compiler gcc/clang
COMPILER ?= gcc
```

---

## 📦 Handling External Dependencies

This template provides two flexible paths for linking third-party dependencies:

### Method A: Local Manifest Management (Vcpkg)

This template uses isolated workspace integration. To add packages, simply create a `vcpkg.json` file in the root directory.

1. When CMake configures the environment, it clones the core package repository down to `build/vcpkg`.


2. It executes a bootstrapper routine to compile the native client manager.


3. It spins up Manifest Mode to parse your package request and map targets down directly into your compilation workspace.



*To pull updates down from upstream tracking trees, run:*

```bash
make vcpkg_update
```

### Method B: Manual Assembly Drops (`libs/`)

If you have offline, proprietary, or unmanaged external vendor dependencies, throw them inside the `libs/` folder matching this exact mapping:

```text
libs/
└── vendor_lib_name/
    ├── include/       # Put tracking public header interfaces here
    └── lib/           # Drop compiled libraries (.a, .so, .lib, etc.) here

```

The automated script inside `ExternalLib.cmake` recursively crawls these target paths, links discovered definitions into your executable environment, and updates public inclusion directories.

---

## 🧪 Executing Unit Tests

Unit tests are managed via C++17 wrappers built over GoogleTest. When test generation flags are enabled, the project builds an internal tracking library architecture (`PersonalProject_LIB`) matching your core source paths. This allows you to easily execute white-box tests directly on individual internal source components.

### Running Tests Via the Wrapper

* **Run via CTest Wrapper:**
```bash
make test_ctest
```


* **Run Executables Directly with Color Support:**
```bash
make test_direct
```


* **Target a Specific Isolated Test Folder Suite:**
```bash
make test_dir DIR=example_suite
```
