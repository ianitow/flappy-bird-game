#ifndef __GLOBAL__GAME__
#define __GLOBAL__GAME__
#define LARGURA_TELA 480
#define ALTURA_TELA 630
#define FPS 60.0
#define FPS_ANIM 10.0
#define DEFAULT_SPEED 5
#define ESTADO_INICIAL 1
#define ESTADO_PRE_MENU 2
#define ESTADO_JOGANDO 3
#define ESTADO_PERDEU 4
#define NIVEL_MAX_VELOCITY 8
#define LARGURA_TELA 480
#define ALTURA_TELA 630
#define TOTAL_TUNELS 3
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

typedef struct
{
  int index;
  int x, y;
  int passed;
} TUNNELS_VARIABLES;

typedef struct
{
  ALLEGRO_BITMAP *backgroundImage;
  int x,y;
  int width,height;
  int velX,velY;
  int dirX,dirY;
} BACKGROUND;


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
  int totalTunnelsPassed;
  int totalTunnelsGenerated;
  int lastTunnelIndex;
  ALLEGRO_BITMAP *logo;
  BACKGROUND background;
  ALLEGRO_BITMAP *i_floor;
  ALLEGRO_BITMAP *button_play;
  ALLEGRO_BITMAP *pre_menu;
  ALLEGRO_BITMAP *tunnel_up;
  ALLEGRO_BITMAP *tunnel_down;
  ALLEGRO_BITMAP *gameover;
  TUNNELS_VARIABLES *TUNNELS;
  ALLEGRO_FONT *scoreboard;
  ALLEGRO_SAMPLE *som_levelup;
  ALLEGRO_SAMPLE *som_dead;
  ALLEGRO_AUDIO_STREAM *musica;
  ALLEGRO_DISPLAY *janela;
  ALLEGRO_EVENT_QUEUE *fila_eventos;
  ALLEGRO_EVENT_QUEUE *fila_eventos_anim;
  ALLEGRO_TIMER *timer;
  ALLEGRO_TIMER *timer_anim;
  int render;
} GAME_VARIABLES;
void drawTunnels(GAME_VARIABLES *GAME);
void initBackground(BACKGROUND *bg, int x, int y,  int width, int height,int velX, int velY, int dirX, int dirY,ALLEGRO_BITMAP *image);

void updateBackground(BACKGROUND *bg );
void drawBackground(BACKGROUND *bg);
#endif