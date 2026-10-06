#include "core/Game.h"
#include "utils/Logger.h"

int main() {
    Logger::instance().setLogFile("game.log");
    LOG_INFO("Starting RUNTIME: The Last Algorithm");

    Game game;
    game.init();
    game.run();
    game.shutdown();

    return 0;
}
