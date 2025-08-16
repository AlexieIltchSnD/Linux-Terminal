// Terminal.h simulates a simple terminal; written commands are executed, on any error, a unique exception is thrown.
// Includes Directory class, which is the root directory the terminal has access to.

#ifndef FIRSTPROJECT_TERMINAL_H
#define FIRSTPROJECT_TERMINAL_H

#include <utility>
#include "Directory.h"

/**
 * Represents a Terminal, Supports all the commands in the exercise.
 * Holds the root directory, and workingDirectory for the command 'pwd'.
 * */
class Terminal {
    Directory root;                 // < Root Directory.
    Directory* workingDirectory;    // < Used for chdir.

public:
    // Explicit constructor.
    explicit Terminal(std::string mRoot): root(std::move(mRoot), nullptr), workingDirectory(&root) {}

    // Starts the mini terminal.
    void startTerminal();

    // deletes all the physical files created on the user's disk.
    void clearFS();
};

#endif //FIRSTPROJECT_TERMINAL_H