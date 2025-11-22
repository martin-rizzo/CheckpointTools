/*
| File    : ckshow.cpp
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
#include "ckshow.h"
#ifdef _WIN32
    inline bool is_terminal_output() { return true; }
#else
#include <unistd.h> // for "::isatty()" and STDOUT_FILENO
    inline bool is_terminal_output() { return ::isatty(STDOUT_FILENO) != 0; }
#endif

using namespace tin;


//============================= CONSTRUCTION ==============================//

CkShow::CkShow(const CkShowArgs& args)
: _args(args)
{}

//================================ HELPERS ================================//

void
CkShow::print_help() const noexcept {
    std::cout << _args.help_message  << std::endl;
}

void
CkShow::print_version() const noexcept {
    std::cout << "ckshow (CheckpointTools ckshow) " << PROJECT_VERSION << std::endl;
}

void
CkShow::fatal_read_error(ReadError readError) {
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
CkShow::list_tensors_columns(const TensorMap& tensorMap) const {
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
CkShow::list_tensors_csv(const TensorMap& tensorMap, bool includeHeader /* = true */) const {
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
CkShow::list_metadata(const TensorMap& tensorMap) const {
    std::cout << "Metadata:" << std::endl;
    for(auto& it : tensorMap.metadata()) {
        std::cout << "  " << it.first << ": " << it.second.as_string() << std::endl;
    }
}

void
CkShow::print_metadata(const TensorMap& tensorMap, StringView key) const {
    std::cout << tensorMap.metadata().get(key).as_string() << std::endl;
}

//================================ RUNNING ================================//

int
CkShow::run() {
    ReadError readError;

    // determine if we should use color output
    bool use_color = _args.use_color == UseColor::ALWAYS ||
                     (_args.use_color == UseColor::AUTO && is_terminal_output());
    if( !use_color ) {
        Colors::instance().disable_colors();
    }

    // if help was requested, show the help message and exit
    if( _args.help ) { print_help(); return 0; }

    // if version was requested, show the version and exit
    if( _args.version ) { print_version(); return 0; }

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
