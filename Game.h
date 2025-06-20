#pragma once
#include <windows.h>
#include <conio.h>   //ввода с клавиатуры
#include <vector>    //для хранения врагов
#include <ctime>     
using namespace std;

//класс игрока
class Player {
public:
    int x, y; //позиция игрока
    bool isShooting; //выстрела

    Player(int startX, int startY);
    void Move(int dx); //движение игрока
    void Shoot();      //выстрел игрока
};
// Класс для врага
class Enemy {
public:
    int x, y; //позиция врага
    bool isActive; //активность врага
    Enemy(int startX, int startY);
    void Move(); //движение врага
};
//класс игры
class Game {
private:
    int width, height;       //игровое поля
    Player* player;          //указатель игрока
    vector<Enemy*> enemies;  //вектор врагов
    int killedEnemies;       //количество убитых врагов
    int reachedBottom;       //граница низа
    bool isPaused;           //pause
    bool isGameOver;         //завершение
    bool playerWon;          //победа

    void SpawnEnemy();       //создание врага
    void ProcessInput();     //обработка ввода
    void Update();           //состояния игры
    void Draw();             //отрисовка
    void CheckCollisions();  //коллизия
    void ResetGame();        //ресет игры
public:
    Game(int w, int h);
    ~Game();
    void Run();              //игровой цикл
};
