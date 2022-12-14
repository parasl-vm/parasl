#include <iostream>
#include "parser/driver.h"


int main() {
    frontend::parser::Driver driver;
    return driver.Parse();
}
