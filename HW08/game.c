#include <stdio.h>
#include "gba.h"
#include "main.h"
#include "images/gamebg.h"
#include "images/ball.h"

//Entities
PLAYER *p1, *op1;
PLAYER *p2, *op2;
BALL *b, *ob;

//States
STATE currState, prevState;

//Variables
int timer;
int p1Score;
int p2Score;

//Game Methods
void init(void) {
    timer = 3;
    p1 = &currState.p1;
    p2 = &currState.p2;
    b = &currState.b;
    initPlayers();
    initBall();
}

void initPlayers(void) {
    //Player 1
    p1->x = 5;
    p1->y = HEIGHT / 2 - PLAYERHEIGHT / 2;
    p1->height = PLAYERHEIGHT;
    p1->width = PLAYERWIDTH;
    p1->dy = PLAYERSPEED;

    //Player 2
    p2->x = WIDTH - PLAYERWIDTH - 5;
    p2->y = HEIGHT / 2 - PLAYERHEIGHT / 2;
    p2->height = PLAYERHEIGHT;
    p2->width = PLAYERWIDTH;
    p2->dy = PLAYERSPEED;
}

void initBall(void) {
    b->direction = randint(-1, 2);
    if (b->direction == 0) b->direction = -1;
    b->x = WIDTH / 2 - BALLSIZE / 2;
    b->y = HEIGHT / 2 - BALLSIZE / 2;
    b->size = BALLSIZE;
    b->dx = BALLSPEED * b->direction;
    b->dy = BALLSPEED * b->direction;
    b->active = 1;
}

void update(void) {
    if (vBlankCounter % 60 == 0 && timer > 0) {
        timer--;
    }
    prevState = currState;
    updatePlayers(p1, p2);
    if (timer == 0) updateBall(b);
}

void draw(void) {
    drawPlayers();
    drawBall();
}

void updatePlayers(PLAYER *p1, PLAYER *p2) {
    if (collision(p1->y, p1->x, p1->width, p1->height, b->y, b->x, b->size, b->size) ||
    collision(p2->y, p2->x, p2->width, p2->height, b->y, b->x, b->size, b->size)) {
        int newDir = randint(-1, 2);
        if (newDir == 0) newDir = -1;
        b->dx = -b->dx;
        b->dy *= newDir;
    }
}

void drawPlayers(void) {
    //Player 1
    op1 = &prevState.p1;
    undrawImageDMA(op1->y, op1->x, op1->width, op1->height, gamebg);
    drawRectDMA(p1->y, p1->x, p1->width, p1->height, RED);
    //Player 2
    op2 = &prevState.p2;
    undrawImageDMA(op2->y, op2->x, op2->width, op2->height, gamebg);
    drawRectDMA(p2->y, p2->x, p2->width, p2->height, WHITE);
    //Score Tracker
    char buffer[10];
    int textWidth = 10;
    sprintf(buffer, "%d   %d", p1Score, p2Score);
    undrawImageDMA(5, WIDTH / 2 - textWidth - 4, 48, 8, gamebg);
    drawString(5, WIDTH / 2 - textWidth - 4, buffer, WHITE);
}

void updateBall(BALL *b) {
    if (b->active) {
        b->x += b->dx;
        b->y += b->dy;
        if (b->x <= 0 || b->x + b->size >= WIDTH) {
            b->active = 0;
        }
        if (b->y <= 0 || b->y + b->size >= HEIGHT) b->dy = -b->dy;
        if (b->x + b->size >= WIDTH) p1Score++;
        if (b->x <= 0) p2Score++;
    } else {
        init();
    }
}

void drawBall(void) {
    ob = &prevState.b;
    undrawImageDMA(ob->y, ob->x, ob->size, ob->size, gamebg);
    if (b->active) {
        drawImageDMA(b->y, b->x, b->size, b->size, ball);
    }
}

//Helper Method
int collision(int rowA, int colA, int widthA, int heightA, int rowB, int colB, int widthB, int heightB) {
    if (rowA < rowB + heightB && rowA + heightA > rowB 
    && colA < colB + widthB && colA + widthA > colB) {
        return 1;
    }
    return 0;
}