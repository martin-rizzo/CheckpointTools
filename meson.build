# File    : meson.build
# Purpose : Main meson build script for CheckpointTools
# Author  : Martin Rizzo | <martinrizzo@gmail.com>
# Date    : Nov 20, 2025
# Repo    : https://github.com/martin-rizzo/CheckpointTools
# License : MIT
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#
#  Each subdirectory in the project should have its own `meson.build` file
#  to add to app_dirs and app_sources its directory and source files.
#
#  Meson function reference: https://mesonbuild.com/Reference-manual_functions.html
#  Meson built-in options  : https://mesonbuild.com/Builtin-options.html#
#  Meson for VS Code       : https://github.com/mesonbuild/vscode-meson
#
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

project(
    'checkpointtools',               # Project name
    'cpp',                         # Primary language
    version: '0.0.1',              # Project version
    default_options: [             # Default options for the project
        'cpp_std=c++20',           # Use C++20 standard  (libtorch requires C++20)
        'warning_level=3',         # Warning level: 3 (many warnings)
        'werror=true',             # Treat warnings as errors
        'buildtype=debug',         # Default build type: debug
    ],
    subproject_dir: 'dependencies',    # Directory to search for subprojects
)
cpp = meson.get_compiler('cpp')    # Get the C++ compiler object
subproject_dir  = 'dependencies'   # Directory where subprojects are located


#-- COMPILER OPTIONS -------------------------------------------------------#

# Suppress warnings about unused parameters
add_project_arguments(['-Wno-unused-parameter'], language : 'cpp')

# Suppress warnings about dangling references
# add_project_arguments(['-Wno-dangling-reference'], language : 'cpp')


#-- DEPENDENCIES -----------------------------------------------------------#
# (subprojects are defined in the `dependencies` directory)

# TensorInfo: https://github.com/martin-rizzo/TensorInfo
# TODO: get the current TensorInfo subproject as dependency
#tensoinfo_dep = subproject('TensorInfo').get_variable('tensorinfo_dep')



##=========================================================================##
## //////////////////// Checkpoint Tools Executables ///////////////////// ##
##=========================================================================##

# Scan source directory for C++ files and subdirectories.
app_dirs    = [ ]
app_sources = [ ]
subdir( 'src' / 'ckview' )

# Define Checkpoint Tools executables and their dependencies.
executable(
    'ckview',
    app_sources,
    include_directories: app_dirs,
    #dependencies      : [ tensorinfo_dep ],
)
