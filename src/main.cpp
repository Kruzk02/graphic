#include <application.h>

int main() {
    Application application {{900, 720, "asset/shader/shader.vs", "asset/shader/shader.fs"}};
    application.run();
    return 0;
}
