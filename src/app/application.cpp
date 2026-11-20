#include "opensim_next/app/application.hpp"
#include <iostream>
namespace opensim_next::app {
int Application::run() const {
    std::cout << name() << '\n';
    std::cout << "Bootstrap build: protocol and renderer modules are not implemented yet.\n";
    return 0;
}
}
