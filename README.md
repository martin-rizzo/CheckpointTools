# TensorTools (test)

## Getting Started

To set up this project, begin by cloning the repository using the `--recurse-submodules` flag, ya que este proyect incorporates TensorInfoLib as a Git submodule.

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
cd TensorTools/dependencies/TensorInfoLib
git remote set-url origin git@github.com:martin-rizzo/TensorInfoLib.git
```

You can verify the change by running `git remote -v` within the `dependencies/your-library` directory.
