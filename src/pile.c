#include "pile.h"
#include <stdlib.h>

Pile* NewPile(Card *card) {
    Pile* pile = malloc(sizeof(Pile));
    pile->card = card;
    pile->prev = nullptr;
    pile->next = nullptr;
    return pile;
}

void AppendCardToPile(Pile **head, Card *card) {
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

void RemovePile(Pile **head, Pile *card) {
    if (*head == NULL || card == NULL)
        return;

    if (card == *head && card->next == NULL) {
        free(card);
        *head = nullptr;
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


void MoveCardsToPile(Pile **pile, Pile *newPile) {
    if (!newPile || !pile)
        return;

    Pile *temp = *pile;
    while (temp->card != NULL) {
        Card *card = temp->card;
        RemovePile(&temp, temp);
        AppendCardToPile(&newPile, card);
        if (temp == NULL) break;
        if (temp->next == NULL) break;
        temp = temp->next;
    }
    *pile = nullptr;
}
