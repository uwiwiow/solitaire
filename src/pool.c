#include "pool.h"

void SetPositionCardFromPool(const Pool* Pool) {

    if (Pool->pile == nullptr) return;

    // temp pile for getting every card at the loop
    const Pile *TempPile = *Pool->pile;

    // looping through all the cards
    while (TempPile->card != nullptr) {

        // if not head
        if (TempPile->prev->next != nullptr)
            TempPile->card->position = (Vector2) {TempPile->prev->card->position.x, TempPile->prev->card->position.y + (float) Pool->gap};
        else
            TempPile->card->position = Pool->position;

        // go next card or if there aren't more cards, then break
        if (TempPile->next != nullptr)
            TempPile = TempPile->next;
        else
            break;
    }


}