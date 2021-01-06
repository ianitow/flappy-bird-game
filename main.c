// Os arquivos de cabeçalho
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "headers/game.h"
// Atributos da tela
#define FPS 60.0
#define FPS_ANIM 10.0
#define LARGURA_TELA 480
#define ALTURA_TELA 630
#define DEFAULT_SPEED 5
#define ESTADO_INICIAL 1
#define ESTADO_PRE_MENU 2
#define ESTADO_JOGANDO 3
#define ESTADO_PERDEU 4
#define NIVEL_MAX_VELOCITY 8
//Variaveis Globais
GAME_VARIABLES GAME = {0, 480, DEFAULT_SPEED, 0, 0, 1, 1, 0, 1, 1, 1};
PLAYER_VARIABLES PLAYER = {(LARGURA_TELA / 2), (ALTURA_TELA / 2), 0, 600, 0, 0};

TUNNELS_VARIABLES *TUNNELS = NULL;
//FONT
ALLEGRO_FONT *scoreboard = NULL;
//Imagens
ALLEGRO_BITMAP *logo = NULL;
ALLEGRO_BITMAP *background = NULL;
ALLEGRO_BITMAP *i_floor = NULL;
ALLEGRO_BITMAP *button_play = NULL;
ALLEGRO_BITMAP *pre_menu = NULL;
ALLEGRO_BITMAP *bird_sprites = NULL;
ALLEGRO_BITMAP *tunnel_up = NULL;
ALLEGRO_BITMAP *tunnel_down = NULL;
ALLEGRO_BITMAP *gameover = NULL;
//SONS
ALLEGRO_SAMPLE *som_levelup = NULL;
ALLEGRO_SAMPLE *som_dead = NULL;
ALLEGRO_AUDIO_STREAM *musica = NULL;
//Configs
ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos_anim = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_TIMER *timer_anim = NULL;
//Stuct player
static int BIRD_MODES[3][4] = {{0, 0, 64, 45},
                               {65, 0, 64, 45},
                               {130, 0, 64, 45}};
void init_tunnels();
void reset_game();

void error_msg(char *text)
{
    al_show_native_message_box(NULL, "ERRO",
                               "Ocorreu o seguinte erro e o programa sera finalizado:",
                               text, NULL, ALLEGRO_MESSAGEBOX_ERROR);
}

int inicializar()
{

    if (!al_init())
    {
        error_msg("Falha ao inicializar a Allegro");
        return 0;
    }
    al_init_primitives_addon();
    if (!al_install_audio())
    {
        error_msg("Falha ao inicializar o audio");
        return 0;
    }
    if (!al_init_acodec_addon())
    {
        error_msg("Falha ao inicializar o codec de audio");
        return 0;
    }

    //cria o mixer (e torna ele o mixer padrao), e adciona 5 samples de audio nele
    if (!al_reserve_samples(5))
    {
        error_msg("Falha ao reservar amostrar de audio");
        return 0;
    }
    musica = al_load_audio_stream("sound/trilha.wav", 4, 500);
    if (!musica)
    {
        error_msg("Audio nao carregado");

        return 0;
    }
    al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());
    //define que o stream vai tocar no modo repeat
    al_set_audio_stream_playmode(musica, ALLEGRO_PLAYMODE_LOOP);

    if (!al_init_image_addon())
    {
        error_msg("Falha ao inicializar o addon de imagens");
        return 0;
    }
    som_levelup = al_load_sample("sound/levelup.ogg");
    if (!som_levelup)
    {
        error_msg("Audio nao carregado");
        return 0;
    }
    som_dead = al_load_sample("sound/dead.ogg");
    if (!som_levelup)
    {
        error_msg("Audio nao carregado");
        return 0;
    }
    if (!al_init_font_addon())
    {
        error_msg("Falha ao inicializar o addon de fonte");
    }
    if (!al_init_ttf_addon())
    {
        error_msg("Falha ao inicializar add-on allegro_ttf");
        return -1;
    }
    timer = al_create_timer(1 / FPS);
    if (!timer)
    {
        error_msg("Falha ao criar temporizador");
        return 0;
    }
    timer_anim = al_create_timer(1 / FPS_ANIM);
    if (!timer)
    {
        error_msg("Falha ao criar temporizador");
        return 0;
    }

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela)
    {
        error_msg("Falha ao criar janela");
        al_destroy_timer(timer);
        return 0;
    }
    al_set_window_title(janela, "Flappy Bird 0.1");
    // Torna apto o uso de mouse na aplicação
    if (!al_install_mouse())
    {
        error_msg("Falha ao inicializar o mouse");
        al_destroy_display(janela);
        return -1;
    }
    // Atribui o cursor padrão do sistema para ser usado
    if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
    {
        error_msg("Falha ao atribuir ponteiro do mouse");
        al_destroy_display(janela);
        return -1;
    }

    fila_eventos = al_create_event_queue();
    if (!fila_eventos)
    {
        error_msg("Falha ao criar fila de eventos");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        return 0;
    }
    fila_eventos_anim = al_create_event_queue();
    if (!fila_eventos_anim)
    {
        error_msg("Falha ao criar fila de eventos");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        return 0;
    }

    //carrega a folha de sprites na variavel

    logo = al_load_bitmap("images/logo.bmp");
    if (!logo)
    {
        error_msg("Falha ao carregar sprites");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        return 0;
    }
    background = al_load_bitmap("images/background.bmp");
    if (!background)
    {
        error_msg("Falha ao carregar sprites");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        return 0;
    }
    i_floor = al_load_bitmap("images/ground.png");
    if (!i_floor)
    {
        error_msg("Falha ao carregar sprites");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        return 0;
    }
    button_play = al_load_bitmap("images/button_play.png");
    if (!button_play)
    {
        error_msg("Falha ao carregar sprites");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        return 0;
    }
    pre_menu = al_load_bitmap("images/pre_menu.png");
    if (!pre_menu)
    {
        error_msg("Falha ao carregar sprites");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        return 0;
    }
    bird_sprites = al_load_bitmap("images/birds.png");
    if (!bird_sprites)
    {
        error_msg("Falha ao carregar sprites");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        return 0;
    }
    tunnel_up = al_load_bitmap("images/tunnel_up.png");
    if (!tunnel_up)
    {
        error_msg("Falha ao carregar tunnel up");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        return 0;
    }
    tunnel_down = al_load_bitmap("images/tunnel_down.png");
    if (!tunnel_down)
    {
        error_msg("Falha ao carregar tunnel down");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        return 0;
    }
    scoreboard = al_load_font("images/flappyfont.ttf", 28, 0);
    if (!scoreboard)
    {
        al_destroy_display(janela);
        error_msg("Falha ao carregar fonte");
        return -1;
    }
    gameover = al_load_bitmap("images/gameover.png");
    if (!gameover)
    {
        al_destroy_display(janela);
        error_msg("Falha ao carregar gameover");
        return -1;
    }
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos_anim, al_get_timer_event_source(timer_anim));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_start_timer(timer);
    al_start_timer(timer_anim);

    init_tunnels();
    return 1;
}
void drawPlayer(int mode)
{
    GAME.desenha = 1;
    al_draw_bitmap_region(bird_sprites, BIRD_MODES[mode][0], BIRD_MODES[mode][1], BIRD_MODES[mode][2], BIRD_MODES[mode][3],
                          PLAYER.x, PLAYER.y + 22.5, 0);
}
void drawTunnels()
{
    // GAME.speed = 0;
    for (int i = 0; i < (GAME.currentLevel * 10); i++)
    {
        al_draw_bitmap(tunnel_up, TUNNELS[i].x, TUNNELS[i].y, 0);

        al_draw_bitmap(tunnel_down, TUNNELS[i].x, TUNNELS[i].y + 700, 0);

        // al_draw_rectangle(TUNNELS[i].x, TUNNELS[i].y + 700, TUNNELS[i].x + 100, TUNNELS[i].y + 700 + 614,
        //                   al_map_rgb(255, 0, 255), 1);
        // al_draw_rectangle(TUNNELS[i].x, TUNNELS[i].y, TUNNELS[i].x + 100, TUNNELS[i].y + 700,
        //                   al_map_rgb(255, 0, 255), 1);
    }
    GAME.desenha = 1;
}
void drawPoints()
{
    int primeira = (PLAYER.x + 53 >= TUNNELS[0].x) ? 1 : 0;
    int segunda = (PLAYER.x <= TUNNELS[0].x + 94) ? 1 : 0;

    // char points[200];

    al_draw_textf(scoreboard, al_map_rgb(0, 0, 0), 10, 10, ALLEGRO_ALIGN_LEFT, "Score: %d", PLAYER.score);
    // al_draw_textf(scoreboard, al_map_rgb(0, 0, 0), 10, 30, ALLEGRO_ALIGN_LEFT, "S: %d", segunda);
    // al_draw_textf(scoreboard, al_map_rgb(0, 0, 0), 10, 60, ALLEGRO_ALIGN_LEFT, "P: %d", primeira);
    // // al_draw_rectangle(PLAYER.x + 13, PLAYER.y + 22.5, PLAYER.x + 66, PLAYER.y + 67.5,
    // //   al_map_rgb(0, 0, 255), 1);
    al_draw_textf(scoreboard, al_map_rgb(0, 0, 0), LARGURA_TELA - 20, 10, ALLEGRO_ALIGN_RIGHT, "Level: %d", GAME.currentLevel);
    // al_draw_textf(scoreboard, al_map_rgb(0, 0, 0), LARGURA_TELA - 20, 40, ALLEGRO_ALIGN_RIGHT, "Player X: %d", PLAYER.x + 64);
    // al_draw_textf(scoreboard, al_map_rgb(0, 0, 0), LARGURA_TELA - 20, 70, ALLEGRO_ALIGN_RIGHT, "Tunnel 0 X: %d", TUNNELS[0].x);
    // al_draw_textf(scoreboard, al_map_rgb(0, 0, 0), LARGURA_TELA - 20, 100, ALLEGRO_ALIGN_RIGHT, "Tunnel 0 Y: %d", TUNNELS[0].y);
}
void reset_game()
{
    GAME.currentLevel = 1;
    GAME.currentState = ESTADO_PRE_MENU;
    PLAYER.score = 0;
    init_tunnels();
}

void init_tunnels()
{ //+700 ENTRE CIMA E BAIXO
    //MIN: -500
    //MAX: 500

    TUNNELS = realloc(TUNNELS, GAME.currentLevel * 10 * sizeof(TUNNELS_VARIABLES));
    const int maximum_number = -200;
    const int minimum_number = -500;
    TUNNELS[0].index = 0;
    TUNNELS[0].x = 650;
    TUNNELS[0].y = (rand() % (maximum_number + 1 - minimum_number)) + minimum_number;
    for (int i = 1; i < (GAME.currentLevel * 10); i++)
    {
        TUNNELS[i].index = i;
        TUNNELS[i].x = TUNNELS[i - 1].x + (rand() % (250 + 1 - 200)) + 250;
        TUNNELS[i].y = (rand() % (maximum_number + 1 - minimum_number)) + minimum_number;
        ;
    }
}

void init_game()
{

    GAME.cx1 -= GAME.speed;
    GAME.cx2 -= GAME.speed;
    if (GAME.cx1 <= -480)
    {
        GAME.cx1 = 0;
        GAME.cx2 = 480;
    }
}

void draw()
{
    if (GAME.desenha)
    {

        al_draw_bitmap(background, 0, -80, 0);
        GAME.desenha = 0;
    }
    if (GAME.currentState == ESTADO_INICIAL)
    {
        al_draw_scaled_bitmap(logo,
                              0, 0,
                              95, 28,
                              (LARGURA_TELA / 2) - 90, (ALTURA_TELA / 4),
                              180, 58, 0);
        al_draw_scaled_bitmap(button_play,
                              0, 0,
                              366, 204,
                              (LARGURA_TELA / 2) - 60, (ALTURA_TELA / 4) + 100,
                              120, 60, 0);
    }
    else if (GAME.currentState == ESTADO_PRE_MENU)
    {
        al_draw_scaled_bitmap(pre_menu,
                              0, 0,
                              370, 320,
                              (LARGURA_TELA / 2) - 90, (ALTURA_TELA / 2),
                              200, 180, 0);
    }
    else if (GAME.currentState == ESTADO_JOGANDO)
    {
        drawPlayer(PLAYER.playerState);

        drawTunnels();
        drawPoints();
    }
    else if (GAME.currentState == ESTADO_PERDEU)
    {
        al_draw_bitmap(gameover,

                       (LARGURA_TELA / 2) - 161, (ALTURA_TELA / 2) - 88,
                       0);

        al_draw_scaled_bitmap(button_play,
                              0, 0,
                              366, 204,
                              (LARGURA_TELA / 2) - 60, (ALTURA_TELA / 3) + 200,
                              120, 60, 0);
        drawPoints();

        GAME.desenha = 1;
    }

    al_draw_bitmap(i_floor,
                   GAME.cx1, (ALTURA_TELA - 100),
                   0);
    al_draw_bitmap(i_floor,
                   GAME.cx2, (ALTURA_TELA - 100),
                   0);

    al_flip_display();
}
void events()
{
    ALLEGRO_EVENT evento;

    ALLEGRO_EVENT evento_anim;
    while (!al_is_event_queue_empty(fila_eventos))
    {
        al_wait_for_event(fila_eventos, &evento);
        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            GAME.sair = 1;
            printf("Saiu");
        }
        if (GAME.currentState == ESTADO_PERDEU)
        {
            if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
            {

                if (evento.mouse.x >= (LARGURA_TELA / 2) - 60 &&
                    evento.mouse.x <= LARGURA_TELA / 2 - 60 + 120 &&
                    evento.mouse.y >= (ALTURA_TELA / 3) + 200 &&
                    evento.mouse.y <= (ALTURA_TELA / 3) + 200 + 60)
                {
                    al_set_system_mouse_cursor(janela,
                                               ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
                }
                else
                {
                    al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
                }
            }
            if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (evento.mouse.x >= (LARGURA_TELA / 2) - 60 &&
                    evento.mouse.x <= LARGURA_TELA / 2 - 60 + 120 &&
                    evento.mouse.y >= (ALTURA_TELA / 3) + 200 &&
                    evento.mouse.y <= (ALTURA_TELA / 3) + 200 + 60)
                {
                    reset_game();
                    GAME.currentState = ESTADO_INICIAL;
                    GAME.speed = 0;
                }
            }
        }
        if (GAME.currentState == ESTADO_INICIAL)
        {
            GAME.desenha = 1;
            if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
            {

                if (evento.mouse.x >= (LARGURA_TELA / 2) - 60 &&
                    evento.mouse.x <= LARGURA_TELA / 2 - 60 + 120 &&
                    evento.mouse.y >= (ALTURA_TELA / 4) + 100 &&
                    evento.mouse.y <= (ALTURA_TELA / 4) + 100 + 60)
                {
                    al_set_system_mouse_cursor(janela,
                                               ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
                }
                else
                {
                    al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
                }
            }

            if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (evento.mouse.x >= (LARGURA_TELA / 2) - 60 &&
                    evento.mouse.x <= LARGURA_TELA / 2 - 60 + 120 &&
                    evento.mouse.y >= (ALTURA_TELA / 4) + 100 &&
                    evento.mouse.y <= (ALTURA_TELA / 4) + 100 + 60)
                {
                    GAME.desenha = 1;
                    GAME.currentState = ESTADO_PRE_MENU;
                    GAME.speed = 0;
                }
            }
        }
        else if (GAME.currentState == ESTADO_PRE_MENU)
        {

            if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                GAME.desenha = 1;
                GAME.currentState = ESTADO_JOGANDO;
                GAME.speed = DEFAULT_SPEED;
                PLAYER.y = evento.mouse.y - 45;
                PLAYER.x = evento.mouse.x - 32;
            }
        }
        if (GAME.currentState == ESTADO_JOGANDO)
        {
            for (int i = 0; i < (GAME.currentLevel * 10); i++)
            {
                if (PLAYER.x + 54 >= TUNNELS[i].x &&
                    PLAYER.x <= TUNNELS[i].x + 100)
                {
                    PLAYER.score++;
                }

                if ((PLAYER.x + 50 >= TUNNELS[i].x &&
                     PLAYER.x + 10 <= TUNNELS[i].x + 94) &&
                    ((PLAYER.y - 40.5 <= TUNNELS[i].y + 545) ^ (PLAYER.y - 40.5 >= TUNNELS[i].y + 594)))
                {
                    al_play_sample(som_dead, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

                    al_rest(1);
                    GAME.currentState = ESTADO_PERDEU;
                }
            }
            al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
            if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
            {
                PLAYER.y = evento.mouse.y - 45;
                PLAYER.x = evento.mouse.x - 32;
                if (PLAYER.y >= ALTURA_TELA - 140)
                {

                    al_play_sample(som_dead, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

                    al_rest(1);
                    GAME.currentState = ESTADO_PERDEU;
                    GAME.desenha = 1;
                }
            }
        }
    }

    while (!al_is_event_queue_empty(fila_eventos_anim))
    {
        al_wait_for_event(fila_eventos_anim, &evento_anim);
        if (GAME.currentState == ESTADO_JOGANDO)
        {

            if (evento_anim.type == ALLEGRO_EVENT_TIMER)
            {
                if (++PLAYER.playerState > 2)
                {
                    PLAYER.playerState = 0;
                }
            }
        }
    }
    if (GAME.currentState == ESTADO_JOGANDO)
    {
        al_hide_mouse_cursor(janela);
        for (int i = 0; i < GAME.currentLevel * 10; i++)
        {
            TUNNELS[i].x -= GAME.speed;
        }
        if (TUNNELS[(GAME.currentLevel * 10) - 1].x < -182)
        {
            GAME.currentLevel++;
            GAME.speed++;
            if (GAME.speed < NIVEL_MAX_VELOCITY)
            {
                GAME.speed++;
            }
            init_tunnels();
            al_play_sample(som_levelup, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        }
    }
    if (GAME.currentState != ESTADO_JOGANDO)
    {
        al_show_mouse_cursor(janela);
    }
}
int destroyAll()
{
    al_destroy_bitmap(logo);
    al_destroy_bitmap(bird_sprites);
    al_destroy_bitmap(tunnel_down);
    al_destroy_bitmap(tunnel_up);
    al_destroy_bitmap(pre_menu);
    al_destroy_bitmap(background);
    al_destroy_bitmap(button_play);
    al_destroy_timer(timer);
    al_destroy_timer(timer_anim);
    al_destroy_event_queue(fila_eventos);
    al_destroy_sample(som_levelup);
    al_destroy_audio_stream(musica);
    al_destroy_event_queue(fila_eventos_anim);
    free(TUNNELS);
    al_destroy_display(janela);
}
int main(void)
{

    if (!inicializar())
    {
        return -1;
    }

    while (!GAME.sair)
    {
        draw();
        events();
        init_game();
    }
    destroyAll();
    return 0;
}