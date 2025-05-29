//
// Created by UD on 29/5/2025.
//

#include "pongis.h"
//
// Created by UD on 29/5/2025.
//

typedef struct {
    int x , y;
    int vx, vy;
    int radius;
}Ball;

typedef struct {
    int x, y , size;
} Hole;

typedef struct {
    double angle; // 0-359
    double power;

}Player;


