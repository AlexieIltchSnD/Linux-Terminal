#include "FileSystemException.h"
#include "CommandGenerator.h"
#include <functional>
#include <iostream>
#include <algorithm>
#include <string>
#include <map>

/**
 * Welcome to the File commandMap generator!
 * Here is where I activate all the File functions!
 * The main functionality of the File happens here.
 * We are transferred to here from the Terminal, when a File command is inserted before execution.
 * **/
std::map<std::string, CommandFunction> buildFileCommandsMap(Directory& root) {
    std::map<std::string, CommandFunction> fileCommandMap;

    /**
     *  Read command, check arguments given, and validate the path start from root.
     *  Find the file needed to be read from the file vector of the found Directory.
     *  Read the file with [] operator.
     *  Upon any error, throw CommandException, NotIndexException, LocationException, FileNotFoundException.
     ***/
    fileCommandMap["read"] = [&root](const std::vector<std::string>& parameters){
        if(parameters.size() != 2){
            throw CommandException("'read' requires 2 arguments.");
        }

        if(!std::all_of(parameters[1].begin(), parameters[1].end(), ::isdigit)){
            throw NotIndexException("Invalid index for reading.");
        }

        std::vector<std::string> path = separatePath(parameters[0]);
        if (path.empty() || path[0] != root.getDirectoryName()) {
            throw LocationException("Invalid path: must start from root.");
        }

        const std::vector<std::string> subPath(path.begin() + 1, path.end() - 1);
        Directory* current = root.depthSearch(subPath);

        const int fileIndex = current->isFileExists(path.back());
        if (fileIndex == -1) {
            throw FileNotFoundException("File does not exist in this path.");
        }

        File& file = current->getFileAt(fileIndex);
        const int index = std::stoi(parameters[1]);
        std::cout << file[index] << "\n";
    };

    /**
     * Write command, check arguments given, and validate the path start from root.
     * Find the file needed to be written to from the file vector of the found Directory.
     * Write into file with [] operator.
     * Upon any error, throw CommandException, NotIndexException, LocationException, FileNotFoundException.
     **/
    fileCommandMap["write"] = [&root](const std::vector<std::string>& parameters){
        if(parameters.size() != 3){
            throw CommandException("'write' requires 3 arguments.");
        }

        if(!std::all_of(parameters[1].begin(), parameters[1].end(), ::isdigit)){
            throw NotIndexException("Invalid index for writing.");
        }

        if(parameters[2].length() != 1){
            throw CommandException("'write' receives only 1 argument to write.");
        }

        std::vector<std::string> path = separatePath(parameters[0]);
        if (path.empty() || path[0] != root.getDirectoryName()) {
            throw LocationException("Invalid path: must start from root.");
        }

        const std::vector<std::string> subPath(path.begin() + 1, path.end() - 1);
        Directory* current = root.depthSearch(subPath);

        const int fileIndex = current->isFileExists(path.back());
        if (fileIndex == -1) {
            throw FileNotFoundException("File does not exist in this path.");
        }

        File& file = current->getFileAt(fileIndex);
        const int index = std::stoi(parameters[1]);
        file[index] = parameters[2][0];
    };

    /**
     *  Touch command, check if the Directory of the file we want to touch is valid.
     *  If it's valid, check if a file exits, if it exists only touch (Timestamp update), otherwise create a physical file and touch.
     *  Throws LocationException if the path doesn't start with the root 'V'.
     ***/
    fileCommandMap["touch"] = [&root](const std::vector<std::string>& parameters){
        std::vector<std::string> path = separatePath(parameters[0]);
        if (path.empty() || path[0] != root.getDirectoryName()) {
            throw LocationException("Invalid path: must start from root.");
        }

        const std::vector<std::string> subPath(path.begin() + 1, path.end() - 1);
        Directory* targetDir = root.depthSearch(subPath);
        int index = targetDir->isFileExists(path.back());
        if (index == -1) {
            index = targetDir->addFile("!"+path.back());
        }
        targetDir->getFileAt(index).touch();
    };

    /**
     *  Copy command, 4 different checks happen here:
     *  1) Physical file and a Physical file.
     *  2) Physical file and a Virtual file.
     *  3) Virtual file and a Physical file.
     *  4) Virtual file and a Virtual file.
     *  Target file may not exist upon copy usage.
     *  The function copies the content of the src file into target.
     *  Throw CommandException, FileNotFoundException, FileSystemException, DirectoryNotFoundException.
     * **/
    fileCommandMap["copy"] = [&root](const std::vector<std::string>& parameters){
        if (parameters.size() != 2) {
            throw CommandException("'copy' requires 2 arguments.");
        }

        std::vector<std::string> firstPath = separatePath(parameters[0]);
        std::vector<std::string> secondPath = separatePath(parameters[1]);

        auto isPhysical = [](const std::vector<std::string>& path) {
            return path.size() == 1;
        };
        auto isVirtual = [&root](const std::vector<std::string>& path) {
            return !path.empty() && path[0] == root.getDirectoryName();
        };

        const File* srcFile = nullptr;
        const File* dstFile = nullptr;
        Directory* parent = nullptr;
        File tempSrc, tempDst;

        if (isPhysical(firstPath)) {
            tempSrc = File(firstPath[0]);
            tempSrc.touch();
            srcFile = &tempSrc;
        } else if (isVirtual(firstPath)) {
            parent = root.depthSearch({firstPath.begin() + 1, firstPath.end() - 1});
            const int idx = parent->isFileExists(firstPath.back());
            if (idx == -1)
                throw FileNotFoundException("Source file does not exist in this path.");
            tempSrc = parent->getFileAt(idx);
            srcFile = &tempSrc;
        }

        if (isPhysical(secondPath)) {
            tempDst = File(secondPath[0]);
            tempDst.touch();
            dstFile = &tempDst;
        } else if (isVirtual(secondPath)) {
            parent = root.depthSearch({secondPath.begin() + 1, secondPath.end() - 1});
            int idx = parent->isFileExists(secondPath.back());
            if (idx == -1) {
                idx = parent->addFile("!" + secondPath.back());
                parent->getFileAt(idx).touch();
            }
            dstFile = &parent->getFileAt(idx);
        }

        if (!srcFile || !dstFile) {
            throw FileNotFoundException("Invalid source or destination.");
        }
        srcFile->copy(*dstFile);
    };

    // Remove command, check arguments, and use the last function -> cat_wc_remove.
    // Throw CommandException, LocationException, FileNotFoundException, DirectoryNotFoundException, FileSystemException.
    fileCommandMap["remove"] =[&fileCommandMap](const std::vector<std::string>& parameters){
        if(parameters.size() != 1){
            throw CommandException("'remove' requires 1 argument.");
        }
        fileCommandMap["cat_wc_remove"](parameters);
    };

    /**
     *  Move command, check arguments, and preform the copy function, then remove.
     *  Both are implemented above.
     *  Throw CommandException, LocationException, FileNotFoundException, DirectoryNotFoundException, FileSystemException.
     ***/
    fileCommandMap["move"] = [&root,&fileCommandMap](const std::vector<std::string>& parameters){
        if(parameters.size() != 2){
            throw CommandException("'move' requires 2 arguments.");
        }
        const std::vector<std::string> temp = separatePath(parameters[0]);
        if (temp[0] != root.getDirectoryName()) {
            fileCommandMap["copy"](parameters);     // File not in our system therefore cannot remove. (trusting user)
            return;
        }
        fileCommandMap["copy"](parameters);
        fileCommandMap["remove"](std::vector<std::string> {parameters[0]});
    };

    /**
     *  Cat command, check arguments, and look for a physical file to cat first.
     *  Otherwise, the file is virtual, use the last function cat_wc_remove
     *  Throw CommandException, LocationException, FileNotFoundException, DirectoryNotFoundException, FileSystemException.
     **/
    fileCommandMap["cat"] = [&root,&fileCommandMap](const std::vector<std::string>& parameters){
        if(parameters.size() != 1){
            throw CommandException("'cat' requires 1 argument.");
        }
        const std::vector<std::string> outsideFile = separatePath(parameters[0]);
        if (outsideFile[0] != root.getDirectoryName()) {
            const File temp(outsideFile[0]);
            temp.touch();
            temp.cat();
            return;
        }
        std::vector<std::string> s_parameters(parameters.begin(),parameters.end());
        s_parameters.emplace_back("cat");
        fileCommandMap["cat_wc_remove"](s_parameters);
    };

    /**
     *  Wc command, check arguments, and look for a physical file to wc first.
     *  Otherwise, the file is virtual, use the last function cat_wc_remove
     *  Throw CommandException, LocationException, FileNotFoundException, DirectoryNotFoundException, FileSystemException.
     **/
    fileCommandMap["wc"] = [&root,&fileCommandMap](const std::vector<std::string>& parameters){
        if(parameters.size() != 1){
            throw CommandException("'wc' requires 1 argument.");
        }

        const std::vector<std::string> outsideFile = separatePath(parameters[0]);
        if (outsideFile[0] != root.getDirectoryName()) {
            const File temp(outsideFile[0]);
            temp.touch();
            temp.wc();
            return;
        }

        std::vector<std::string> s_parameters(parameters.begin(),parameters.end());
        s_parameters.emplace_back("wc");
        fileCommandMap["cat_wc_remove"](s_parameters);
    };

    /**
     *  Ln command, creates a hard-link from the target file to the src file.
     *  Both files must be virtual, inside our system.
     *  Find the source file, (must be present), try to find the target file,(can be abscent).
     *  After source is found and target made / found, target will now point on source.
     *  Throw CommandException, LocationException, FileNotFoundException, DirectoryNotFoundException, FileSystemException.
     ***/
    fileCommandMap["ln"] = [&root,&fileCommandMap](const std::vector<std::string>& parameters){
        if (parameters.size() != 2) {
            throw CommandException("'ln' requires 2 arguments.");
        }

        std::vector<std::string> path1 = separatePath(parameters[0]);
        std::vector<std::string> path2 = separatePath(parameters[1]);

        if (path1.empty() || path1[0] != root.getDirectoryName() ||
            path2.empty() || path2[0] != root.getDirectoryName()) {
            throw LocationException("Invalid path: must start from root.");
        }

        const std::vector<std::string> parent1(path1.begin() + 1, path1.end() - 1);
        const std::vector<std::string> parent2(path2.begin() + 1, path2.end() - 1);

        Directory* source = root.depthSearch(parent1);
        Directory* target = root.depthSearch(parent2);

        const int src_index = source->isFileExists(path1.back());
        int trg_index = target->isFileExists(path2.back());

        if (src_index == -1) {
            throw FileNotFoundException("Source file does not exist.");
        }
        if (trg_index == -1) {
            const std::vector<std::string> temp(parameters.begin() + 1, parameters.end());
            fileCommandMap["touch"](temp);
            trg_index = target->isFileExists(path2.back());
        }
        source->getFileAt(src_index).ln(target->getFileAt(trg_index));
    };

    /**
     *  Cat or Wc or Remove function, since those 3 have identical exception checks, I merged them into one.
     *  Check the existence of files, and preform the right operation based on the last parameter of the vector given.
     *  Throw LocationException, FileNotFoundException, DirectoryNotFoundException, FileSystemException.
     ***/
    fileCommandMap["cat_wc_remove"] = [&root](const std::vector<std::string>& parameters){
        std::vector<std::string> path = separatePath(parameters[0]);
        if (path.empty() || path[0] != root.getDirectoryName()) {
            throw LocationException("Invalid path: must start from root.");
        }

        const std::vector<std::string> parent(path.begin() + 1, path.end() - 1);
        Directory* current = root.depthSearch(parent);
        const int target = current->isFileExists(path.back());
        if(target == -1){
            throw FileNotFoundException("File does not exist.");
        }

        if(parameters.back() == "cat") current->getFileAt(target).cat();
        else if(parameters.back() == "wc") current->getFileAt(target).wc();
        else{
            current->getFileAt(target).remove();
            current->removeFileAt(target);
        }
    };

    return fileCommandMap;
}