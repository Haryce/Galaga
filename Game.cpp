#include "Game.h"
#include <iostream>
Player::Player(int startX, int startY) : x(startX), y(startY), isShooting(false) {}

//движение игрока
void Player::Move(int dx) {
    x += dx;
    // Ограничение движения
    if (x < 1) x = 1;
    if (x > 20) x = 20; //ширина
}

void Player::Shoot() {
    isShooting = true; 
}

Enemy::Enemy(int startX, int startY) : x(startX), y(startY), isActive(true) {}

void Enemy::Move() {
    y++;
}

Game::Game(int w, int h) : width(w), height(h), killedEnemies(0), reachedBottom(0),
isPaused(false), isGameOver(false), playerWon(false) {
    player = new Player(width / 2, height - 2); //создаем игрока в центре снизу
    srand(static_cast<unsigned int>(time(0)));  //генератор случайных чисел
}

//деструктор
Game::~Game() {
    delete player;
    for (auto enemy : enemies) {
        delete enemy;
    }
}

//создание нового врага
void Game::SpawnEnemy() { //создание врагов
    int x = rand() % width + 1;
    enemies.push_back(new Enemy(x, 1));
}
void Game::ProcessInput() { //ввод
    if (_kbhit()) {
        int key = _getch();
        //движения влево
        if (key == 'a' || key == 'A' || key == 75) {
            player->Move(-1);
        }
        //движения вправо
        else if (key == 'd' || key == 'D' || key == 77) {
            player->Move(1);
        }
        //выстрел
        else if (key == ' ') {
            player->Shoot();
        }
        //пауза
        else if (key == 'p' || key == 'P') {
            isPaused = !isPaused;
        }
    }
}
void Game::CheckCollisions() {
    //выстрел игрока
    if (player->isShooting) {
        for (size_t i = 0; i < enemies.size(); ++i) {
            if (enemies[i]->isActive && enemies[i]->x == player->x) {
                enemies[i]->isActive = false; 
                killedEnemies++;
                //условие победы
                if (killedEnemies >= 10) {
                    isGameOver = true;
                    playerWon = true;
                }
            }
        }
        player->isShooting = false; //Сброс выстрел
    }
    for (size_t i = 0; i < enemies.size(); ++i) { //проверяем врагов, достигших низа
        if (enemies[i]->isActive && enemies[i]->y >= height - 1) {
            enemies[i]->isActive = false;
            reachedBottom++;
            //проверяем условий поражения
            if (reachedBottom >= 3) {
                isGameOver = true;
                playerWon = false;
            }
        }
    }

    //удаление неактивных врагов
    enemies.erase(remove_if(enemies.begin(), enemies.end(),
        [](Enemy* e) {
            if (!e->isActive) {
                delete e;
                return true;
            }
            return false;
        }),
        enemies.end());
}
//обновление состояния игры
void Game::Update() {
    if (isPaused || isGameOver) return;
    //случайное создание нового врага
    if (rand() % 10 == 0) { //10% шанс создания врага каждый кадр
        SpawnEnemy();
    }

    //движение врагов
    for (auto enemy : enemies) {
        if (enemy->isActive) {
            enemy->Move();
        }
    }
    CheckCollisions(); //проверка столкновений
}
// Отрисовка игры
void Game::Draw() {
    system("cls"); //очистка консоль
    //верхнюю границу
    for (int i = 0; i < width + 2; i++) cout << "#"; //верхнюю границу
    cout << endl;
    for (int y = 1; y <= height; y++) { //игровое поле
        cout << "#"; // Левая граница
        for (int x = 1; x <= width; x++) {
            bool isPlayer = (x == player->x && y == player->y);
            bool isEnemy = false;
            bool isShot = (player->isShooting && x == player->x && y < player->y);
            // Проверяем врага на позиции
            for (auto enemy : enemies) {
                if (enemy->isActive && enemy->x == x && enemy->y == y) {
                    isEnemy = true;
                    break;
                }
            }
            if (isPlayer) cout << "A";
            else if (isEnemy) cout << "V";
            else if (isShot) cout << "|";
            else cout << " ";
        }
        cout << "#" << endl; //правая граница
    }
    //нижняя граница
    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << endl;
    //вывод статистику
    cout << "Уничтожено: " << killedEnemies << " / 10" << endl;
    cout << "Достигло низа: " << reachedBottom << " / 3" << endl;
    if (isPaused) cout << "ПАУЗА" << endl;
    if (isGameOver) {
        if (playerWon) cout << "ПОБЕДА! Нажмите R для рестарта или любую другую клавишу для выхода" << endl;
        else cout << "ПОРАЖЕНИЕ! Нажмите R для рестарта или любую другую клавишу для выхода" << endl;
    }
}

// Сброс игры
void Game::ResetGame() {
    //очищаем врагов
    for (auto enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
    //cбос статистику
    killedEnemies = 0;
    reachedBottom = 0;
    isGameOver = false;
    playerWon = false;
    delete player;
    player = new Player(width / 2, height - 2);
}
void Game::Run() {
    while (true) {
        ProcessInput(); //обработка ввода
        Update();      //обновление состояния
        Draw();        //отрисовка
        if (isGameOver) { //рестарт
            if (_kbhit()) {
                int key = _getch();
                if (key == 'r' || key == 'R') {
                    ResetGame();
                }
                else {
                    break;
                }
            }
        }

        Sleep(100); //задержка для управления скоростью игры
    }
}
