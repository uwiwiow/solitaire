#ifndef CARD_H
#define CARD_H

#include <raylib.h>

enum CardNumber { A = 1, J = 11, Q = 12, K = 13, BACK = 14, BORDER = 15};

typedef struct Card {
	int number;
	bool show;
	Vector2 position;
} Card;

void LoadTextureCard(Texture2D *CardAtlas, char* filename);

void DrawCard(Texture2D atlas, Card card);

void DrawCardBorder(Texture2D atlas, Card card);

#endif
