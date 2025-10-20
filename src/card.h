#ifndef CARD_H
#define CARD_H

#include <raylib.h>

enum CardNumber { A = 0, J = 10, Q, K, BACK, BORDER};

typedef struct Card {
	int number;
	bool show;
	Vector2 position;
} Card;

constexpr int CARD_ATLAS_COLS = 5;
constexpr int CARD_WIDTH = 88;
constexpr int CARD_HEIGHT = 124;

void LoadTextureCard(Texture2D *CardTexture, const char* filename, int Index);

#endif
