#ifndef PARASL_DEBUG_H_
#define PARASL_DEBUG_H_

#include <iostream>


namespace utils {
void AssertionFail(const char *expr, const char *file, unsigned line, const char *function);
void PrintWarning(const char *mess, const char *file, unsigned line, const char *function);

void AssertionFail(std::string expr, const char *file, unsigned line, const char *function);
void PrintWarning(std::string mess, const char *file, unsigned line, const char *function);
}   // end namespace utils

#endif // PARASL_DEBUG_H_
