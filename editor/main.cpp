

#include "src/Editor.hpp"
#include "src/DocumentWindows/ConsoleWindow.hpp"

#include <thread>

int main(int argc, char **argv)
{
    loguru::init(argc, argv);
    loguru::g_stderr_verbosity = loguru::Verbosity_1;
    nexo::editor::Editor editor;
    editor.registerWindow("Console", std::make_shared<nexo::editor::ConsoleWindow>(editor));
    editor.init();

    while (editor.isOpen())    // Detect window close button or ESC key
    {
        auto start = std::chrono::high_resolution_clock::now();
        editor.update();
        editor.render();

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        std::this_thread::sleep_for(std::chrono::milliseconds(16) - elapsed);
    }

    return 0;
}