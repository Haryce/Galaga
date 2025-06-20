#include <iostream>
#include "Game.h"
#include <ctime>
#include <vector>
#include <conio.h> 
int main() {
    setlocale(LC_ALL, "Russian");
    Game game(20, 15); //запус игры с размерностью поля
    game.Run();

    return 0;
}
