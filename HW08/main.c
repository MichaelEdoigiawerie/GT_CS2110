#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
#include "images/title.h"
#include "images/gamebg.h"
#include "images/ball.h"

//States.
enum gba_state {
  START,
  PLAY,
  PAUSE,
  PLAYER1WIN,
  PLAYER2WIN
};

//State Transition Methods
void goToStart(void);
void start(void);
void goToPlay(void);
void play(void);
void goToPause(void);
void pause(void);
void goToP1Win(void);
void p1Win(void);
void goToP2Win(void);
void p2Win(void);

// Load initial application state
enum gba_state state;

u32 previousButtons;
u32 currentButtons;

int main(void) {
  // Manipulate REG_DISPCNT to set Mode 3.
  REG_DISPCNT = MODE3 | BG2_ENABLE;

  // Save current and previous state of button input.
  previousButtons = BUTTONS;
  currentButtons = BUTTONS;

  //Display Start Screen
  goToStart();

  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons

    // Manipulate the state machine
    switch (state) {
      case START:
        start();
        break;
      case PLAY:
        play();
        break;
      case PAUSE:
        pause();
        break;
      case PLAYER1WIN:
        p1Win();
        break;
      case PLAYER2WIN:
        p2Win();
        break;
    }

    previousButtons = currentButtons; // Store the current state of the buttons
  }

  return 0;
}

//Method Definitions
void goToStart(void) {
  state = START;
  p1Score = 0;
  p2Score = 0;
  drawFullScreenImageDMA(title);
}

void start(void) {
  if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) 
    goToPlay();
}

void goToPlay(void) {
  if (state != PAUSE) init();
  state = PLAY;
  drawFullScreenImageDMA(gamebg);
}

void play(void) {
  update();
  //Button Controls
  if (KEY_DOWN(BUTTON_A, currentButtons)) 
    if (p1->y > 5) p1->y -= p1->dy;
  if (KEY_DOWN(BUTTON_B, currentButtons)) 
    if (p1->y + p1->height + 5 < HEIGHT) p1->y += p1->dy;
  if (KEY_DOWN(BUTTON_UP, currentButtons)) 
    if (p2->y > 5) p2->y -= p2->dy;
  if (KEY_DOWN(BUTTON_DOWN, currentButtons)) 
    if (p2->y + p2->height + 5 < HEIGHT) p2->y += p2->dy;
  
  //Drawing
  waitForVBlank();
  draw();

  //State Navigation Controls
  if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) 
    goToPause();
  if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons))
    goToStart();
  if (p1Score == 5 && timer == 1) {
    goToP1Win();
    p1Score = 0;
  }
  if (p2Score == 5 && timer == 1) {
    goToP2Win();
    p2Score = 0;
  };
}

void goToPause(void) {
  state = PAUSE;
  fillScreenDMA(WHITE);
  drawString(HEIGHT / 2 - 4, WIDTH / 2 - 3 * 6, "PAUSED", BLACK);
}

void pause(void) {
  if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) 
    goToPlay();
  if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons))
    goToStart();
}

void goToP1Win(void) {
  state = PLAYER1WIN;
  fillScreenDMA(RED);
  drawString(HEIGHT / 2 - 4, WIDTH / 2 - 3 * 15, "PLAYER 1 WINS!", CYAN);
}

void p1Win(void) {
  if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons))
    goToStart();
}

void goToP2Win(void) {
  state = PLAYER2WIN;
  fillScreenDMA(CYAN);
  drawString(HEIGHT / 2 - 4, WIDTH / 2 - 3 * 15, "PLAYER 2 WINS!", RED);
}

void p2Win(void) {
  if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons))
    goToStart();
}
