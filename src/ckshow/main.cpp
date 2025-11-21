/*
| File    : main.cpp
| Purpose : Main entry point for the `ckshow` command tool.
| Author  : Martin Rizzo | <martinrizzo@gmail.com>
| Date    : Nov 20, 2025
| Repo    : https://github.com/martin-rizzo/CheckpointTools
| License : MIT
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
|                              CheckpointTools
|      CLI tools for inspecting and manipulating model checkpoint files
\_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
#include "ckshow_args.h"
#include "ckshow_command.h"

//=========================================================================//
// //////////////////////////////// MAIN ///////////////////////////////// //
//=========================================================================//

int main(int argc, char* argv[]) {
    CkShowArgs    args{argc, argv};
    CkShowCommand command{args};
    return command.run();
}
