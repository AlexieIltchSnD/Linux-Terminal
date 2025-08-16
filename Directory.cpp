#include "Directory.h"
#include <iostream>
#include <algorithm>
#include "FileSystemException.h"

// Function that adds a new File into the vector and returns his index.
int Directory::addFile(const std::string &filename) {
    const std::string path = getFullPath() + filename;
    files.emplace_back(path);
    return static_cast<int>(files.size()) - 1;
}

// Function that validates that the path exists from the root, until the last directory.
// Check if the last part of path exists, if not create a new Directory,
// else throw DirectoryAlreadyExistsException.
void Directory::mkdir(const std::vector<std::string>& path) {
    if (path.empty() || path[0] != directoryName) {
        throw LocationException("Invalid path: must start from root.");
    }
    if (path.size() == 1) {
        throw DirectoryAlreadyExistsException("Root directory cannot be created.");
    }
    const std::vector<std::string> parentPath(path.begin() + 1,path.end() - 1);
    Directory* current = depthSearch(parentPath);

    const std::string& targetDirectory = path.back();
    for (auto& sub : current->subDirectories) {         // Check if the last of the path exists.
        if (sub.directoryName == targetDirectory) {
            throw DirectoryAlreadyExistsException("Directory already exists at targetDirectory location.");
        }
    }

    current->subDirectories.emplace_back(targetDirectory, current);
}

// Function to change the current working-directory, check if the path exists with
// depthSearch, and return a pointer if found, or throw DirectoryNotFoundException.
Directory* Directory::chdir(const std::vector<std::string>& path) {
    if (path.empty() || path[0] != directoryName) {
        throw LocationException("Invalid path: must start from root.");
    }
    const std::vector<std::string> subPath(path.begin() + 1, path.end());

    if (subPath.empty()) {
        return this;
    }

    return depthSearch(subPath);
}

// Function to remove a directory, finds the directory to remove, if found, it gets removed
// from the directory vector via the parent directory, if the working-directory is removed,
// it gets transferred to its parent.
Directory* Directory::rmdir(const std::vector<std::string>& path, const Directory* workingDirectory) {
    if (path.empty() || path[0] != directoryName) {
        throw LocationException("Invalid path: must start from root.");
    }

    const std::vector<std::string> parentPath(path.begin() + 1, path.end() - 1);
    Directory *current = depthSearch(parentPath);         // Find the parent directory of toBeRemoved.
    const std::string& target = path.back();

    const Directory* toBeRemoved = nullptr;
    for (auto &sub : current->subDirectories) {     // Find the directory to remove.
        if (sub.directoryName == target) {
            toBeRemoved = &sub;
            break;
        }
    }

    if (!toBeRemoved) {
        throw DirectoryNotFoundException("Target directory not found.");
    }

    // Validate the removal.
    const auto it = std::find_if(current->subDirectories.begin(),current->subDirectories.end(), [&](const Directory& d) {
        return d.directoryName == target;
    });

    if (it == current->subDirectories.end()) {
        throw DirectoryNotFoundException("Directory not found at target location.");
    }
    clearFiles(*it);
    current->subDirectories.erase(it);
    if (workingDirectory == toBeRemoved) {
        return current;
    }
    return this;
}

// Function that prints the content of the folder given a path.
// If a function is called from lproot, also print the reference count for all files.
void Directory::ls(const std::string& path, const std::string& lp_root){
    std::cout << path + "/:\n";
    int i = 1;
    for(const auto& directory : subDirectories){      // Print all directory names.
        if( i % tab_amount == 0)
            std::cout << "\n";
        std::cout << "\t" << directory.directoryName << "\t";
        i++;
    }
    for(int j = 0; j < static_cast<int>(files.size()); j++){               // Print all File names.
        if( i % tab_amount == 0)
            std::cout << "\n";
        if(lp_root == "HL"){
            std::cout << "\t" << getFileAt(j).getFileName() << " " << getFileAt(j).getRefCounter() << "\t";
        }else {
            std::cout << "\t" << getFileAt(j).getFileName() << "\t";
        }
        i++;
    }
    if (i % tab_amount != 1) std::cout << "\n";
}

// Function that prints all the file names, with directory names, across the whole
// File System recursively.
void Directory::lproot(const std::string& path) {
    const std::string Path = path + directoryName;
    ls(Path,"HL");
    for (auto& sub : subDirectories) {
        sub.lproot(path + directoryName + "/");
    }
}

// Function that prints the working-directory via going backwards in the directory tree with parent pointer.
// Reverse the vector to get the accurate path, and print to user.
void Directory::pwd() const {
    std::vector<std::string> pathInParts;
    const Directory* current = this;

    while(current != nullptr){
        pathInParts.push_back(current->directoryName);
        current = current->parent;
    }

    std::reverse(pathInParts.begin(),pathInParts.end());

    std::string fullPwd;
    for(const auto& part : pathInParts){
        fullPwd += part + "/";
    }
    std::cout << fullPwd << "\n";
}

// Function that traverses the directory tree of vectors, if the whole path given was found,
// return the pointer to the last one found, otherwise throws DirectoryNotFoundException.
Directory* Directory::depthSearch(const std::vector<std::string> &path) {
    Directory* current = this;
    for (const auto &part: path) {
        bool target = false;

        for (auto &sub: current->subDirectories) {
            if (sub.directoryName == part) {
                current = &sub;
                target = true;
                break;
            }
        }
        if (!target) {
            throw DirectoryNotFoundException("Invalid path: Directory was not found.");
        }
    }
    return current;
}

// Function that returns the whole path of any given directory recursively.
std::string Directory::getFullPath() const {
    if (parent == nullptr) return directoryName;
    return parent->getFullPath() + "!" + directoryName;
}

// Function that returns the current directory name.
const std::string& Directory::getDirectoryName() const {
    return directoryName;
}

// Function that returns an index of a File from the File vector via name.
int Directory::isFileExists(const std::string& filename) const{
    const std::string fullPath = getFullPath() + "!" + filename;
    for(size_t i = 0 ;i < files.size(); i++){
        if(files[i].getFullFileName() == fullPath) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// Function that returns a File from the File vector via index.
File& Directory::getFileAt(const int index) {
    if (index >= 0 && index < static_cast<int>(files.size())) {
        return files.at(index);
    }
    throw FileSystemException("Invalid file index.");
}

// Function that removes a File from the File vector via index.
void Directory::removeFileAt(const int index) {
    if (index >= 0 && index < static_cast<int>(files.size())) {
        files.erase(files.begin() + index);
        return;
    }
    throw FileSystemException("Invalid file index.");
}

// Function that removes all the physical files created by the user recursively.
void Directory::clearFiles(Directory &directory) {
    for(File& file : directory.files) {
        file.remove();
    }
    for(auto& sub: directory.subDirectories){
        clearFiles(sub);
    }
}