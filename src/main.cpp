#include <application.h>

int main() {
    Application application {{900, 720, "asset/shader/lighting.vs", "asset/shader/lighting.fs"}};
    application.run();
    return 0;
}
