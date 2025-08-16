#include <sstream>
#include <iostream>
#include "Terminal.h"
#include "CommandGenerator.h"

// Simulates a terminal, reads commands from user and executes them.
// Command maps are ['command': lambda function], for more information, go to CommandGenerator.h
void Terminal::startTerminal() {
    auto DirectoryCommands = buildDirectoryCommandsMap(root, workingDirectory);
    auto FileCommands = buildFileCommandsMap(root);

    std::string inputString;
    while(true) {
        std::getline(std::cin, inputString);

        if(inputString == "exit") {
            clearFS();  // User exit command clears the physical files created if needed.
            break;
        }
        std::stringstream stream(inputString);
        std::string command;
        stream >> command;

        std::vector<std::string> parameters;
        std::string parameter;
        while (stream >> parameter) parameters.push_back(parameter);

        auto iterator = DirectoryCommands.find(command);
        try{
            if(iterator != DirectoryCommands.end()){        // Check if the command is for Directories, or files.
                if (parameter.empty() || inputString[inputString.length()-1] == '/')
                    iterator->second(parameters);
                else
                    throw CommandException("Invalid path: last character has to be a slash.");
            }
            else {
                iterator = FileCommands.find(command);
                if (iterator != FileCommands.end()) {
                    iterator->second(parameters);
                } else {
                    std::cout << "Unknown command: " << command << "\n";
                }
            }
        }catch(std::exception& e){                          // Throw a unique exception for each case encounter.
            std::cerr << "ERROR: " << e.what() << "\n";
        }
    }
}

void Terminal::clearFS() {
    root.clearFiles(root);
}