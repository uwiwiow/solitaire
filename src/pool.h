#ifndef POOL_H
#define POOL_H

#include <raylib.h>

#include "pile.h"

typedef struct Pool {
    Vector2 position;
    int gap;
    Pile *pile;
} Pool;

#endif