//
// Created by uwiwiow on 6/8/25.
//

#ifndef DEBUG_H
#define DEBUG_H

#include <raylib.h>

bool debug = 1;

// change debug visibility, place in loop
#define Debug()                                 \
do {                                            \
    if (IsKeyPressed(KEY_F1)) debug = !debug;   \
} while (0)

// text formatter for DebugDraw -> F("%d", 1)
#define LINE(...) TextFormat(__VA_ARGS__)

/*
place inside draw scope, when canvas is set up
example ->
DebugDraw(
    LINE("%.3f\t%.3f", mv.x, mv.y),
    LINE("%.3f\t%.3f", rect.x, rect.y)
);
 */
#define DebugDraw(...)                                      \
do {                                                        \
    if (debug) {                                            \
        DrawFPS(10, 10);                                    \
        const char* texts[] = { __VA_ARGS__ };              \
        int count = sizeof(texts) / sizeof(texts[0]);       \
        for (int i = 0; i < count; i++) {                   \
            DrawText(texts[i], 10, 30 + i * 20, 20, DARKGREEN); \
        }                                                   \
    }                                                       \
} while(0)

#endif //DEBUG_H
