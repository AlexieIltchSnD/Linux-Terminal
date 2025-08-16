#ifndef FIRSTPROJECT_FILESYSTEMEXCEPTION_H
#define FIRSTPROJECT_FILESYSTEMEXCEPTION_H

#include <exception>
#include <string>
#include <utility>

/**
 * Welcome to FileSystemExceptions.h! Contains all custom exception classes used in the file system project.
 * **/

// Base exception class for file system errors.
class FileSystemException : public std::exception{
    std::string message;
public:
    explicit FileSystemException(std::string  message) : message(std::move(message)) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// Thrown when a File cound not be found.
class FileNotFoundException final : public FileSystemException {
public:
    explicit FileNotFoundException(const std::string& msg) : FileSystemException(msg) {}
};

// Thrown when a file is not shareable.
class FileNotShareableException final : public FileSystemException {
public:
    explicit FileNotShareableException(const std::string& msg) : FileSystemException(msg) {}
};

// Thrown when a user tries to read a file or write into one when the index is bigger then file size.
class IndexOutOfBounds final : public FileSystemException {
public:
    explicit IndexOutOfBounds(const std::string& msg) : FileSystemException(msg) {}
};

// Thrown when command is used incorrectly or with invalid arguments.
class CommandException final : public FileSystemException {
public:
    explicit CommandException(const std::string& msg) : FileSystemException(msg) {}
};

// Thrown when the index of read/write is not a number.
class NotIndexException final : public FileSystemException {
public:
    explicit NotIndexException(const std::string& msg) : FileSystemException(msg) {}
};

// Thrown when the directory the user wanted to create, already exists in the FileSystem.
class DirectoryAlreadyExistsException final : public FileSystemException {
public:
    explicit DirectoryAlreadyExistsException(const std::string& msg) : FileSystemException(msg) {}
};

// Thrown when the directory wasn't found in the FileSystem.
class DirectoryNotFoundException final : public FileSystemException {
public:
    explicit DirectoryNotFoundException(const std::string& msg) : FileSystemException(msg) {}
};

// Thrown when the directory or file path provided is incorrect.
class LocationException final : public FileSystemException {
public:
    explicit LocationException(const std::string& msg) : FileSystemException(msg) {}
};

#endif //FIRSTPROJECT_FILESYSTEMEXCEPTION_H