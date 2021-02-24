#ifndef PATH_H
#define PATH_H

#include <string>

class Path
{
public:
    static Path& getInstance()
    {
        static Path instance;
        return instance;
    }
    void setPathToApp(const std::string& _path);
    std::string getPathToMaps();

private:
    Path() = default;
    Path(const Path& _other) = delete;
    Path(Path&& _other) = delete;

private:
std::string m_toApp;

};

#endif // PATH_H
