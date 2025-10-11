#ifndef CARD_H
#define CARD_H

#include <raylib.h>

enum CardNumber { A = 0, J = 10, Q, K, BACK, BORDER};

typedef struct Card {
	int number;
	bool show;
	Vector2 position;
} Card;

typedef struct Pile {
	Card *card;
	Card *next;
} Pile;

void LoadTextureCard(Texture2D *CardAtlas, char* filename);

void DrawCard(Texture2D atlas, Card card);

void DrawCardBorder(Texture2D atlas, Card card);

#endif
