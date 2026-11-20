#pragma once
#include <string_view>
namespace opensim_next::app {
class Application final {
public:
    [[nodiscard]] int run() const;
    [[nodiscard]] static constexpr std::string_view name() noexcept {
        return "OpenSim-next PC Viewer";
    }
};
}
