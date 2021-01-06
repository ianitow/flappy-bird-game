#ifndef __EVENTS__GAME__
#define __EVENTS__GAME__
#include <stdlib.h>

typedef struct
{
  int cx1;
  int cx2;
  double speed;
  int press;
  int gover;
  int isMenuOpen;
  int isAnimating;
  int sair;
  int desenha;
  int currentState;
  int currentLevel;

} GAME_VARIABLES;

typedef struct
{
  int x, y;
  int playerState;
  int playerAnimStart;
  int playerAnimEnd;
  int playerAnimCurrent;
  int currentTunnel;
  int score;
} PLAYER_VARIABLES;

typedef struct
{
  int index;
  int x, y;
} TUNNELS_VARIABLES;

#endif