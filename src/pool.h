#ifndef POOL_H
#define POOL_H

#include <raylib.h>

#include "pile.h"

constexpr float PADDING_X = 80.0f;
constexpr float PADDING_Y = 50.0f;
constexpr float OFFSET_X = 160.0f;
constexpr float OFFSET_Y = 30.0f;

typedef struct Pool {
    int gap;
    Vector2 position;
    Pile **pile;
} Pool;

void SetPositionCardFromPool(const Pool* Pool);

void MoveCardsToPile(Pool *selectedPool, Card *selectedCard, Pool *newPool);

#endif