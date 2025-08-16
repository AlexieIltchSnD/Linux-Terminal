# Mini Terminal File System with Reference Counting
# Mini Terminal File System with Reference Counting

## Overview
This project implements a console-based mini-terminal simulating a virtual file system in C++.  
The system supports files with **reference counting** and virtual directories organized in a hierarchical structure.  
Users can interact with the system through commands, similar to a Unix-like terminal.

---

## Features

### File Object with Reference Counting
- **Random Access Operators** (`CharProxy`):
  - `operator[] (read)`: Read a character at a given position. Throws exception if out of bounds.
  - `operator[] (write)`: Write a character at a given position. Throws exception if out of bounds.
- **File Operations** (`File`, `FileValue`):
  - `touch`: Creates a new empty file or updates timestamp.
  - `copy`: Copies content from a source file to a target file.
  - `remove`: Deletes a file.
  - `move`: Moves content from source to target and deletes the original.
  - `cat`: Prints file content.
  - `wc`: Counts lines, words, and characters.
  - `ln`: Creates a hard link to an existing file (reference counting applied).

### Virtual Directory Object (`Directory`)
- **Directory Operations** (`Directory`, `DirectoryCommands`):
  - `mkdir`: Create a new directory.
  - `chdir`: Change current working directory.
  - `rmdir`: Delete a directory recursively.
  - `ls`: List directory contents.
  - `lproot`: Print the full file system hierarchy with reference counts.
  - `pwd`: Print the current working directory.

### Mini-Terminal Simulation (`Terminal`, `CommandGenerator`, `FilesCommands`, `DirectoryCommands`)
- Interactive REPL for executing commands.
- Outputs for commands like `cat`, `wc`, `ls`, `lproot`, and `pwd`.
- Handles invalid commands and errors gracefully.

---

## Supported Commands
| Command | Description |
|---------|-------------|
| `read FILENAME POSITION` | Read character at position from file. |
| `write FILENAME POSITION CHARACTER` | Write character at position to file. |
| `touch FILENAME` | Create a new file or update timestamp. |
| `copy SOURCE_FILENAME TARGET_FILENAME` | Copy file contents. |
| `remove FILENAME` | Delete a file. |
| `move SOURCE_FILENAME TARGET_FILENAME` | Move file contents. |
| `cat FILENAME` | Print file content. |
| `wc FILENAME` | Count lines, words, and characters. |
| `ln TARGET_FILENAME LINK_NAME` | Create a hard link. |
| `mkdir FOLDERNAME` | Create a new directory. |
| `chdir FOLDERNAME` | Change current working directory. |
| `rmdir FOLDERNAME` | Delete a directory recursively. |
| `ls FOLDERNAME` | List directory contents. |
| `lproot` | Print the full file system hierarchy. |
| `pwd` | Print current working directory. |
| `exit` | Exit the mini-terminal. |

---

## Project Structure
├── main.cpp # Entry point and REPL interface
├── Terminal.cpp/h # Handles user input and command execution
├── CommandGenerator.cpp/h # Parses and executes terminal commands
├── File.cpp/h # File object with reference counting
├── FileValue.cpp/h # Stores file content
├── FilesCommands.cpp # Implements file-related commands
├── Directory.cpp/h # Virtual directory object
├── DirectoryCommands.cpp # Implements directory-related commands
├── RCObject.h # Base class for reference-counted objects
├── RCPtr.h # Template for smart pointers
├── RefCountPointer.h # Reference counting pointer implementation
├── CharProxy.cpp/h # Proxy class for character access in files
├── FileSystemException.h # Custom exceptions for the file system


## Building the Project
This project uses **C++11** and above.

### Compilation Example (using g++):
```bash
g++ -std=c++11 -Wall -Wextra -o mini_terminal *.cpp

- Student: Alexie Iltchouk