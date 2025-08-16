#ifndef FIRSTPROJECT_FILEVALUE_H
#define FIRSTPROJECT_FILEVALUE_H

#include <fstream>
#include <utility>
#include "FileSystemException.h"
#include "RCObject.h"

/**
 * FileValue class acts as a shared file object. Used with
 * RCPtr<FileValue> in the File wrapper class (File.h).
 * FileValue contains a heap allocated fstream, and a file name.

The big 3:
    1) Copy constructors - are for sharing or copying, like touch and ln. (Can be default)
    2) Copy assignment   - when assigning one FileValue to another.
    3) Destructor        - to clean up your heap-allocated fstream.
 */
class FileValue: public RCObject{
    void init();

public:
    explicit FileValue(std::string  name):filename(std::move(name)){ stream = new std::fstream(); };
    FileValue(const FileValue& other): RCObject(other), filename(other.filename), stream(other.stream) {}
    FileValue& operator=(const FileValue& other);
	~FileValue() override;

    std::string filename;   //< Actual file name opened.
    std::fstream* stream;   //< File stream used for file operations (allocated on heap as required).
};

#endif //FIRSTPROJECT_FILEVALUE_H