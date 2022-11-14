#include "game.h"
#include "player.h"
#include <stdio.h>
#define OUTPUT_FILE "record.txt"
// DRAW FUNCTIONS

void drawTunnels(GAME_VARIABLES *GAME)
{

  al_draw_bitmap(GAME->tunnel_up, GAME->TUNNELS[0].x, GAME->TUNNELS[0].y, 0);
  al_draw_bitmap(GAME->tunnel_down, GAME->TUNNELS[0].x, GAME->TUNNELS[0].y + 700, 0);
  al_draw_bitmap(GAME->tunnel_up, GAME->TUNNELS[1].x, GAME->TUNNELS[1].y, 0);
  al_draw_bitmap(GAME->tunnel_down, GAME->TUNNELS[1].x, GAME->TUNNELS[1].y + 700, 0);
  al_draw_bitmap(GAME->tunnel_up, GAME->TUNNELS[2].x, GAME->TUNNELS[2].y, 0);
  al_draw_bitmap(GAME->tunnel_down, GAME->TUNNELS[2].x, GAME->TUNNELS[2].y + 700, 0);
}
void drawPoints(GAME_VARIABLES *GAME, PLAYER_VARIABLES *PLAYER)
{
  al_draw_textf(GAME->scoreboard, al_map_rgb(0, 0, 0), 10, 10, ALLEGRO_ALIGN_LEFT, "Score: %d", PLAYER->score);
  al_draw_textf(GAME->scoreboard, al_map_rgb(0, 0, 0), LARGURA_TELA - 20, 10, ALLEGRO_ALIGN_RIGHT, "Level: %d", GAME->currentLevel);
}
void drawPlayer(GAME_VARIABLES *GAME, PLAYER_VARIABLES *PLAYER)
{
  GAME->desenha = 1;
  al_draw_bitmap_region(PLAYER->bird_sprites, BIRD_MODES[PLAYER->playerState][0], BIRD_MODES[PLAYER->playerState][1], BIRD_MODES[PLAYER->playerState][2], BIRD_MODES[PLAYER->playerState][3],
                        PLAYER->x, PLAYER->y + 22.5, 0);
}
void drawRecord(GAME_VARIABLES *GAME, PLAYER_VARIABLES *PLAYER)
{

  al_draw_textf(GAME->scoreboard, al_map_rgb(120, 120, 120), 10, 35, ALLEGRO_ALIGN_LEFT, "Recorde: %d", PLAYER->maxScore);
}
// EVENTS
void onPlayerInitGame(GAME_VARIABLES *GAME, PLAYER_VARIABLES *PLAYER)
{
  al_show_mouse_cursor(GAME->janela);
  GAME->currentLevel = 1;
  GAME->currentState = ESTADO_INICIAL;
  PLAYER->score = 0;
  GAME->lastTunnelIndex = 0;
  GAME->totalTunnelsPassed = 0;
  readRecord(PLAYER, OUTPUT_FILE);
  recountTunnels(GAME, PLAYER);
}
void onPlayerStartGame(GAME_VARIABLES *GAME, PLAYER_VARIABLES *PLAYER)
{
  GAME->totalTunnelsGenerated = TOTAL_TUNELS;
  GAME->currentState = ESTADO_PRE_MENU;
  GAME->speed = 0;
}
void onPlayerPlayGame(GAME_VARIABLES *GAME, PLAYER_VARIABLES *PLAYER)
{

  al_hide_mouse_cursor(GAME->janela);
  GAME->currentState = ESTADO_JOGANDO;
  GAME->speed = DEFAULT_SPEED;
}
void onPlayerLoseGame(GAME_VARIABLES *GAME, PLAYER_VARIABLES *PLAYER)
{
  if (PLAYER->score > PLAYER->maxScore)
  {
    PLAYER->maxScore = PLAYER->score;
    createOrUpdateRecord(PLAYER, OUTPUT_FILE);
  }
  al_set_system_mouse_cursor(GAME->janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
  al_play_sample(GAME->som_dead, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
  al_rest(1);
  GAME->currentState = ESTADO_PERDEU;
  al_show_mouse_cursor(GAME->janela);
}
void onPlayerPassTunnel(GAME_VARIABLES *GAME, PLAYER_VARIABLES *PLAYER)
{
  PLAYER->score++;
  GAME->totalTunnelsPassed++;
 
}
// OTHERS
void initBackground(BACKGROUND *bg, int x, int y, int width, int height, int velX, int velY, int dirX, int dirY, ALLEGRO_BITMAP *image)
{

  bg->x = x;
  bg->y = y;
  bg->width = width;
  bg->height = height;
  bg->velX = velX;
  bg->velY = velY;
  bg->dirX = dirX;
  bg->dirY = dirY;
  bg->backgroundImage = image;
}

void updateBackground(BACKGROUND *bg)
{
  bg->x += bg->velX * bg->dirX;
  if (bg->x + bg->width <= 0)
    bg->x = 0;
}
void drawBackground(BACKGROUND *bg)
{

  al_draw_bitmap(bg->backgroundImage, bg->x, bg->y, 0);
  if (bg->x + bg->width < LARGURA_TELA)
    al_draw_bitmap(bg->backgroundImage, bg->x + bg->width, bg->y, 0);
}
void recountTunnels(GAME_VARIABLES *GAME, PLAYER_VARIABLES *PLAYER)
{
  //+700 ENTRE CIMA E BAIXO
  // MIN: -500
  // MAX: 500
  const int maximum_number = -200;
  const int minimum_number = -500;
  GAME->TUNNELS[0].index = 0;
  GAME->TUNNELS[0].x = GAME->currentLevel == 1 ? 650 : 1500;
  GAME->TUNNELS[0].y = (rand() % (maximum_number + 1 - minimum_number)) + minimum_number;
  GAME->TUNNELS[0].passed = 0;
  
  GAME->TUNNELS[1].index = 1;
  GAME->TUNNELS[1].x = GAME->TUNNELS[0].x + (rand() % (250 + 1 - 200)) + 250;
  GAME->TUNNELS[1].y = (rand() % (maximum_number + 1 - minimum_number)) + minimum_number;
  GAME->TUNNELS[1].passed = 0;
  GAME->TUNNELS[2].index = 2;
  GAME->TUNNELS[2].x = GAME->TUNNELS[1].x+ (rand() % (250 + 1 - 200)) + 250;
  GAME->TUNNELS[2].y = (rand() % (maximum_number + 1 - minimum_number)) + minimum_number;
  GAME->TUNNELS[2].passed = 0;
}

void createOrUpdateRecord(PLAYER_VARIABLES *PLAYER, char *input)
{
  FILE *file;
  if (file = fopen(input, "w+"))
  {
    fwrite(&PLAYER->maxScore, sizeof(int), 1, file);

    fclose(file);
  }
  else
  {
    printf("ERRO");
  }
}
void readRecord(PLAYER_VARIABLES *PLAYER, char *input)
{
  int buffer;
  FILE *stream;
  // Reading value from file
  stream = fopen(OUTPUT_FILE, "r");
  if (!stream)
    return;
  fread(&buffer, sizeof(int), 1, stream);
  PLAYER->maxScore = (int)buffer;
  fclose(stream);
}