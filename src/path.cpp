#include "path.h"

void Path::setPathToApp(const std::string &_path)
{
    m_toApp = _path;
}

std::string Path::getPathToMaps()
{
    return m_toApp + "/maps";
}
