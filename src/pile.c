#include "pile.h"
#include <stdlib.h>

Pile* NewPile(Card *card) {
    Pile* pile = malloc(sizeof(Pile));
    pile->card = card;
    pile->prev = nullptr;
    pile->next = nullptr;
    return pile;
}

void AppendCard(Pile **head, Card *card) {
    Pile *pile = NewPile(card);
    if (*head == NULL) {
        *head = pile;
        pile->prev = pile;
        return;
    }
    Pile *temp = *head;
    while (temp->next != NULL)
        temp = temp->next;

    temp->next = pile;
    pile->prev = temp;
    (*head)->prev = pile;
}

void RemoveCard(Pile **head, Pile *card) {
    if (*head == NULL || card == NULL)
        return;

    if (card == *head && card->next == NULL) {
        free(card);
        *head = NULL;
        return;
    }

    if (card == *head) {
        *head = card->next;
    }

    if (card->prev != NULL && card->prev->next != NULL)
        card->prev->next = card->next;

    if (card->next != NULL)
        card->next->prev = card->prev;

    if ((*head)->prev == card)
        (*head)->prev = card->prev ? card->prev : *head;

    free(card);
}
