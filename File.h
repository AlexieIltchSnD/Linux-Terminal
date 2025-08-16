#ifndef FIRSTPROJECT_FILE_H
#define FIRSTPROJECT_FILE_H

#include "RCPtr.h"
#include "FileValue.h"
#include "CharProxy.h"

/**
 *  File class
 *  This class is a wrapper of FileValue, it is used to extend fstream,
 *  Holds and executes each of the File functions (touch, copy, remove, move, cat, wc, ln)
 *  alongside other helper functions.
 *  (since 'move' uses copy and remove, It's not here).
 * **/
class File {
    friend class CharProxy;
    RCPtr<FileValue> value;     //< Smart pointer to a FileValue.
    mutable size_t count;       //< Character count on each File.
    std::string logicalName;    //< Full file name. (Example: V!tt!gg!test.txt)
    bool hardLink = false;      //< File that was hard-linked cannot be hard-linked AGAIN.

public:
    File():value(nullptr),count(0){}
    explicit File(const std::string& filename);
    File(const File& other) = default;      // Default copy constructor.
    char operator[](int i) const;           // Read operator.
    CharProxy operator[](int i);            // Write operator.
    File& operator=(const File& rhs);       // Assignment operator.
    std::string getFileName() const;        // Returns the current file name.
    std::string getFullFileName() const;    // Returns the full name of a file.
    int getRefCounter() const;              // Return the reference count of a file.

    void touch() const;                     // Creates a physical file, or refreshes timestamp of an existing file.
    void copy(const File& target) const;    // Copies the content of this File, into another target.
    void remove() const;                    // Removes a physical file from the system.
    void cat() const;                       // Prints the content of this File.
    void wc() const;                        // Prints word/lines/characters of this File.
    void ln(File& target) const;            // Creates a hard-link.
};

#endif //FIRSTPROJECT_FILE_H