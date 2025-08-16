#include "CommandGenerator.h"
#include "sstream"

// The function receives a path, and separates him via the delimiter.
// For example, 'V/gg/tt/ss/h', splits it into 'V','gg','tt','ss','h', return the vector.
std::vector<std::string> separatePath(const std::string& path, const char delimiter) {
    if (path.find("//") != std::string::npos) {
        throw CommandException("Invalid path: contains consecutive slashes.");
    }
    std::vector<std::string> result;
    std::stringstream stream(path);
    std::string item;
    while (std::getline(stream, item, delimiter)) {
        if (!item.empty()) result.push_back(item);
    }
    return result;
}