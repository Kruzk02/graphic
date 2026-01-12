#include <application.h>

int main() {
    Application application {{900, 720, "asset/shader/cube-shader.vs", "asset/shader/cube-shader.fs"}};
    application.run();
    return 0;
}
