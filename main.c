// Os arquivos de cabeçalho
#include <stdio.h>
#include "headers/game.h"
#include "headers/player.h"

//Variaveis Globais
GAME_VARIABLES GAME = {0, 480, DEFAULT_SPEED, 0, 0, 1, 1, 0, 1, 1, 1};
PLAYER_VARIABLES PLAYER = {(LARGURA_TELA / 2), (ALTURA_TELA / 2), 0, 600, 0, 0};

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
    if (!al_reserve_samples(5))
    {
        error_msg("Falha ao reservar amostrar de audio");
        return 0;
    }
    GAME.musica = al_load_audio_stream("sound/trilha.wav", 4, 500);
    if (!GAME.musica)
    {
        error_msg("Audio nao carregado");

        return 0;
    }
    al_attach_audio_stream_to_mixer(GAME.musica, al_get_default_mixer());
    al_set_audio_stream_playmode(GAME.musica, ALLEGRO_PLAYMODE_LOOP);

    if (!al_init_image_addon())
    {
        error_msg("Falha ao inicializar o addon de imagens");
        return 0;
    }
    GAME.som_levelup = al_load_sample("sound/levelup.ogg");
    if (!GAME.som_levelup)
    {
        error_msg("Audio nao carregado");
        return 0;
    }
    GAME.som_dead = al_load_sample("sound/dead.ogg");
    if (!GAME.som_dead)
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
    GAME.timer = al_create_timer(1 / FPS);
    if (!GAME.timer)
    {
        error_msg("Falha ao criar temporizador");
        return 0;
    }
    GAME.timer_anim = al_create_timer(1 / FPS_ANIM);
    if (!GAME.timer)
    {
        error_msg("Falha ao criar temporizador");
        return 0;
    }

    GAME.janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!GAME.janela)
    {
        error_msg("Falha ao criar GAME.janela");
        al_destroy_timer(GAME.timer);
        return 0;
    }
    al_set_window_title(GAME.janela, "Flappy Bird 0.1");
    // Torna apto o uso de mouse na aplicação
    if (!al_install_mouse())
    {
        error_msg("Falha ao inicializar o mouse");
        al_destroy_display(GAME.janela);
        return -1;
    }
    // Atribui o cursor padrão do sistema para ser usado
    if (!al_set_system_mouse_cursor(GAME.janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
    {
        error_msg("Falha ao atribuir ponteiro do mouse");
        al_destroy_display(GAME.janela);
        return -1;
    }

    GAME.fila_eventos = al_create_event_queue();
    if (!GAME.fila_eventos)
    {
        error_msg("Falha ao criar fila de eventos");
        al_destroy_timer(GAME.timer);
        al_destroy_display(GAME.janela);
        return 0;
    }
    GAME.fila_eventos_anim = al_create_event_queue();
    if (!GAME.fila_eventos_anim)
    {
        error_msg("Falha ao criar fila de eventos");
        al_destroy_timer(GAME.timer);
        al_destroy_display(GAME.janela);
        return 0;
    }

    //carrega a folha de sprites na variavel

    GAME.logo = al_load_bitmap("images/logo.bmp");
    if (!GAME.logo)
    {
        error_msg("Falha ao carregar sprites");
        al_destroy_timer(GAME.timer);
        al_destroy_display(GAME.janela);
        al_destroy_event_queue(GAME.fila_eventos);
        return 0;
    }
    GAME.background = al_load_bitmap("images/background.bmp");
    if (!GAME.background)
    {
        error_msg("Falha ao carregar background");
        al_destroy_timer(GAME.timer);
        al_destroy_display(GAME.janela);
        al_destroy_event_queue(GAME.fila_eventos);
        return 0;
    }
    GAME.i_floor = al_load_bitmap("images/ground.png");
    if (!GAME.i_floor)
    {
        error_msg("Falha ao carregar ground");
        al_destroy_timer(GAME.timer);
        al_destroy_display(GAME.janela);
        al_destroy_event_queue(GAME.fila_eventos);
        return 0;
    }
    GAME.button_play = al_load_bitmap("images/button_play.png");
    if (!GAME.button_play)
    {
        error_msg("Falha ao carregar button_play");
        al_destroy_timer(GAME.timer);
        al_destroy_display(GAME.janela);
        al_destroy_event_queue(GAME.fila_eventos);
        return 0;
    }
    GAME.pre_menu = al_load_bitmap("images/pre_menu.png");
    if (!GAME.pre_menu)
    {
        error_msg("Falha ao carregar pre-menu");
        al_destroy_timer(GAME.timer);
        al_destroy_display(GAME.janela);
        al_destroy_event_queue(GAME.fila_eventos);
        return 0;
    }
    PLAYER.bird_sprites = al_load_bitmap("images/birds.png");
    if (!PLAYER.bird_sprites)
    {
        error_msg("Falha ao carregar sprites");
        al_destroy_timer(GAME.timer);
        al_destroy_display(GAME.janela);
        al_destroy_event_queue(GAME.fila_eventos);
        return 0;
    }
    GAME.tunnel_up = al_load_bitmap("images/tunnel_up.png");
    if (!GAME.tunnel_up)
    {
        error_msg("Falha ao carregar tunnel up");
        al_destroy_timer(GAME.timer);
        al_destroy_display(GAME.janela);
        al_destroy_event_queue(GAME.fila_eventos);
        return 0;
    }
    GAME.tunnel_down = al_load_bitmap("images/tunnel_down.png");
    if (!GAME.tunnel_down)
    {
        error_msg("Falha ao carregar tunnel down");
        al_destroy_timer(GAME.timer);
        al_destroy_display(GAME.janela);
        al_destroy_event_queue(GAME.fila_eventos);
        return 0;
    }
    GAME.scoreboard = al_load_font("images/flappyfont.ttf", 28, 0);
    if (!GAME.scoreboard)
    {
        al_destroy_display(GAME.janela);
        error_msg("Falha ao carregar fonte");
        return -1;
    }
    GAME.gameover = al_load_bitmap("images/gameover.png");
    if (!GAME.gameover)
    {
        al_destroy_display(GAME.janela);
        error_msg("Falha ao carregar gameover");
        return -1;
    }
    al_register_event_source(GAME.fila_eventos, al_get_display_event_source(GAME.janela));
    al_register_event_source(GAME.fila_eventos, al_get_timer_event_source(GAME.timer));
    al_register_event_source(GAME.fila_eventos_anim, al_get_timer_event_source(GAME.timer_anim));
    al_register_event_source(GAME.fila_eventos, al_get_mouse_event_source());
    al_start_timer(GAME.timer);
    al_start_timer(GAME.timer_anim);
    onPlayerInitGame(&GAME, &PLAYER);
    return 1;
}

void draw()
{

    al_draw_bitmap(GAME.background, 0, -80, 0);

    if (GAME.currentState == ESTADO_INICIAL)
    {
        al_draw_scaled_bitmap(GAME.logo,
                              0, 0,
                              95, 28,
                              (LARGURA_TELA / 2) - 90, (ALTURA_TELA / 4),
                              180, 58, 0);
        al_draw_scaled_bitmap(GAME.button_play,
                              0, 0,
                              366, 204,
                              (LARGURA_TELA / 2) - 60, (ALTURA_TELA / 4) + 100,
                              120, 60, 0);
    }
    else if (GAME.currentState == ESTADO_PRE_MENU)
    {
        al_draw_scaled_bitmap(GAME.pre_menu,
                              0, 0,
                              370, 320,
                              (LARGURA_TELA / 2) - 90, (ALTURA_TELA / 2),
                              200, 180, 0);
    }
    else if (GAME.currentState == ESTADO_JOGANDO)
    {
        drawPlayer(&GAME, &PLAYER);

        drawTunnels(&GAME);
        drawPoints(&GAME, &PLAYER);
    }
    else if (GAME.currentState == ESTADO_PERDEU)
    {
        al_draw_bitmap(GAME.gameover,

                       (LARGURA_TELA / 2) - 161, (ALTURA_TELA / 2) - 88,
                       0);

        al_draw_scaled_bitmap(GAME.button_play,
                              0, 0,
                              366, 204,
                              (LARGURA_TELA / 2) - 60, (ALTURA_TELA / 3) + 200,
                              120, 60, 0);
        drawPoints(&GAME, &PLAYER);
    }

    al_draw_bitmap(GAME.i_floor,
                   GAME.cx1, (ALTURA_TELA - 100),
                   0);
    al_draw_bitmap(GAME.i_floor,
                   GAME.cx2, (ALTURA_TELA - 100),
                   0);

    al_flip_display();
}
void events()
{
    ALLEGRO_EVENT evento;
    ALLEGRO_EVENT evento_anim;
    while (!al_is_event_queue_empty(GAME.fila_eventos))
    {
        al_wait_for_event(GAME.fila_eventos, &evento);
        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            GAME.sair = 1;
            printf("Saiu");
        }
        else if (GAME.currentState == ESTADO_PERDEU)
        {
            if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
            {

                if (evento.mouse.x >= (LARGURA_TELA / 2) - 60 &&
                    evento.mouse.x <= LARGURA_TELA / 2 - 60 + 120 &&
                    evento.mouse.y >= (ALTURA_TELA / 3) + 200 &&
                    evento.mouse.y <= (ALTURA_TELA / 3) + 200 + 60)
                {
                    al_set_system_mouse_cursor(GAME.janela,
                                               ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
                }
                else
                {
                    al_set_system_mouse_cursor(GAME.janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
                }
            }
            if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (evento.mouse.x >= (LARGURA_TELA / 2) - 60 &&
                    evento.mouse.x <= LARGURA_TELA / 2 - 60 + 120 &&
                    evento.mouse.y >= (ALTURA_TELA / 3) + 200 &&
                    evento.mouse.y <= (ALTURA_TELA / 3) + 200 + 60)
                {
                    onPlayerInitGame(&GAME, &PLAYER);
                }
            }
        }
        else if (GAME.currentState == ESTADO_INICIAL)
        {

            if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
            {

                if (evento.mouse.x >= (LARGURA_TELA / 2) - 60 &&
                    evento.mouse.x <= LARGURA_TELA / 2 - 60 + 120 &&
                    evento.mouse.y >= (ALTURA_TELA / 4) + 100 &&
                    evento.mouse.y <= (ALTURA_TELA / 4) + 100 + 60)
                {
                    al_set_system_mouse_cursor(GAME.janela,
                                               ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
                }
                else
                {
                    al_set_system_mouse_cursor(GAME.janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
                }
            }

            if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (evento.mouse.x >= (LARGURA_TELA / 2) - 60 &&
                    evento.mouse.x <= LARGURA_TELA / 2 - 60 + 120 &&
                    evento.mouse.y >= (ALTURA_TELA / 4) + 100 &&
                    evento.mouse.y <= (ALTURA_TELA / 4) + 100 + 60)
                {
                    onPlayerStartGame(&GAME, &PLAYER);
                }
            }
        }
        else if (GAME.currentState == ESTADO_PRE_MENU)
        {

            if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                onPlayerPlayGame(&GAME, &PLAYER);
                PLAYER.y = evento.mouse.y - 45;
                PLAYER.x = evento.mouse.x - 32;
            }
        }
        if (GAME.currentState == ESTADO_JOGANDO)
        {
            for (int i = 0; i < (GAME.currentLevel * 10); i++)
            {
                if (PLAYER.x + 54 >= GAME.TUNNELS[i].x &&
                    PLAYER.x <= GAME.TUNNELS[i].x + 100)
                {
                    PLAYER.score++;
                }

                if ((PLAYER.x + 50 >= GAME.TUNNELS[i].x &&
                     PLAYER.x + 10 <= GAME.TUNNELS[i].x + 94) &&
                    ((PLAYER.y - 40.5 <= GAME.TUNNELS[i].y + 545) ^ (PLAYER.y - 40.5 >= GAME.TUNNELS[i].y + 594)))
                {
                    al_play_sample(GAME.som_dead, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

                    al_rest(1);
                    GAME.currentState = ESTADO_PERDEU;
                }
            }
            if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
            {
                PLAYER.y = evento.mouse.y - 45;
                PLAYER.x = evento.mouse.x - 32;
                if (PLAYER.y >= ALTURA_TELA - 140)
                {

                    onPlayerLoseGame(&GAME, &PLAYER);
                }
            }
        }
    }

    while (!al_is_event_queue_empty(GAME.fila_eventos_anim))
    {
        al_wait_for_event(GAME.fila_eventos_anim, &evento_anim);
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
        for (int i = 0; i < GAME.currentLevel * 10; i++)
        {
            GAME.TUNNELS[i].x -= GAME.speed;
        }
        if (GAME.TUNNELS[(GAME.currentLevel * 10) - 1].x < -182)
        {
            GAME.currentLevel++;
            GAME.speed++;
            if (GAME.speed < NIVEL_MAX_VELOCITY)
            {
                GAME.speed++;
            }
            recountTunnels(&GAME, &PLAYER);
            al_play_sample(GAME.som_levelup, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        }
    }
}
int destroyAll()
{
    al_destroy_bitmap(GAME.logo);
    al_destroy_bitmap(PLAYER.bird_sprites);
    al_destroy_bitmap(GAME.tunnel_down);
    al_destroy_bitmap(GAME.tunnel_up);
    al_destroy_bitmap(GAME.pre_menu);
    al_destroy_bitmap(GAME.background);
    al_destroy_bitmap(GAME.button_play);
    al_destroy_timer(GAME.timer);
    al_destroy_timer(GAME.timer_anim);
    al_destroy_event_queue(GAME.fila_eventos);
    al_destroy_sample(GAME.som_levelup);
    al_destroy_audio_stream(GAME.musica);
    al_destroy_event_queue(GAME.fila_eventos_anim);
    free(GAME.TUNNELS);
    al_destroy_display(GAME.janela);
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
        GAME.cx1 -= GAME.speed;
        GAME.cx2 -= GAME.speed;
        if (GAME.cx1 <= -480)
        {
            GAME.cx1 = 0;
            GAME.cx2 = 480;
        }
    }
    destroyAll();
    return 0;
}