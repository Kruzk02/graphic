#include <application.h>

namespace ApplicationScreen
{
constexpr int width = 1080;
constexpr int height = 720;
}; // namespace ApplicationScreen

auto main() -> int
{
    Application application{{.width = ApplicationScreen::width,
                             .height = ApplicationScreen::height,
                             .shaderVertex = "asset/shader/lighting.vs",
                             .shaderFragment = "asset/shader/lighting.fs"}};
    application.run();
    return 0;
}
