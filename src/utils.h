#ifndef DSUN_UTILS_H
#define DSUN_UTILS_H
#include <string>
#include <sstream>

namespace dsun_utils {
  template < typename T > std::string to_string(const T& n) {
    std::ostringstream stm;
    stm << n;
    return stm.str();
  }
}

#endif // DSUN_UTILS_H