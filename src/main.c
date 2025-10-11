#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include "card.h"
#include "pile.h"
#include "debug.h"

constexpr int WIDTH = 1240;
constexpr int HEIGHT = 720;

int main(int argc, char** argv) {

	SetTraceLogLevel(LOG_WARNING);
	InitWindow(WIDTH, HEIGHT, "Solitaire");

	Texture2D SpadesAtlas = {};
	LoadTextureCard(&SpadesAtlas, "res/Poker/Top-Down/Cards/Spades-88x124.png");

	constexpr int DeckSize = 52;
	Card Deck[DeckSize] = {};
	for (int i = 0; i < DeckSize; i++) {
		Deck[i] = (Card) {i % 13, false, {0, 0}};
	}

	static Card *Selected = nullptr;

	constexpr int PileSize = 7;
	Pile* Piles[PileSize] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
	int CardIndex = 0;
	for (int pile = 0; pile < PileSize; pile++) {
		for (int pileAmount = 0; pileAmount <= pile; pileAmount++) {
			if (pileAmount == pile)
				Deck[CardIndex].show = true;
			Deck[CardIndex].position = (Vector2) { 80 + (float) pile * 160, 50 + (float) pileAmount * 25};
			AppendCard(&Piles[pile], &Deck[CardIndex]);
			CardIndex++;
		}
	}

	constexpr int StockSize = 24;
	Pile* Stock = {};
	for (int pileAmount = 0; pileAmount < StockSize; pileAmount++) {
		Deck[CardIndex].position = (Vector2) {WIDTH - 200, HEIGHT - 180 - (float) pileAmount * 2};
		AppendCard(&Stock, &Deck[CardIndex]);
		CardIndex++;
	}

	while(!WindowShouldClose()) {

		Debug();

		for (int i = 0; i < DeckSize; i++) {
			if (!Deck[i].show) {
				continue;
			}
			const Rectangle CardRect = {Deck[i].position.x, Deck[i].position.y, CARD_WIDTH, CARD_HEIGHT};
			if (CheckCollisionPointRec(GetMousePosition(), CardRect)) {
				Selected = &Deck[i];
			}
		}


		BeginDrawing();

		ClearBackground((Color) {51, 87, 171, 255});

		for (int i = 0; i < DeckSize; i++){
			DrawCard(SpadesAtlas, Deck[i]);
			if (Selected){
				if (Vector2Equals(Selected->position, Deck[i].position))
					DrawCardBorder(SpadesAtlas, *Selected);
			}
			
		}


		DebugDraw(
			// LINE("%.0f\t%.0f", MousePosition.x, MousePosition.y),
			LINE("%.0f\t%.0f", GetMousePosition().x, GetMousePosition().y)
		);


		EndDrawing();

	}

	UnloadTexture(SpadesAtlas);

	CloseWindow();

	for (int i = 0; i < PileSize; i++) {
		Pile* current = Piles[i];
		while (current != NULL) {
			Pile* next = current->next;
			free(current);
			current = next;
		}
		Piles[i] = nullptr;
	}

	Pile* current = Stock;
	while (current != NULL) {
		Pile* next = current->next;
		free(current);
		current = next;
	}
	Stock = nullptr;


	return 0;
}
