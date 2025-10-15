#include "card.h"

void LoadTextureCard(Texture2D *CardAtlas, const char* filename) {
    Image Image = LoadImage(filename);
    ImageColorReplace(&Image, (Color){0, 128, 128, 255}, BLANK);
    *CardAtlas = LoadTextureFromImage(Image);
    UnloadImage(Image);
}

Rectangle GetCardRect(const int index) {
    const int x = (index % CARD_ATLAS_COLS) * CARD_WIDTH;
    const int y = (index / CARD_ATLAS_COLS) * CARD_HEIGHT;
    return (Rectangle){ (float)x, (float)y, (float)CARD_WIDTH, (float)CARD_HEIGHT };
}

void DrawCard(const Texture2D atlas, const Card card) {
    const Rectangle src = GetCardRect(card.show? card.number : BACK);
    const Rectangle dest = { card.position.x, card.position.y, src.width, src.height };
    DrawTexturePro(atlas, src, dest, (Vector2){0, 0}, 0.0f, WHITE);
}

void DrawCardBorder(const Texture2D atlas, const Card card) {
    const Rectangle src = GetCardRect(BORDER);
    const Rectangle dest = { card.position.x, card.position.y, src.width, src.height };
    DrawTexturePro(atlas, src, dest, (Vector2){0, 0}, 0.0f, WHITE);
}
