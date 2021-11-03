#ifndef GRID_H
#define GRID_H

#include <vector>
#include <random>
#include <fstream>
#include <iostream>
#include <cmath>

enum class Way{Right, Left, Up, Down};

class Person{
    int x_;
    int money_;

    public:
    Person(int x, int money): x_{x}, money_{money} {}
    Person() {}

    int GetPosition(){ return x_; }
    void SetPosition(int i){ x_ = i; }
    int GetMoney(){ return money_; }
    void SetMoney(int S);
    void Move(Way &direction, int l);

};

void SetGrid(std::vector<Person> &Grid, int l, int Cap);
void Gamble(double &p, Person &p1, Person &p2);
void Evolve(int F, int l, int N, int Cap);

#endif