#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <sys/random.h>


#include "./core.h"


#define COLLISION_PERCENT 0.1f
#define BALL_SZ 0.03f
#define BALL_THICK 0.005f

#define FLASH_DURATION 0.2f
#define FLASH_INTERVAL 1.3f
#define FLASH_REPS 30

#define RESET_REPS 15
#define RESET_LAP_DURATION 2
#define RESET_LAP_WAIT 0.5

#define BNF_REPS 15
#define BNF_DURATION 2.0f

#define FONT_SPACING 10


Game generic_init(u32 win_width, u32 win_height)
{
    Game game = {0};
    game.win_width = win_width;
    game.win_height = win_height;

    game.state = PAUSED;
    game.frame = 0;
    return game;
}

Game reset_init(u32 win_width, u32 win_height)
{
    Game game = generic_init(win_width, win_height);

    game.ball[0] = (Ball) {
        .x = 0.5f * win_width,
        .y = COLLISION_PERCENT * win_height,
        .radius = BALL_SZ * MIN(win_width, win_height),
        .vx = 0.0f,
        .vy = ((win_height / RESET_LAP_DURATION) / FPS) * (1 - 2 * COLLISION_PERCENT),
        .is_visable = 1,
    };

    game.reps = RESET_REPS;
    return game;
}

void reset_update(Game *game)
{
    if (game->reps <= 0) {
        game->state = DONE;
        return;
    }

    if (game->frame % FPS * (RESET_LAP_DURATION + RESET_LAP_WAIT) == 0) {
        if (game->ball[0].vy == 0) {
            game->ball[0].vy = ((game->win_height / RESET_LAP_DURATION) / FPS) * (1 - 2 * COLLISION_PERCENT);
        }

    }

    Ball *ball = game->ball;
    if (ball->x + ball->vx + ball->radius  <= game->win_width * COLLISION_PERCENT ||
            ball->x +ball->vx >= game->win_width * (1 - COLLISION_PERCENT)) {
        ball->x = game->win_width * 0.1;
        ball->vx = 0;
        --game->reps;
    }

    if (ball->y + ball->vy + ball->radius <= game->win_height * COLLISION_PERCENT ||
            ball->y +ball->vy >= game->win_height * (1 - COLLISION_PERCENT)) {
        ball->y = game->win_height * 0.1;
        ball->vy = 0;
        --game->reps;
    }

    ball->x += ball->vx;
    ball->y += ball->vy;
}

void ball_render(Ball ball, Color color)
{
    DrawCircle(ball.x, ball.y, ball.radius, color);
}

void ball_render_cross(Ball ball, Color color, u32 width, u32 height)
{
    ball_render(ball, color);
    float thick = BALL_THICK * MIN(width, height);

    Vector2 left = { ball.x - ball.radius, ball.y };
    Vector2 right = { ball.x + ball.radius, ball.y };
    DrawLineEx(left, right, thick, COL_AQUA);

    Vector2 top = { ball.x, ball.y - ball.radius };
    Vector2 bottom = { ball.x, ball.y + ball.radius };
    DrawLineEx(top, bottom, thick, COL_AQUA);
}

Game back_and_forth_init(u32 win_width, u32 win_height)
{
    Game game = generic_init(win_width, win_height);

    game.ball[0] = (Ball) {
        .x = 0.5f * win_width,
        .y = 0.5f * win_height,
        .radius = BALL_SZ * MIN(win_width, win_height),
        .vx = (win_width / BNF_DURATION) / FPS,
        .vy = 0.0f,
        .is_visable = 1,
    };

    game.reps = BNF_REPS;
    return game;
}

Game up_and_down_init(u32 win_width, u32 win_height)
{
    Game game = generic_init(win_width, win_height);

    game.ball[0] = (Ball) {
        .x = 0.5f * win_width,
        .y = 0.5f * win_height,
        .radius = BALL_SZ * MIN(win_width, win_height),
        .vx = 0.0f,
        .vy = (win_height / BNF_DURATION) / FPS,
        .is_visable = 1,
    };
    game.reps = BNF_REPS;
    return game;
}

void back_and_forth_update(Game *game)
{
    if (game->reps <= 0) {
        game->state = DONE;
        return;
    }
    Ball *ball = game->ball;
    if (ball->x + ball->vx + ball->radius  <= game->win_width * COLLISION_PERCENT ||
            ball->x +ball->vx >= game->win_width * (1 - COLLISION_PERCENT)) {
        ball->vx *= -1;
        --game->reps;
    }

    if (ball->y + ball->vy + ball->radius <= game->win_height * COLLISION_PERCENT ||
            ball->y +ball->vy >= game->win_height * (1 - COLLISION_PERCENT)) {
        ball->vy *= -1;
        --game->reps;
    }

    ball->x += ball->vx;
    ball->y += ball->vy;
}

Game flash_init_hz(u32 win_width, u32 win_height)
{
    Game game = generic_init(win_width, win_height);

    game.ball[0] = (Ball) {
        .x = 0.1f * win_width,
        .y = 0.5f * win_height,
        .radius = BALL_SZ * MIN(win_width, win_height),
        .vx = 0.0f,
        .vy = 0.0f,
        .is_visable = 1,
    };

    game.ball[1] = game.ball[0];
    game.ball[1].x = 0.9f * win_width;

    game.reps = BNF_REPS;
    return game;
}

Game flash_init_vt(u32 win_width, u32 win_height)
{
    Game game = generic_init(win_width, win_height);

    game.ball[0] = (Ball) {
        .x = 0.5f * win_width,
        .y = 0.1f * win_height,
        .radius = BALL_SZ * MIN(win_width, win_height),
        .vx = 0.0f,
        .vy = 0.0f,
        .is_visable = 1,
    };

    game.ball[1] = game.ball[0];
    game.ball[1].y = 0.9f * win_height;

    game.reps = BNF_REPS;
    return game;
}

void flash_update(Game *game)
{
    if (game->frame >= FLASH_REPS * FPS * FLASH_INTERVAL) {
        game->state = DONE;
        return;
    }
    if (game->frame % (size_t) (FLASH_INTERVAL * FPS) == 0 ||
            (i32) (game->frame - (FLASH_DURATION * FPS)) % (i32) (FLASH_INTERVAL * FPS) == 0) {

        if (!game->ball[0].is_visable) {
            game->ball[0].is_visable = !game->ball[0].is_visable;
            return;
        }

        if (!game->ball[1].is_visable) {
            game->ball[1].is_visable = !game->ball[1].is_visable;
            return;
        }

        size_t rand_id = rand() % 2;
        game->ball[rand_id].is_visable = !game->ball[rand_id].is_visable;
    }
}

void generic_render(Game game) {
    ClearBackground(COL_FG);

    size_t i;
    for (i = 0; i < sizeof(game.ball) / sizeof(*game.ball); ++i) {
        if (game.ball[i].is_visable) {
            ball_render_cross(game.ball[i], COL_RED, game.win_width, game.win_height);
        }
    }
}

void flash_render(Game game) {
    ClearBackground(COL_FG);
    DrawCircle(game.win_width / 2, game.win_height / 2, game.ball[0].radius, COL_BG);

    if (game.ball[0].is_visable) {
        ball_render(game.ball[0], COL_RED);
    }
    if (game.ball[1].is_visable) {
        ball_render(game.ball[1], COL_RED);
    }
}


int main(void)
{
    i32 seed;
    getentropy(&seed, sizeof seed);
    srand(seed);

    InitWindow(INIT_WIDTH, INIT_HEIGHT, "Exercise");
    SetTargetFPS(FPS);


    GameMode games[] = {
        {
            .init = reset_init,
            .update = reset_update,
            .render = generic_render,
        },
        {
            .init = back_and_forth_init,
            .update = back_and_forth_update,
            .render = generic_render,
        },
        {
            .init = up_and_down_init,
            .update = back_and_forth_update,
            .render = generic_render,
        },
        {
            .init = flash_init_hz,
            .update = flash_update,
            .render = flash_render,
        },
        {
            .init = flash_init_vt,
            .update = flash_update,
            .render = flash_render,
        },
    };

    size_t game_amount = sizeof(games) / sizeof (*games);

    u32 width = INIT_WIDTH;
    u32 height = INIT_HEIGHT;

    u32 font_size;
    Vector2 center;
    u32 sel = 0;

    while (!WindowShouldClose()) {
        if (IsWindowResized() || games[sel].game.frame == 0) {
            width = GetScreenWidth();
            height = GetScreenHeight();
            font_size = MIN(width, height) / 8;
            center = MeasureTextEx(GetFontDefault(), "PAUSED", font_size, FONT_SPACING);
            center.x = (width / 2) - (center.x / 2);
            center.y = (height / 2) - (center.y / 2);
            games[sel].game = games[sel].init(width, height);
        }

        if (IsKeyPressed(KEY_N)) {
            sel = (sel + 1) % game_amount;
            games[sel].game = games[sel].init(width, height);
        }



        switch (games[sel].game.state) {
            case RUNNING:
                games[sel].update(&games[sel].game);
                if (IsKeyPressed(KEY_SPACE)) {
                    games[sel].game.state = PAUSED;
                }
                break;
            case PAUSED: {
                if (IsKeyPressed(KEY_SPACE)) {
                    games[sel].game.state = RUNNING;
                }
                break;
            }
            case DONE:
                if (IsKeyPressed(KEY_SPACE)) {
                    games[sel].game = games[sel].init(width, height);
                    games[sel].game.state = RUNNING;
                }
                break;
        }

        BeginDrawing(); {
            games[sel].render(games[sel].game);
            switch (games[sel].game.state) {
                case RUNNING:
                    break;
                case PAUSED:
                    if ((games[sel].game.frame / (FPS / 2)) % 2) {
                        DrawTextEx(GetFontDefault(), "PAUSED", center, font_size, FONT_SPACING, COL_PURPLE);
                    }
                    break;
                case DONE:
                    DrawTextEx(GetFontDefault(), "FINISHED", center, font_size, FONT_SPACING, COL_PURPLE);
                    break;
            }
        } EndDrawing();
        ++games[sel].game.frame;
    }

    CloseWindow();
}
