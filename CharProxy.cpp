#include "CharProxy.h"
#include "File.h"

// Opens the file in input mode, seeks to the target index, reads a single character,
// and then closes the stream, and returns that char read.
CharProxy::operator char() const {
    file->value->stream->clear();
    file->value->stream->open(file->value->filename, std::ios::in);
    file->value->stream->seekg(index);
    char ch = 0;
    file->value->stream->get(ch);
    file->value->stream->close();
    return ch;
}

// Opens the file in both input and output mode to allow writing without truncating.
// Writes the character at the specified index, flushes changes, and closes the stream.
CharProxy& CharProxy::operator=(const char c){
    file->value->stream->clear();
    file->value->stream->open(file->value->filename, std::ios::in | std::ios::out);
    file->value->stream->seekp(index);
    file->value->stream->put(c);
    file->value->stream->flush();
    file->value->stream->close();
    return *this;
}