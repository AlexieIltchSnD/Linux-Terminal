#ifndef FIRSTPROJECT_CHARPROXY_H
#define FIRSTPROJECT_CHARPROXY_H

#include <ostream>

/**
 * CharProxy is a Proxy class for character-level access.
 * Used by File to support expressions like `file[i] = 'x';` or `char c = file[i];`.
 * **/
class File;    // Forward declaration to eliminate circular including.
class CharProxy {
    File* file;   //< Pointer to the owning FileValue.
    int index;    //< Index to read/write to.

public:
    // Constructor to initialize a CharProxy with its parent FileValue and target index.
    CharProxy(File* v, const int i) : file(v), index(i) {}

    // Implicit conversion operator, Allows read access like `char c = file[i];`.
    explicit operator char() const;

    // Assignment operator to write a character at the given index.
    CharProxy& operator=(char c);

    // Printing the proxy character. (Used in Read command at FileCommands.cpp)
    friend std::ostream& operator<<(std::ostream& os, const CharProxy& proxy) {
        os << static_cast<char>(proxy);
        return os;
    }
};

#endif //FIRSTPROJECT_CHARPROXY_H