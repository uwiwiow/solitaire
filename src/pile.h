#ifndef PILE_H
#define PILE_H

#include "card.h"

typedef struct Pile {
	Card *card;
	struct Pile *prev;
	struct Pile *next;
} Pile;

void AppendCardToPile(Pile **head, Card *card);

void RemovePile(Pile **head, Pile *card);

#endif