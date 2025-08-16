#ifndef FIRSTPROJECT_COMMANDGENERATOR_H
#define FIRSTPROJECT_COMMANDGENERATOR_H

#include "Directory.h"
#include <functional>
#include <string>
#include <map>

// Alias for the actual Function, to reduce line space.
using CommandFunction = std::function<void(const std::vector<std::string>&)>;

// Create a map of Directory commands and the functions that correspond to each command.
// Receives the root directory for searching, also the working directory for chdir.
// (mkdir, rmdir, chdir, etc.)
std::map<std::string, CommandFunction> buildDirectoryCommandsMap(Directory& root, Directory* workingDirectory);

// Create a map of File commands and the functions that correspond to each command.
// Receives the root directory for searching,
// (cat, touch, write, etc.)
std::map<std::string, CommandFunction> buildFileCommandsMap(Directory& root);

// Function that separates a path by a delimiter returns the separated path as a vector.
std::vector<std::string> separatePath(const std::string& path, char delimiter = '/');

#endif //FIRSTPROJECT_COMMANDGENERATOR_H