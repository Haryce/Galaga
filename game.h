#pragma once
#include <vector>
#include <string>
#include <windows.h>

using namespace std;

class Game {
private:
    // Размеры игрового поля
    static const int WIDTH = 40;
    static const int HEIGHT = 20;

    // Состояние игрока
    int playerX;        // Позиция игрока по оси X

    // Элементы игры
    vector<int> enemies;    // Враги (координата X)
    vector<int> shots;      // Выстрелы (координата Y)
    int enemySpawnTimer;    // Таймер появления врагов

    // Счетчики
    int killed;         // Уничтожено врагов
    int passed;         // Врагов достигших низа

    // Флаги состояния
    bool isPaused;      // Пауза в игре
    bool isGameOver;    // Игра завершена
    bool isWin;         // Флаг победы

public:
    Game();
    void run();         // Главный игровой цикл

private:
    void handleInput(); // Обработка ввода пользователя
    void update();      // Обновление игрового состояния
    void render() const;// Отрисовка игрового поля
    void reset();       // Сброс состояния игры
};