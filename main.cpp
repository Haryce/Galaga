#include "game.h"

int main() {
    srand(static_cast<unsigned>(time(nullptr))); // Инициализация генератора случайных чисел

    Game game;
    game.run();

    return 0;
}