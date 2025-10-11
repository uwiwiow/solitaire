#ifndef PILE_H
#define PILE_H

#include "card.h"

typedef struct Pile {
	Card *card;
	struct Pile *prev;
	struct Pile *next;
} Pile;

void AppendCard(Pile **head, Card *card);

void RemoveCard(Pile **head, Pile *card);

#endif