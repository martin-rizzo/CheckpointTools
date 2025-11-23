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

//================================ HELPERS ================================//

static int
to_integer(StringView str, int defaultValue = 0) {
    int result;
    auto end = str.data() + str.length();
    auto resultInfo = std::from_chars(str.data(), end, result);
    return resultInfo.ec == std::errc{} ? result : defaultValue;
}

/**
 * Parses an option from the command-line arguments.
 * 
 * This function parses an option from the command-line arguments. It handles
 * the different formats in which an option can be specified, such as:
 * - `--option=value` (the code must call `get_value`)
 * - `--option value` (the code must call `get_value`)
 * - `--option`
 *
 * @param i    The index of the argument to parse as an option.
 * @param argc The total number of arguments passed in the command line.
 * @param argv The array of argument strings passed in the command line.
 * @return
 *     A pair containing the parsed option and its associated value, if any.
 */
static StringViewPair
parseOption(int i, int argc, char* argv[]) {

    // by default, assume the associated value is the next argument
    StringView curr{ argv[i] };
    StringView value{ (i+1) < argc && argv[i+1][0] != '-' ? argv[i+1] : "" };

    // if the option starts with "--" and contains an "=",
    // extract the value (including the "=")
    if( curr.starts_with("--") ) {
        if( auto it = curr.find('='); it != StringView::npos) {
            value = curr.substr(it);
            curr  = curr.substr(0, it);
        }
    }
    // if the option does not start with "--", assume no associated value
    else { value = ""; }
    return StringViewPair{ curr, value };
}

/**
 * Returns the value associated with an option, handling embedded or separate values.
 *
 * This function processes the value part of an option pair. If the value
 * was embedded in the option (e.g., --option=value), it extracts the value
 * portion after the equal sign. If the value was provided as a separate argument,
 * it advances the argument index and returns the next argument as the value.
 *
 * @param option A reference to the option pair containing the option and its value.
 * @param i      A reference to the argument index, which will be advanced if needed.
 * @return
 *    The extracted value associated with the option.
 */
static StringView
get_value(StringViewPair& option, int& i) {
    StringView value = option.second;

    // if the value starts with "=", extract the portion after "="
    if( value.starts_with('=') ) { value = value.substr(1); }
    // if no leading '=', assume value was provided as a separate argument and advance 'i'
    else { i++; }

    // clear the option's second element to indicate the value has been processed
    option.second = "";
    return value;
}

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
        auto option = parseOption(i, argc, argv);
        auto arg    = option.first;

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

