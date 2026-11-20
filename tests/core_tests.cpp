#include "opensim_next/app/application.hpp"
#include <cassert>
#include <string_view>
int main() {
    using opensim_next::app::Application;
    assert(Application::name() == std::string_view{"OpenSim-next PC Viewer"});
    return 0;
}
