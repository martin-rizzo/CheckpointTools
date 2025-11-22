# TensorTools (test)

## Getting Started

To set up this project, begin by cloning the repository using the `--recurse-submodules` flag, ya que este proyect incorporates TensorInfo library as a Git submodule.

```bash
git clone --recurse-submodules https://github.com/martin-rizzo/TensorTools.git
```

To keep your project up-to-date, first pull the latest changes. Then, update all submodules to match the new references.

```bash
git pull
git submodule update --init --recursive
```

### For Internal Contributors

Internal contributors intending to push changes to the project and submodules may configure its local remote to use SSH for authenticated operations. This change is local to your development environment and will not affect the project's public submodule configuration.

First, ensure you are in the root directory of your cloned `TensorTools` project. Then, navigate into a submodule directory and update its `origin` remote URL:

```bash
cd TensorTools/dependencies/TensorInfo
git remote set-url origin git@github.com:martin-rizzo/TensorInfo.git
```

You can verify the change by running `git remote -v` within the `dependencies/your-library` directory.

## Requirements

This project requires a modern C++ compiler with mature support for the C++20 standard library:

* **GCC (GNU C++ Compiler):**
    * **Version 12 or higher:** Ensure you are using `g++` version 12 or later. These versions provide robust and complete support for the C++20 `<format>` header and other C++20 standard library features.

* **Clang (LLVM C++ Compiler):**
    * **Version 15 or higher:** Clang, particularly when paired with `libc++` (its standard library implementation), generally offers comprehensive C++20 support, including `<format>`, from version 15 onwards.

* **Microsoft Visual C++ (MSVC):**
    * **Visual Studio 2022 (v17.0) or higher:** MSVC's implementation of `<format>` is available in Visual Studio 2022. Ensure you have the latest updates installed for the best compatibility.


