#include "pathResolver.h"
namespace fs = std::filesystem;

std::string resolve(std::string &name)
{
    const auto PATHs = splitString(std::getenv("PATH"), ':');
    for (auto &&path : PATHs)
    {
        for (fs::directory_entry &&f : fs::recursive_directory_iterator(path))
        {
            if (f.path.filename() == name)
            {
                return f.path.filename();
            }
        }
    }
    return std::string("");
}