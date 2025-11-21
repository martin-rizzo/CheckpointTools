/*
| File    : ckshow_command.cpp
| Purpose : The `ckshow` command line tool.
| Author  : Martin Rizzo | <martinrizzo@gmail.com>
| Date    : Nov 20, 2025
| Repo    : https://github.com/martin-rizzo/CheckpointTools
| License : MIT
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
|                              CheckpointTools
|      CLI tools for inspecting and manipulating model checkpoint files
\_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
#include <tin/tensormap.h>
#include "colors.h"
#include "messages.h"
#include "ckshow_command.h"
static const char HELP[]=R"(
Usage: ckshow [OPTIONS] file

  Allows you to compile and manage the OpenDiffusion project in Linux.

  OPTIONS:
    -n, --name <NAME>      Show the value of a tensor (or metadata) with the given key. e.g. 'model.layer.1.bias'
    -m, --metadata         Print metadata information related to the checkpoint file
    -p, --prefix <PREFIX>  Filter the tensor names by a prefix to display only matching tensors
    -d, --depth <DEPTH>    Specify the depth level of the hierarchical index to display
    --thumbnail            Extract the thumbnail from the .safetensors file and save it as a .jpg image

  Output formats:
    -u, --human            Output in a human-readable format with clear formatting (default)
    -b, --basic            Output in a plain, easily parseable format for scripts or tools
    -j, --json             Output data in JSON format when available

    --nc, --no-color       Disable color output.
    -h  , --help           Show this help message and exit.

  Examples:
    ckshow --prefix model.layer.1.bias 'checkpoint.safetensors'
    ckshow --no-color 'checkpoint.safetensors'
)";

#ifdef _WIN32
    inline bool is_terminal_output() { return true; }
#else
#include <unistd.h> // for "::isatty()" and STDOUT_FILENO
    inline bool is_terminal_output() { return ::isatty(STDOUT_FILENO) != 0; }
#endif

using namespace tin;


//============================= CONSTRUCTION ==============================//

CkShowCommand::CkShowCommand(const CkShowArgs& args)
: _args(args)
{}

//================================ HELPERS ================================//

void
CkShowCommand::print_help() {
    std::cout << HELP << std::endl;
}

void
CkShowCommand::fatal_read_error(ReadError readError) {
    switch(readError) {
        case ReadError::FileNotFound:
            Messages::fatal_error("File not found.");
            break;

        case ReadError::InvalidFormat:
            Messages::fatal_error("This is probably not a valid .safetensors or .gguf file.");
            break;

        case ReadError::UnsupportedVersion:
            Messages::fatal_error("The file may be from an older or newer version of the format that this tool does not support.");
            break;

        case ReadError::HeaderTooLarge:
            Messages::fatal_error("The file header may be corrupted, incomplete, or have other issues that prevent it from being read correctly.");
            break;

        case ReadError::MemoryAllocationFailed:
            Messages::fatal_error("There may not be enough memory available to read this file, or it is corrupted in a way that prevents allocation of enough memory.");
            break;

        case ReadError::MissingData:
            Messages::fatal_error("The file is missing some required data, which may indicate corruption or have other issues that prevent it from being read correctly.");
            break;
            
        default:
            Messages::fatal_error("An unknown error occurred while reading the file.");
    }
}

//============================== SUBCOMMANDS ==============================//

void
CkShowCommand::list_tensors_columns(const TensorMap& tensorMap) const {
    auto sortedTensors = tensorMap.collect_tensors(SortBy::NAME_AND_INDEX);

    size_t nameMaxLen = 0, shapeMaxLen = 0;
    for(const auto& tensor : sortedTensors) {
        auto shapeString = tensor.shape().to_string("[]", ",");
        nameMaxLen = std::max(nameMaxLen, tensor.name().length());
        shapeMaxLen = std::max(shapeMaxLen, shapeString.length());
    }
    
    for(const auto& tensor : sortedTensors) {
        auto shapeString = tensor.shape().to_string("[]", ",");
        std::cout << std::format("{:<{}}   {:<{}}  {}\n",
            tensor.name(), nameMaxLen,
            shapeString, shapeMaxLen,
            tensor.dtype());
    }
}

void
CkShowCommand::list_tensors_csv(const TensorMap& tensorMap, bool includeHeader /* = true */) const {
    auto sortedTensors = tensorMap.collect_tensors(SortBy::NAME_AND_INDEX);
    if(includeHeader) {
        std::cout << "name,shape,dtype" << std::endl;
    }
    for(const auto& tensor : sortedTensors) {
        auto tensor_shape = tensor.shape().to_string("", "x");
        std::cout << tensor.name() << ", " << tensor_shape << ", " << tensor.dtype() << std::endl;
    }
}

void
CkShowCommand::list_metadata(const TensorMap& tensorMap) const {
    std::cout << "Metadata:" << std::endl;
    for(auto& it : tensorMap.metadata()) {
        std::cout << "  " << it.first << ": " << it.second.as_string() << std::endl;
    }
}

void
CkShowCommand::print_metadata(const TensorMap& tensorMap, const std::string& key) const {
    std::cout << tensorMap.metadata().get(key).as_string() << std::endl;
}

//================================ RUNNING ================================//

int
CkShowCommand::run() {
    ReadError readError;

    // determine if we should use color output
    bool use_color = _args.use_color == UseColor::ALWAYS ||
                     (_args.use_color == UseColor::AUTO && is_terminal_output());
    if(!use_color) {
        Colors::instance().disable_colors();
    }

    // if help was requested, show the help message and exit
    if(_args.help) { print_help(); return 0; }

    // if the user didn't provide any file, show an error message and exit
    if(_args.filename.empty()) {
        Messages::fatal_error("No file provided. Please specify a .safetensors or .gguf file.", {
            "To get help on how to use this tool, run: ckshow --help"
        });
    }

    // load the checkpoint file
    auto tensorMap = TensorMap::from_file(_args.filename, readError);
    if(readError != ReadError::None) { fatal_read_error(readError); }

    std::cout << std::endl;
    std::cout << _args << std::endl;
    std::cout << std::endl;

    if(_args.command == Command::LIST_METADATA) {
        if(!_args.name.empty()) { print_metadata(tensorMap, _args.name); }
        else                    { list_metadata(tensorMap); }
    } else {
        // print the names of all tensors in the file
        list_tensors_csv(tensorMap);
    }

    return 0;
}
