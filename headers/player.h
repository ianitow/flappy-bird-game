#ifndef __PLAYER__GAME__
#define __PLAYER__GAME__
#include <stdlib.h>
#include <allegro5/allegro5.h>
static int BIRD_MODES[3][4] = {{0, 0, 64, 45},
                               {65, 0, 64, 45},
                               {130, 0, 64, 45}};

typedef struct
{
  int x, y;
  int playerState;
  int playerAnimUp;
  int currentTunnel;
  int score;
  int maxScore;
  ALLEGRO_BITMAP *bird_sprites;
} PLAYER_VARIABLES;
//Draw
void drawPoints(GAME_VARIABLES *GAME, PLAYER_VARIABLES *PLAYER);
void drawPlayer(GAME_VARIABLES *GAME, PLAYER_VARIABLES *PLAYER);
void drawRecord(GAME_VARIABLES *GAME, PLAYER_VARIABLES *PLAYER);
//Events
void onPlayerInitGame(GAME_VARIABLES *GAME, PLAYER_VARIABLES *PLAYER);
void onPlayerStartGame(GAME_VARIABLES *GAME, PLAYER_VARIABLES *PLAYER);
void onPlayerPlayGame(GAME_VARIABLES *GAME, PLAYER_VARIABLES *PLAYER);
void onPlayerLoseGame(GAME_VARIABLES *GAME, PLAYER_VARIABLES *PLAYER);
//OTHERS
void recountTunnels(GAME_VARIABLES *GAME, PLAYER_VARIABLES *PLAYER);
void onPlayerPassTunnel(GAME_VARIABLES *GAME, PLAYER_VARIABLES *PLAYER);
void createOrUpdateRecord(PLAYER_VARIABLES *PLAYER, char *input);
void readRecord(PLAYER_VARIABLES *PLAYER, char *input);
#endif