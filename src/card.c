#include "card.h"


Rectangle GetCardRect(const int index) {
    const int x = (index % CARD_ATLAS_COLS) * CARD_WIDTH;
    const int y = (index / CARD_ATLAS_COLS) * CARD_HEIGHT;
    return (Rectangle){ (float)x, (float)y, (float)CARD_WIDTH, (float)CARD_HEIGHT };
}

void LoadTextureCard(Texture2D *CardTexture, const char* filename, const int Index) {
    Image CardImage = LoadImage(filename);
    ImageCrop(&CardImage, GetCardRect(Index));
    *CardTexture = LoadTextureFromImage(CardImage);
    UnloadImage(CardImage);
}