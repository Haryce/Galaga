#include "game.h"
#include <iostream>
#include <windows.h>    // Для Sleep, system
#include <cstdlib>      // Для system
#include <ctime>        // Для time

using namespace std;

// Константы для элементов игры
const char PLAYER = 'A';
const char ENEMY = 'V';
const char SHOT = '|';
const char EMPTY = ' ';

// Коды виртуальных клавиш
const int VK_A = 0x41;
const int VK_D = 0x44;
const int VK_P = 0x50;
const int VK_SPACE = 0x20;

Game::Game() {
    reset();
}

void Game::reset() {
    playerX = WIDTH / 2; // Центрируем игрока
    enemies.clear();     // Очищаем врагов
    shots.clear();       // Очищаем выстрелы
    enemySpawnTimer = 0; // Сбрасываем таймер
    killed = 0;
    passed = 0;
    isPaused = false;
    isGameOver = false;
    isWin = false;
}

void Game::run() {
    while (true) {
        // Главный игровой цикл
        while (!isGameOver) {
            handleInput();

            if (!isPaused) {
                update();
            }

            render();
            Sleep(50); // Задержка для управления скоростью
        }

        // Обработка завершения игры
        render();
        cout << (isWin ? "You WIN!" : "Game OVER!") << endl;
        cout << "Play again? (y/n): ";

        char choice;
        cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            reset();
        } else {
            break;
        }
    }
}

void Game::handleInput() {
    // Проверка состояния клавиш
    if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(VK_A)) {
        if (playerX > 0) playerX--;
    }
    if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(VK_D)) {
        if (playerX < WIDTH - 1) playerX++;
    }
    if (GetAsyncKeyState(VK_SPACE)) {
        // Добавляем выстрел только если предыдущий уже ушел достаточно далеко
        if (shots.empty() || shots.back() < HEIGHT - 5) {
            shots.push_back(HEIGHT - 2);
        }
    }
    if (GetAsyncKeyState(VK_P)) {
        isPaused = !isPaused;
        // Задержка для предотвращения многократного срабатывания
        Sleep(200);
    }
}

void Game::update() {
    // Генерация новых врагов
    if (++enemySpawnTimer >= 20) { // Каждые 20 итераций
        enemies.push_back(rand() % WIDTH);
        enemySpawnTimer = 0;
    }

    // Движение врагов вниз
    for (int i = 0; i < enemies.size(); i++) {
        // Удаление врагов достигших низа
        if (enemies[i] < 0) { // Помечаем достигших дна
            enemies.erase(enemies.begin() + i);
            passed++;
            i--;
            continue;
        }

        // Каждый 10-й кадр двигаем врагов
        if (rand() % 10 == 0) {
            enemies[i] += WIDTH; // Перемещаем в "нижний ряд"
        }
    }

    // Движение выстрелов
    for (int i = 0; i < shots.size(); i++) {
        shots[i]--; // Передвигаем выстрел вверх

        // Удаление выстрелов вышедших за пределы
        if (shots[i] < 0) {
            shots.erase(shots.begin() + i);
            i--;
            continue;
        }

        // Проверка столкновений
        for (int j = 0; j < enemies.size(); j++) {
            if (enemies[j] >= 0 && enemies[j] < WIDTH) { // Враги в верхнем ряду
                // Проверка координат X врага и выстрела
                if (enemies[j] == playerX && shots[i] == 0) {
                    // Уничтожение врага
                    enemies[j] = -1; // Помечаем для удаления
                    shots[i] = -1;   // Помечаем выстрел для удаления
                    killed++;
                    break;
                }
            }
        }
    }

    // Условие победы
    if (killed >= 10) {
        isGameOver = true;
        isWin = true;
    }

    // Условие поражения
    if (passed >= 3) {
        isGameOver = true;
    }
}

void Game::render() const {
    system("cls"); // Очистка консоли

    // Верхняя граница
    cout << '+';
    for (int x = 0; x < WIDTH; x++) cout << '-';
    cout << '+' << endl;

    // Игровое поле
    for (int y = 0; y < HEIGHT; y++) {
        cout << '|'; // Левая граница

        for (int x = 0; x < WIDTH; x++) {
            char cell = EMPTY;

            // Отрисовка врагов (верхний ряд)
            for (int e : enemies) {
                if (e == x && y == 0) {
                    cell = ENEMY;
                    break;
                }
            }

            // Отрисовка выстрелов
            for (int s : shots) {
                if (x == playerX && y == s) {
                    cell = SHOT;
                    break;
                }
            }

            // Отрисовка игрока
            if (y == HEIGHT - 1 && x == playerX) {
                cell = PLAYER;
            }

            cout << cell;
        }

        cout << '|'; // Правая граница

        // Вывод статистики
        if (y == 0) cout << " Killed: " << killed;
        if (y == 1) cout << " Passed: " << passed;
        if (y == 2) cout << " " << (isPaused ? "[PAUSED]" : "");

        cout << endl;
    }

    // Нижняя граница
    cout << '+';
    for (int x = 0; x < WIDTH; x++) cout << '-';
    cout << '+' << endl;

    // Подсказка по управлению
    cout << "Controls: A/<- D/-> SPACE-shoot P-pause" << endl;
}