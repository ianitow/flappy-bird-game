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
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
static int BIRD_MODES[3][4] = {{0, 0, 64, 45},
                               {65, 0, 64, 45},
                               {130, 0, 64, 45}};

typedef struct
{
  int index;
  int x, y;
} TUNNELS_VARIABLES;

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
  ALLEGRO_BITMAP *logo;
  ALLEGRO_BITMAP *background;
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
} GAME_VARIABLES;
void drawTunnels(GAME_VARIABLES *GAME);

#endif