#include "card.h"

void LoadTextureCard(Texture2D *CardAtlas, char* filename) {
    Image Image = LoadImage(filename);
    ImageColorReplace(&Image, (Color){0, 128, 128, 255}, BLANK);
    *CardAtlas = LoadTextureFromImage(Image);
    UnloadImage(Image);
}

Rectangle GetCardRect(int index) {
    int x = (index % CARD_ATLAS_COLS) * CARD_WIDTH;
    int y = (index / CARD_ATLAS_COLS) * CARD_HEIGHT;
    return (Rectangle){ (float)x, (float)y, (float)CARD_WIDTH, (float)CARD_HEIGHT };
}

void DrawCard(Texture2D atlas, Card card) {
    Rectangle src = GetCardRect(card.show? card.number : BACK);
    Rectangle dest = { card.position.x, card.position.y, src.width, src.height };
    DrawTexturePro(atlas, src, dest, (Vector2){0, 0}, 0.0f, WHITE);
}

void DrawCardBorder(Texture2D atlas, Card card) {
    Rectangle src = GetCardRect(BORDER);
    Rectangle dest = { card.position.x, card.position.y, src.width, src.height };
    DrawTexturePro(atlas, src, dest, (Vector2){0, 0}, 0.0f, WHITE);
}
