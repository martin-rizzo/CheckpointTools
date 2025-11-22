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
#include "ckshow_args.h"
#include "messages.h"


static int
to_integer(std::string_view str, int defaultValue = 0) {
    int result;
    auto end = str.data() + str.length();
    auto resultInfo = std::from_chars(str.data(), end, result);
    return resultInfo.ec == std::errc{} ? result : defaultValue;
}

static std::string_view
to_string(bool value) {
    return value ? "true" : "false";
}

//============================= CONSTRUCTION ==============================//

/**
 * Constructs a new CkShowArgs object by parsing command line arguments.
 *
 * This function processes the command line arguments provided to the program,
 * distinguishing between flag-style options (prefixed with '-') and positional
 * arguments. Each argument is managed according to predefined rules.
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

    for( int i=1 ; i < argc ; ++i ) {
        std::string_view arg{ argv[i] };
        std::string_view nextarg{ (i+1) < argc && argv[i+1][0] != '-' ? argv[i+1] : "" };

        // parse the flags arguments
        if( arg.starts_with('-') ) {
        //-OPTIONS:
            if     ( arg=="-n"  || arg=="--name"     ) { name = nextarg; ++i; }
            else if( arg=="-m"  || arg=="--metadata" ) { command = Command::LIST_METADATA; }
            else if(               arg=="--thumbnail") { command = Command::EXTRACT_THUMBNAIL; }
            else if( arg=="-p"  || arg=="--prefix"   ) { prefix = nextarg; ++i; }
            else if( arg=="-d"  || arg=="--depth"    ) { depth  = to_integer(nextarg); ++i; }
        //-FORMATS:
            else if( arg=="-u"  || arg=="--human"    ) { format = Format::HUMAN; }
            else if( arg=="-b"  || arg=="--basic"    ) { format = Format::PLAIN; }
            else if( arg=="-j"  || arg=="--json"     ) { format = Format::JSON; }
        //-EXTRA:
            else if( arg=="--nc"|| arg=="--no-color" ) { use_color = UseColor::NEVER; }
            else if( arg=="-h"  || arg=="--help"     ) { help = true; }
            else if( arg=="-v"  || arg=="--version"  ) { version = true; }
            else {
                Messages::fatal_error( std::format("Unknown argument: {}", arg), {
                    "Try `ckshow --help` for more information."
                });
            }
        }
        // parse the positional arguments (not starting with '-')
        else {
            if( filename.empty() ) { filename = arg;  }
            else {
                Messages::fatal_error("Too many files specified.", {
                    "You can only specify one file."
                });
            }
        }
    }
}

/**
 * Overloads the insertion operator (<<) for printing CkShowArgs objects to an output stream.
 *
 * This function provides a way to print the contents of an Args object in a 
 * human-readable format. It outputs each member variable with its name and
 * value to the specified output stream, facilitating debugging or logging.
 *
 * @param os   The output stream where the Args data will be printed.
 * @param args The Args object being printed to the stream.
 * @return A reference `os` for chaining.
 */
std::ostream&
operator<<(std::ostream& os, const CkShowArgs& args) {
    os << "Args:"                                        << std::endl;
    os << "  command: "     << to_string(args.command)   << std::endl;
    os << "  filename: "    << args.filename             << std::endl;
    os << "  name: "        << args.name                 << std::endl;
    os << "  prefix: "      << args.prefix               << std::endl;
    os << "  depth: "       << args.depth                << std::endl;
    os << "  format: "      << to_string(args.format)    << std::endl;
    os << "  use_color: "   << to_string(args.use_color) << std::endl;
    os << "  Help: "        << to_string(args.help)      << std::endl;
    return os;
}
