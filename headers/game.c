#include "game.h"
#include "player.h"
#include <stdio.h>
#define OUTPUT_FILE "record.txt"
//DRAW FUNCTIONS
void drawTunnels(GAME_VARIABLES *GAME)
{
  for (int i = 0; i < (GAME->currentLevel * 10); i++)
  {
    al_draw_bitmap(GAME->tunnel_up, GAME->TUNNELS[i].x, GAME->TUNNELS[i].y, 0);
    al_draw_bitmap(GAME->tunnel_down, GAME->TUNNELS[i].x, GAME->TUNNELS[i].y + 700, 0);
  }
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
//EVENTS
void onPlayerInitGame(GAME_VARIABLES *GAME, PLAYER_VARIABLES *PLAYER)
{
  al_show_mouse_cursor(GAME->janela);
  GAME->currentLevel = 1;
  GAME->currentState = ESTADO_INICIAL;
  PLAYER->score = 0;
  readRecord(PLAYER, OUTPUT_FILE);
  recountTunnels(GAME, PLAYER);
}
void onPlayerStartGame(GAME_VARIABLES *GAME, PLAYER_VARIABLES *PLAYER)
{

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
  }
  al_set_system_mouse_cursor(GAME->janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
  al_play_sample(GAME->som_dead, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
  drawPlayer(GAME, PLAYER);
  al_rest(1);
  GAME->currentState = ESTADO_PERDEU;
  al_show_mouse_cursor(GAME->janela);
  createOrUpdateRecord(PLAYER, OUTPUT_FILE);
}
void onPlayerPassTunnel(GAME_VARIABLES *GAME, PLAYER_VARIABLES *PLAYER)
{
  PLAYER->score++;
}
//OTHERS
void recountTunnels(GAME_VARIABLES *GAME, PLAYER_VARIABLES *PLAYER)
{
  //+700 ENTRE CIMA E BAIXO
  //MIN: -500
  //MAX: 500

  GAME->TUNNELS = realloc(GAME->TUNNELS, GAME->currentLevel * 10 * sizeof(TUNNELS_VARIABLES));
  const int maximum_number = -200;
  const int minimum_number = -500;
  GAME->TUNNELS[0].index = 0;
  GAME->TUNNELS[0].x = 650;
  GAME->TUNNELS[0].y = (rand() % (maximum_number + 1 - minimum_number)) + minimum_number;
  for (int i = 1; i < (GAME->currentLevel * 10); i++)
  {
    GAME->TUNNELS[i].index = i;
    GAME->TUNNELS[i].x = GAME->TUNNELS[i - 1].x + (rand() % (250 + 1 - 200)) + 250;
    GAME->TUNNELS[i].y = (rand() % (maximum_number + 1 - minimum_number)) + minimum_number;
    ;
  }
}
void createOrUpdateRecord(PLAYER_VARIABLES *PLAYER, char *input)
{
  FILE *file;
  if (file = fopen(input, "w+"))
  {
    fprintf(file, "%d", PLAYER->maxScore);
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
  stream = fopen("file.txt", "r");
  if (!stream)
    return;
  fread(&buffer, sizeof(int), 1, stream);

  fclose(stream);
}