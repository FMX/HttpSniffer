#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>

template <typename T>
std::string t_to_string(T i)
{
    std::stringstream ss;
    std::string s;
    ss << i;
    s = ss.str();
    return s;
}

#endif // UTILS_H
