#ifndef CORE_H
#define CORE_H

#include <stddef.h>

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long int i64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long int u64;

typedef float f32;
typedef double f64;

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define FPS 60
#define INIT_WIDTH 450
#define INIT_HEIGHT 450

// NOTE: reverse rgba
#define COL_BG     *(Color*) (u32[1]) { 0xff282828 }
#define COL_FG     *(Color*) (u32[1]) { 0xffb2dbeb }
#define COL_BLUE   *(Color*) (u32[1]) { 0xff888545 }
#define COL_AQUA   *(Color*) (u32[1]) { 0xff6a9d68 }
#define COL_PURPLE *(Color*) (u32[1]) { 0xff8662b1 }
#define COL_RED    *(Color*) (u32[1]) { 0xff1d24cc }
#define COL_GREEN  *(Color*) (u32[1]) { 0xff1a9798 }
#define COL_YELLOW *(Color*) (u32[1]) { 0xff2199d7 }
#define COL_ORANGE *(Color*) (u32[1]) { 0xff0e5dd6 }

typedef struct {
    u32 radius;
    f32 x;
    f32 y;
    f32 vx;
    f32 vy;
    u32 is_visable;
} Ball;

typedef enum {
    RUNNING = 0,
    PAUSED = 1,
    DONE = 2,
} GameState;

typedef struct {
    u32 win_width;
    u32 win_height;
    Ball ball[2];
    size_t reps;
    GameState state;
    size_t frame;
} Game;

typedef struct {
    Game (*init)(u32, u32);
    void (*update)(Game*);
    void (*render)(Game);
    Game game;
} GameMode;
#endif // CORE_H
