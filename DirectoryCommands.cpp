#include "CommandGenerator.h"
#include "FileSystemException.h"
#include "Terminal.h"
#include <map>
#include <string>
#include <functional>

/**
 * Welcome to the Directory commandMap generator!
 * Here is where I activate all the Directory functions!
 * The main functionality of the Directories happens here.
 * We are transferred to here from the Terminal, when a Directory command is inserted before execution.
 * **/
std::map<std::string, CommandFunction> buildDirectoryCommandsMap(Directory& root, Directory* workingDirectory){
    std::map<std::string, CommandFunction> directoryCommandMap;

    // mkdir command, checks number of arguments given, and activate mkdir from root.
    directoryCommandMap["mkdir"] = [&root](const std::vector<std::string>& parameters){
        if(parameters.size() != 1) {
            throw CommandException("'mkdir' requires only 1 argument.");
        }
        const std::vector<std::string> path = separatePath(parameters[0]);
        root.mkdir(path);
    };

    // chdir command, checks number of arguments given, and activate chdir from root.
    directoryCommandMap["chdir"] = [&root, &workingDirectory](const std::vector<std::string>& parameters){
        if (parameters.size() != 1) {
            throw CommandException("'chdir' requires only 1 argument.");
        }

        const std::vector<std::string> path = separatePath(parameters[0]);
        workingDirectory = root.chdir(path);
    };

    // rmdir command, checks number of arguments given, and if the path starts from root.
    // then activate remove from root, and change working-directory if needed.
    directoryCommandMap["rmdir"] = [&root, &workingDirectory](const std::vector<std::string>& parameters) {
        if (parameters.size() != 1) {
            throw CommandException("'rmdir' requires only 1 argument.");
        }

        const std::vector<std::string> path = separatePath(parameters[0]);
        if(path.size() == 1 && path[0] == "V"){
            throw FileSystemException("Cannot delete root directory.");
        }

        workingDirectory = root.rmdir(path,workingDirectory);
    };

    // Ls command, check the number of arguments given, and if the path starts from root.
    // Activate ls on the Directory returned from depthSearch.
    directoryCommandMap["ls"] = [&root, &workingDirectory](const std::vector<std::string>& parameters){
        if (parameters.empty()) {
            workingDirectory->ls(workingDirectory->getDirectoryName());
            return;
        }
        std::vector<std::string> path = separatePath(parameters[0]);
        if (path[0] != root.getDirectoryName()) {
            throw LocationException("Invalid path: must start from root.");
        }

        if(path.size() == 1){
            root.ls("V");
        }else {
            const std::vector<std::string> subPath(path.begin() + 1, path.end());
            Directory *current = root.depthSearch(subPath);
            current->ls(current->getDirectoryName());
        }
    };

    // lproot command, check the number of arguments given, and activate lproot from root.
    directoryCommandMap["lproot"] = [&root](const std::vector<std::string>& parameters){
        if (!parameters.empty()) {
            throw CommandException("'lproot' requires 0 arguments.");
        }
        root.lproot("");
    };

    // pwd command, check the number of arguments given, and activate pwd.
    directoryCommandMap["pwd"] = [&workingDirectory](const std::vector<std::string>& parameters){
        if(!parameters.empty()) {
            throw CommandException("'pwd' takes no arguments.");
        }
        workingDirectory->pwd();
    };

    return directoryCommandMap;
}