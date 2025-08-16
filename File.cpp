#include <iostream>
#include <sstream>
#include "File.h"
#include "CommandGenerator.h"

File::File(const std::string& filename) : value(new FileValue(filename)), count(0), logicalName(filename){}

// Assignment operator, RCPtr handles the value.
File& File::operator=(const File& rhs) {
    if (this != &rhs) {
        value = rhs.value;
        count = rhs.count;
        logicalName = rhs.logicalName;
    }
    return *this;
}

// Function that returns only the actual file name.
std::string File::getFileName() const {
    return separatePath(logicalName,'!').back();
}

// Function that returns the full file name.
std::string File::getFullFileName() const {
    return logicalName;
}

// Read operator, opens the file, seeks the index you want to read from,
// returns the char that was read.
char File::operator[](const int i) const {
    if (i < 0 || i > static_cast<int>(count)) {
        throw IndexOutOfBounds("Index is out of bounds.");
    }
    value->stream->clear();
    value->stream->open(logicalName, std::ios::in);
    value->stream->seekg(i);
    char ch;
    value->stream->get(ch);
    value->stream->close();
    return ch;
}

// Write operator, here CharProxy handles the reading and writing,
// Since returning char& is not viable here, we need a proxy class to achieve the functionality
// We want, adds 1 into the character count inside the file if written above him by one.
CharProxy File::operator[](const int i) {
    if (i < 0 || i > static_cast<int>(count)) {
        throw IndexOutOfBounds("Index is out of bounds.");
    }
    if(i >= static_cast<int>(count)){
        count = i + 1;
    }
    return CharProxy(this, i);
}

// Function that updates the timestamps of a file, or creates a physical file it is not existed before.
void File::touch() const {
    value->stream->open(logicalName, std::ios::in | std::ios::out);
    if (!value->stream->is_open()) {
        value->stream->open(logicalName, std::ios::out);
    }
    value->stream->flush();
    value->stream->close();
}

// Function that copies the content of the current file, into a target file.
void File::copy(const File& target) const {
    value->stream->open(logicalName, std::ios::in);
    target.value->stream->open(target.getFullFileName(),std::ios::out);

    *target.value->stream << value->stream->rdbuf();

    const size_t target_size = value->stream->tellg();
    target.count = target_size;

    value->stream->flush();
    value->stream->close();
    target.value->stream->flush();
    target.value->stream->close();
}

// Function that removes the physical File from the disk.
void File::remove() const {
    if (value->stream->is_open()) {
        value->stream->close();
    }
    if (std::remove(logicalName.c_str()) != 0) {
        perror("Remove failed");
        throw FileSystemException("Failed to remove the file.");
    }
}

// Function that prints all the current file content.
void File::cat() const{
    value->stream->open(value->filename,std::ios::in);
    std::string line;
    while (std::getline(*value->stream, line)) {
        std::cout << line << std::endl;
    }
    value->stream->flush();
    value->stream->close();
}

// Function that prints the number of lines,words,and characters inside the current file.
void File::wc() const{
    value->stream->clear();
    value->stream->open(logicalName, std::ios::in);
    size_t lines = 0, words = 0, characters = 0;
    std::string word, line;
    while (std::getline(*value->stream, line)) {
        ++lines;
        characters += line.length();
        std::istringstream iss(line);
        while (iss >> word) {
            ++words;
        }
    }
    std::cout << "Lines: " << lines << ", Words: " << words << ", Characters: " << characters << '\n';
    value->stream->flush();
    value->stream->close();
}

// Function that creates a Hard-Link.
void File::ln(File& target) const {
    if (!target.hardLink) {
        target.value = value;
        target.value->filename = logicalName;
        target.count = count;
        target.hardLink = true;
    }
}

// Function that returns the reference counter of the FileValue attached to the smart pointer.
int File::getRefCounter() const {
    return value->getRefCount();
}