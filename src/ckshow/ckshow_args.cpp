/*
| File    : ckshow_args.h
| Purpose : The arguments of the `ckshow` command line
| Author  : Martin Rizzo | <martinrizzo@gmail.com>
| Date    : Nov 20, 2025
| Repo    : https://github.com/martin-rizzo/CheckpointTools
| License : MIT
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
|                              CheckpointTools
|      CLI tools for inspecting and manipulating model checkpoint files
\_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
#include <format>   // for std::format
#include <string>   // for std::string
#include <charconv> // for std::from_chars
#include "common.h"
#include "ckshow_args.h"
#include "messages.h"


//============================= CONSTRUCTION ==============================//

/**
 * Constructs a new CkShowArgs object by parsing command line arguments.
 *
 * This function processes the command line arguments provided to the program,
 * distinguishing between flag-style options (prefixed with '-'), flag-style
 * options with values (prefixed with '--' and containing an '='), and
 * positional arguments.
 *
 * @param argc The number of command line arguments passed to the program.
 * @param argv An array of C strings representing the command line arguments.
 */
CkShowArgs::CkShowArgs(int argc, char* argv[])
: help_message{R"(
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
    -v  , --version        Show version information and exit.

  Examples:
    ckshow --prefix model.layer.1.bias 'checkpoint.safetensors'
    ckshow --no-color 'checkpoint.safetensors'
)"}
{
    for( int i=1 ; i < argc ; ++i )
    {
        auto option = parse_option(i, argc, argv);
        auto arg    = get_name(option,i);

        // parse the flags arguments
        if( arg.starts_with('-') ) {
        //-OPTIONS:
            if     ( arg=="-n"  || arg=="--name"     ) { name    = get_value(option,i); }
            else if( arg=="-m"  || arg=="--metadata" ) { command = Command::LIST_METADATA; }
            else if(               arg=="--thumbnail") { command = Command::EXTRACT_THUMBNAIL; }
            else if( arg=="-p"  || arg=="--prefix"   ) { prefix  = get_value(option,i); }
            else if( arg=="-d"  || arg=="--depth"    ) { depth   = to_integer(get_value(option,i)); }
        //-FORMATS:
            else if( arg=="-u"  || arg=="--human"    ) { format  = Format::HUMAN; }
            else if( arg=="-b"  || arg=="--basic"    ) { format  = Format::PLAIN; }
            else if( arg=="-j"  || arg=="--json"     ) { format  = Format::JSON; }
        //-EXTRA:
            else if( arg=="--nc"|| arg=="--no-color" ) { when_color = "never"; }
            else if(               arg=="--color"    ) { when_color = get_value(option,i); }
            else if( arg=="-h"  || arg=="--help"     ) { help = true; }
            else if( arg=="-v"  || arg=="--version"  ) { version = true; }
            else {
                // if an unknown argument is encountered, display a fatal error message
                Messages::fatal_error( "Unknown argument: " + String{arg}, {
                    "Try `ckshow --help` for more information." });
            }
            // check if the option's value starts with '=' (not consumed),
            // indicating user-provided value not expected by the option
            if( option.second.starts_with('=') ) {
                Messages::fatal_error( "The argument '"+ String{arg}+"' no expects a value and '"+String{option.second}+"' was provided.", {
                    "Try `ckshow --help` for more information." });
            }
        }
        // handle positional arguments, arguments without a preceding hyphen
        // (assume the positional argument is the filename)
        else {
            if( filename.empty() ) { filename = arg;  }
            else {
                Messages::fatal_error("Too many files specified.", {
                    "You can only specify one file." });
            }
        }
    }

    // salir del programa abrutamente
    std::cout << (*this) << std::endl;
    std::exit( 0 );
}

