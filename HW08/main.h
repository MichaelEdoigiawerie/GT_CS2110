#ifndef MAIN_H
#define MAIN_H

#include "gba.h"
#include "images/gamebg.h"
#include "images/ball.h"

#define BALLSIZE 10
#define PLAYERHEIGHT 30
#define PLAYERWIDTH 10
#define BALLSPEED 3
#define PLAYERSPEED 2

//Entities
typedef struct {
    int x;
    int y;
    int width;
    int height;
    int dy;
} PLAYER;

typedef struct {
    int x;
    int y;
    int size;
    int dx;
    int dy;
    int direction;
    int active;
} BALL;

//Game State
typedef struct {
    PLAYER p1;
    PLAYER p2;
    BALL b;
} STATE;

//Glabal Variable Declarations
extern PLAYER *p1, *op1;
extern PLAYER *p2, *op2;
extern BALL *b, *ob;
extern int timer;
extern int p1Score;
extern int p2Score;

//Game Methods
void init(void );
void initPlayers(void);
void initBall(void);
void update(void);
void draw(void);
void updatePlayers(PLAYER *p1, PLAYER *p2);
void drawPlayers(void);
void updateBall(BALL *b);
void drawBall(void);

//Helper Method
int collision(int rowA, int colA, int widthA, int heightA, int rowB, int colB, int widthB, int heightB);

#endif
