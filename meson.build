# File    : meson.build
# Purpose : Main meson build script for CheckpointTools
# Author  : Martin Rizzo | <martinrizzo@gmail.com>
# Date    : Nov 20, 2025
# Repo    : https://github.com/martin-rizzo/CheckpointTools
# License : MIT
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#                              CheckpointTools
#      CLI tools for inspecting and manipulating model checkpoint files
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
    'checkpointtools',                 # Project name
    'cpp',                             # Primary language
    default_options: [                 # Default options for the project
        'cpp_std=c++20',               # Use C++20 standard  (libtorch requires C++20)
        'warning_level=3',             # Warning level: 3 (many warnings)
        'werror=true',                 # Treat warnings as errors
        'buildtype=debug',             # Default build type: debug
    ],
    subproject_dir: 'dependencies',    # Directory to search for subprojects
    version: run_command('cat', 'VERSION').stdout().strip(),
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
tensorinfo_proj = subproject('TensorInfo')
tensorinfo_static_dep = tensorinfo_proj.get_variable('tensorinfo_static_dep')
tensorinfo_shared_dep = tensorinfo_proj.get_variable('tensorinfo_shared_dep')


#-- PROJECT VERSIONING -----------------------------------------------------#

# Get the project version 
project_version = meson.project_version()

# Prepare configuration data that will be used to generate the "common.h" header
cdata = configuration_data()
cdata.set('PROJECT_NAME', meson.project_name())
cdata.set('PROJECT_VERSION', project_version)

# Split the project version into major, minor, and patch
version_parts = project_version.split('.')
if version_parts.length() >= 3
    cdata.set('PROJECT_VERSION_MAJOR', version_parts[0])
    cdata.set('PROJECT_VERSION_MINOR', version_parts[1])
    cdata.set('PROJECT_VERSION_PATCH', version_parts[2])
endif

# Generate the header file "common.h" from a template
configure_file(
    input : 'src' / 'base' / 'common.h.in',
    output: 'common.h',
    configuration: cdata
)


##=========================================================================##
## //////////////////// Checkpoint Tools Executables ///////////////////// ##
##=========================================================================##

# Scan "common" source files and subdirectories
app_dirs    = [ ]
app_sources = [ ]
subdir( 'src' / 'base' )
base_dirs    = app_dirs
base_sources = app_sources

# Scan "ckshow" source files and subdirectories and build an executable
app_dirs    = [ ]
app_sources = [ ]
subdir( 'src' / 'ckshow' )
executable(
    'ckshow',                                  # Executable name
    base_sources + app_sources,                # Source files for compilation
    include_directories: base_dirs + app_dirs, # Include dirs for compilation
    dependencies: [ tensorinfo_static_dep ],   # Dependencies for the executable
    install     : true,                        # true = it should be installed when running 'meson install'
    install_dir : 'bin',                       # directory under prefix where to install the executable
)

#Scan "ckskeletonize" source files and subdirectories and build an executable
app_dirs    = [ ]
app_sources = [ ]
subdir( 'src' / 'ckskeletonize' )
executable(
    'ckskeletonize',                           # Executable name
    base_sources + app_sources,                # Source files for compilation
    include_directories: base_dirs + app_dirs, # Include dirs for compilation
    dependencies: [ tensorinfo_static_dep ],   # Dependencies for the executable
    install     : true,                        # true = it should be installed when running 'meson install'
    install_dir : 'bin',                       # directory under prefix where to install the executable
)
