#ifndef FIRSTPROJECT_DIRECTORY_H
#define FIRSTPROJECT_DIRECTORY_H
#include <vector>
#include <string>
#include "File.h"

/**
 *  Directory class.
 *  Simulates a virtual folder and file system tree via vectors.
 *  Each layer of Directories has its own vector of subdirectories and file vector just like in any operating system.
 *  Holds and executes each of the Directory functions (mkdir,chdir,rmdir,ls,lproot,pwd)
 *  alongside other helper functions.

 The big 3:
    Not implemented, since the default is enough.
    1) I never make copies of Directory, only raw pointer assignments (Copy constructor isn't needed)
    2) I never assign one Directory to another, only raw pointer assignments. (Assignment operator isn't needed)
    3) I never delete a Directory, since each object is not allocated on the heap, only on stack. (Deconstruction isn't needed).
    All default operations are enough here.
 * **/
constexpr int tab_amount = 4;               // Used for printing.
class Directory {
    std::string directoryName;              //< Each directory has its own name.
    Directory* parent;                      //< Each directory holds a pointer to his parent.
    std::vector<Directory> subDirectories;  //< Each directory holds a vector of subdirectories.
    std::vector<File> files;                //< Each directory holds a vector of files.

public:
    // Creates a new Directory constructor.
    explicit Directory(std::string name, Directory* parent = nullptr): directoryName(std::move(name)), parent(parent) {};
    int addFile(const std::string& filename);                     // Adds a new File into the File vector.
    void mkdir(const std::vector<std::string>& path);             // Adds a new Directory to an existing one by given path.
    Directory* chdir(const std::vector<std::string>& path);       // Change the working-directory by given path.
    Directory* rmdir(const std::vector<std::string>& path, const Directory* workingDirectory); // Removes a directory by given path, change working-directory if needed.
    void ls(const std::string& path, const std::string& lp_root = "");                      // Prints the contents of a given path.
    void lproot(const std::string& path);                         // Prints all the directories and files inside the system.
    void pwd() const;                                             // Prints the working-directory path.

    std::string getFullPath() const;                              // Returns the full path of a Directory.
    const std::string& getDirectoryName() const;                  // Returns the Directory name.
    int isFileExists(const std::string& filename) const;          // Returns the index of a File inside the vector of Files, -1 otherwise.
    Directory* depthSearch(const std::vector<std::string>& path); // Returns the Directory at a given path.
    File& getFileAt(int index);                                   // Returns an address of a file inside the File vector.
    void removeFileAt(int index);                                 // Removes a file from File vector.

    // Recursively removes all physical files in the directory tree.
    // (used from Terminal.cpp on 'exit' command, on root directory)
    void clearFiles(Directory& directory);
};

#endif //FIRSTPROJECT_DIRECTORY_H