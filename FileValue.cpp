#include <fstream>
#include "FileValue.h"

void FileValue::init() {
    stream = new std::fstream();
}

// Assignment Operator, copy the filename, and copy the pointer of other.
// RCPtr manages reference counting.
FileValue& FileValue::operator=(const FileValue &other){
    if (this != &other) {
        filename = other.filename;
        stream = other.stream;
    }
    return *this;
}

// Straightforward destructure.
FileValue::~FileValue() {
    if (stream) {
        if (stream->is_open())
            stream->close();
        delete stream;
    }
}