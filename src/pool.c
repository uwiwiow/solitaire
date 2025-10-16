#include "pool.h"

void SetPositionCardFromPool(const Pool* pool, const Pool* winPools) {

    if (*pool->pile == nullptr) return;

    // temp pile for getting every card at the loop
    const Pile *TempPile = *pool->pile;

    bool possibleWin = true;
    int CardNumber = 12;
    const Pile *winPile = nullptr;

    // looping through all the cards
    while (TempPile->card != nullptr) {

        // if not head
        if (TempPile->prev->next != nullptr)
            TempPile->card->position = (Vector2) {TempPile->prev->card->position.x, TempPile->prev->card->position.y + (float) pool->gap};
        else
            TempPile->card->position = pool->position;

        if (TempPile->card->number == K && TempPile->card->show) {
            possibleWin = true;
            winPile = TempPile;
            CardNumber = 12;
        }

        if (TempPile->card->number == CardNumber) {
            CardNumber--;
        }
        else possibleWin = false;


        // go next card or if there aren't more cards, then break
        if (TempPile->next != nullptr)
            TempPile = TempPile->next;
        else
            break;
    }

    if (CardNumber == -1 && possibleWin) {
        if (winPile == nullptr) return;
        if (winPile->card == nullptr) return;
        if (winPools == nullptr) return;

        int winPool = 0;
        for (int i = 0; i < 4; i++) {
            if (winPools[i].pile == nullptr) {
                winPool = i;
                break;
            }
        }

        MoveCardsToPile(pool, winPile->card, &winPools[winPool]);
    }

}

void MoveCardsToPile(const Pool *selectedPool, const  Card *selectedCard, const  Pool *newPool) {
    if (selectedPool == nullptr || selectedCard == nullptr || newPool == nullptr)
        return;

    Pile *pile = *selectedPool->pile;

    while (pile->card != nullptr) {
        if (pile->card == selectedCard) break;
        pile = pile->next;
    }

    pile->prev->card->show = true;

    bool run = true;
    while (run) {
        Pile *tempPile = pile;
        Card *tempCard = tempPile->card;
        if (pile->next == nullptr) run = false;
        else pile = pile->next;
        RemovePile(selectedPool->pile, tempPile);
        tempPile = nullptr;
        AppendCardToPile(newPool->pile, tempCard);
        if (pile == nullptr) break;
    }
}
