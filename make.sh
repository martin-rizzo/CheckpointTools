#!/usr/bin/env bash
# File    : make.sh
# Purpose : Script to illustrate how to compile the "CheckpointTools" project
# Author  : Martin Rizzo | <martinrizzo@gmail.com>
# Date    : Nov 20, 2025
# Repo    : https://github.com/martin-rizzo/CheckpointTools
# License : MIT
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#                              CheckpointTools
#      CLI tools for inspecting and manipulating model checkpoint files
#
#     Copyright (c) 2025 Martin Rizzo
#
#     Permission is hereby granted, free of charge, to any person obtaining
#     a copy of this software and associated documentation files (the
#     "Software"), to deal in the Software without restriction, including
#     without limitation the rights to use, copy, modify, merge, publish,
#     distribute, sublicense, and/or sell copies of the Software, and to
#     permit persons to whom the Software is furnished to do so, subject to
#     the following conditions:
#
#     The above copyright notice and this permission notice shall be
#     included in all copies or substantial portions of the Software.
#
#     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
#     EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
#     MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
#     IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
#     CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
#     TORT OR OTHERWISE, ARISING FROM,OUT OF OR IN CONNECTION WITH THE
#     SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
SCRIPT_NAME=$(basename "${BASH_SOURCE[0]}" .sh)         # script name without extension
SCRIPT_DIR=$(realpath "$(dirname "${BASH_SOURCE[0]}")") # script directory
PROJECT_DIR=$SCRIPT_DIR             # project directory
PROJECT_NAME="CheckpointTools"      # project name (used in packages)
VERSION_FILE="VERSION"              # file containing project version number
RELEASE_BRANCH="master"             # the branch used for releases (tags)
RELEASE_DIR="/tmp/$PROJECT_NAME"    # local release directory
HELP="
Usage: ./$SCRIPT_NAME.sh [COMMAND] [OPTIONS]

  Allows you to compile and manage the 'CheckpointTools' project in Linux environments.

  Available commands:
    build        Build the project (default)
    rebuild      Rebuild the project from scratch
    release      Build a release version locally in '${RELEASE_DIR}'
    deploy       Build, tag, and publish a new release version on GitHub
    clean        Clean the build directory

  Available options:
    -l, --error-limit [N]   Maximum number of errors to show (default 30 if N is empty)
    -n, --no-color          Disable color output
    -h, --help              Show this help message and exit.

  Examples: 
    $SCRIPT_NAME.sh clean
    $SCRIPT_NAME.sh --no-color build
"



# ANSI escape codes for colored terminal output
RED='\e[91m'; YELLOW='\e[93m'; GREEN='\e[92m'; CYAN='\e[96m'; RESET='\e[0m'
disable_color() { RED=''; YELLOW=''; GREEN=''; CYAN=''; RESET=''; }

#============================= ERROR MESSAGES ==============================#

# Display help message
help() { echo "${HELP}"; }

# Display a warning message
warning() { echo -e "\n${CYAN}[${YELLOW}WARNING${CYAN}]${RESET} $1" >&2; }

# Display an error message
error() { echo -e "\n${CYAN}[${RED}ERROR${CYAN}]${RESET} $1" >&2; }

# Displays a fatal error message and exits the script
fatal_error() {
    error "$1"; shift
    while [[ $# -gt 0 ]]; do
        echo -e " ${CYAN}\xF0\x9F\x9B\x88 $1${RESET}" >&2
        shift
    done
    echo; exit 1
}

#================================= HELPERS =================================#

# Removes the release directory if it exists
#
# Usage:
#   remove_release_dir
#
# Parameters:
#   The function uses the variable `RELEASE_DIR` which should contain the
#   path to the directory intended for removal.
#
# Notes:
#   - This function assumes that the variable `RELEASE_DIR` is set.
#   - Using this function on directories outside "/tmp" will result in no action,
#     preventing accidental deletions from unintended locations.
#
remove_release_dir() {
    # if the directory does not exist, do nothing
    if [[ ! -d "${RELEASE_DIR}" ]]; then
        return 0;
    fi
    # if the directory is not located within "/tmp", better not delete it
    if [[ "${RELEASE_DIR}" != "/tmp"* ]]; then
        echo "Release directory will not be deleted because it is not in /tmp"
        return 0
    fi
    rm -r "${RELEASE_DIR}"
    echo "Release directory deleted."
}

# Print the current project version number
#
# Usage:
#   get_project_version
#
# Parameters:
#   This function uses two global variables:
#    - PROJECT_DIR: The full path to the root directory of the project.
#    - VERSION_FILE: The name of the file containing the version number.
#
get_project_version() {
    if [[ ! -f "$PROJECT_DIR/$VERSION_FILE" ]]; then
        fatal_error "File '$VERSION_FILE' not found in the root of the project."
    fi
    cat "$PROJECT_DIR/$VERSION_FILE" | tr -d '\n\r '
}

#================================ COMMANDS =================================#

# Builds the CheckpointTools project using the Meson build system.
#
# Usage:
#   build_project [ERROR_LIMIT]
#
# Parameters:
#   [ERROR_LIMIT]: An optional parameter that specifies the maximum number of
#                  error messages to display. If not provided, all errors will
#                  be shown during compilation.
#
# Requirements:
#   - Meson and Ninja build systems must be installed on the system.
#
build_project() {
    local error_limit=$1  #< (optional) must be empty or a positive integer

    # make sure we are at the root of the project directory
    cd "$PROJECT_DIR" || fatal_error "Failed to change directory."

    # this is a small hack to show only the first compilation errors up to 'error_limit'
    # it uses `grep` to find error messages and then `head` to limit the number of lines shown
    if [[ "${error_limit}" ]]; then
        meson setup      builddir
        meson compile -C builddir 2>&1 | grep -E "error:|fatal error:" --color="always" | head -n "${error_limit}"
        return 0
    fi
    

    # this is the recommended way to compile the project
    meson setup      builddir
    meson compile -C builddir

}

# Builds the release version of CheckpointTools project using Meson.
#
# Usage:
#   release_project
#
# Description:
#   This function automates the process of setting up, compiling, and installing 
#   the project in release mode. It configures the build environment with specified
#   parameters to ensure that the output is optimized for release. The resulting
#   binaries and other files are installed into the "${RELEASE_DIR}" directory.
#
# Requirements:
#   - Meson and Ninja build systems must be installed on the system.
#   - The `PROJECT_DIR` variable should point to the root of the project directory.
#   - The `RELEASE_DIR` variable should specify where the release files will be installed.
#
build_release() {
    local release_args=(
        --buildtype release
        -Ddebug=false
        -Doptimization=2
        -Dstrip=true
        -Dwerror=true
    )
    meson setup      builddir --prefix "${RELEASE_DIR}" "${release_args[@]}" || fatal_error "Meson setup failed."
    meson compile -C builddir                                                || fatal_error "Meson compile failed."
    meson install -C builddir                                                || fatal_error "Meson install failed."
    echo "Release version built successfully."
    echo "Release files are located in ${RELEASE_DIR}"
}

# Cleans the project by removing build and release directories.
#
clean_project() {
    # make sure we are at the root of the project directory
    cd "$PROJECT_DIR"   || { fatal_error "Failed to change directory."; }

    # remove the build directory
    if [[ -d 'builddir' ]]
    then
        rm -rf 'builddir'
        echo "Build directory cleaned."
    fi

    # remove the release directory ($RELEASE_DIR)
    remove_release_dir
}

# Creates a Git tag for the project based on its version.
#
# Usage:
#   create_git_tag <VERSION>
#
# Parameters:
#   <VERSION>: The version number used to create the Git tag. It should follow
#              semantic versioning "MAJOR.MINOR.PATCH" format. Example: "1.0.0".
#
# Requirements:
#   - Git must be installed and properly configured with access to the project's repository.
#   - The 'RELEASE_BRANCH' variable should point to the branch used for releases (e.g., "master").
#
create_git_tag() {
    local version=$1
    local tag_name="v$version"
    local tag_message=$(printf "%s %s\n\nAutomatic tag for release %s" "$PROJECT_NAME" "$tag_name" "$version")
    local current_branch

    echo "Checking if current branch is master/main..."
    current_branch=$(git rev-parse --abbrev-ref HEAD)

    if [[ "$current_branch" != "$RELEASE_BRANCH" ]]; then
        fatal_error "You are not on the '$RELEASE_BRANCH' branch." \
                    "Please switch to '$RELEASE_BRANCH' branch before creating a release."
    fi

    git rev-parse --verify --quiet "$tag_name" \
        && fatal_error "The tag '$tag_name' already exists" \
                       "Maybe you need to increase the version in 'VERSION' file."

    echo "Creating Git tag: '$tag_name'..."
    git tag -a "$tag_name" -m "$tag_message" \
        || fatal_error "Failed to create Git tag."
    echo "Git tag '$tag_name' created locally."

    echo "Pushing the tag '$tag_name' to remote repository..."
    git push origin "$tag_name" \
        || fatal_error "Failed to push the tag '$tag_name' to remote repository."
    echo "Git tag '$tag_name' pushed successfully."
}

# #===========================================================================#
# #////////////////////////////////// MAIN ///////////////////////////////////#
# #===========================================================================#
main() {

    local no_color=false     # no color mode flag
    local show_help=false    # show help flag
    local show_version=false # show version flag
    local command="build"    # default command to execute
    local error_limit=''     # no error limit
    local version=$(get_project_version)

    while [[ $# -gt 0 ]]; do
        if [[ $1 == -* ]]; then
            case $1 in
                -l|--error-limit)
                    if [[ "$2" =~ ^[0-9]+$ ]]; then
                        error_limit="$2"; shift
                    else
                        error_limit=30 #< default error limit if -l without argument
                    fi
                ;;
                -n|--nc|--no-color) no_color=true     ;;
                -h|--help)          show_help=true    ;;
                -v|--version)       show_version=true ;;
                *)
                    fatal_error "Invalid option: \"$1\"" "Use --help for usage." ;;
            esac
        else
            command=$1;
        fi
        shift
    done

    # disable color output if --no-color option is provided
    if [[ $no_color == true ]]; then disable_color; fi

    # display help message and exit if --help option is provided
    if [[ $show_help == true ]]; then help; exit 0; fi

    # display project version and exit
    if [[ $show_version == true ]]; then echo "$version"; exit 0; fi

    # execute the specified command
    case $command in
        build|all)  build_project "$error_limit" ; exit 0 ;;
        rebuild)    clean_project; build_project ; exit 0 ;;
        release)    build_release                ; exit 0 ;;
        deploy)     create_git_tag "$version"    ; exit 0 ;;
        clean)      clean_project                ; exit 0 ;;
        library)    fatal_error "Not implemented yet." ;;
        tools)      fatal_error "Not implemented yet." ;;
        *)          fatal_error "Invalid command: \"$command\"" "Use --help for usage." ;;
    esac

}
main "$@"
