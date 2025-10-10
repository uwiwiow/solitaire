#include "card.h"

void LoadTextureCard(Texture2D *CardAtlas, char* filename) {
    Image Image = LoadImage(filename);
    ImageColorReplace(&Image, (Color){0, 128, 128, 255}, BLANK);
    *CardAtlas = LoadTextureFromImage(Image);
    UnloadImage(Image);
}

Rectangle GetCardRect(int index) {
    int cols = 5;
    int cardW = 88;
    int cardH = 124;
    int x = (index % cols) * cardW;
    int y = (index / cols) * cardH;
    return (Rectangle){ (float)x, (float)y, (float)cardW, (float)cardH };
}

void DrawCard(Texture2D atlas, Card card) {
    Rectangle src = GetCardRect(card.show? card.number - 1 : BACK - 1);
    Rectangle dest = { card.position.x, card.position.y, src.width, src.height };
    DrawTexturePro(atlas, src, dest, (Vector2){0, 0}, 0.0f, WHITE);
}

void DrawCardBorder(Texture2D atlas, Card card) {
    Rectangle src = GetCardRect(BORDER - 1);
    Rectangle dest = { card.position.x, card.position.y, src.width, src.height };
    DrawTexturePro(atlas, src, dest, (Vector2){0, 0}, 0.0f, WHITE);
}
