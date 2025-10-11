#include <raylib.h>
#include <raymath.h>
#include "card.h"
#include "debug.h"
#include <stdio.h>

constexpr int WIDTH = 900;
constexpr int HEIGHT = 600;

void AddCardToPile(Pile *pile, Card *card, int *CardIndex, Card *Deck) {
	if (pile->card) {
		AddCardToPile(pile, card, CardIndex, Deck);
	}
	
}

int main(int argc, char** argv) {

	SetTraceLogLevel(LOG_WARNING);
	InitWindow(WIDTH, HEIGHT, "Solitaire");

	Texture2D SpadesAtlas = {};
	LoadTextureCard(&SpadesAtlas, "res/Poker/Top-Down/Cards/Spades-88x124.png");

	const int DeckSize = 52;
	Card Deck[DeckSize] = {};
	for (int i = 0; i < DeckSize; i++) {
		Deck[i] = (Card) {i % 13, true, {0, 0}};
	}

	const int PileSize = 7;
	Pile Piles[PileSize] = {};
	int CardIndex = 0;
	for (int pile = 0; pile < PileSize; pile++) {
		for (int pileAmount = 0; pileAmount <= pile; pileAmount++) {
			Piles[pile] = (Pile){.card = &Deck[CardIndex], .next = nullptr};
			Piles[pile].card->position = (Vector2) {50 + pile * 120, 50 + pileAmount * 50};
			CardIndex++;
			if (pile > 0) {
				Piles[pile - 1].next = Piles[pile].card;
			}
		}
	}

	// const int StockSize = 24;
	// Pile Stock = {};
	// for (int pileAmount = 0; pileAmount < StockSize; pileAmount++) {
	// 	Stock = (Pile){.card = &Deck[CardIndex], .next = nullptr};
	// 	Piles[pile].card->position = (Vector2) {50 + pile * 120, 50 + pileAmount * 50};
	// 	CardIndex++;
	// 	if (pile > 0) {
	// 		Piles[pile - 1].next = Piles[pile].card;
	// 	}
	// }

	while(!WindowShouldClose()) {

		Debug();


		BeginDrawing();

		ClearBackground((Color) {51, 87, 171, 255});

		for (int i = 0; i < DeckSize; i++){
			DrawCard(SpadesAtlas, Deck[i]);
			// if (Selected){
			// 	if (Vector2Equals(Selected->position, Deck[i].position))
			// 		DrawCardBorder(SpadesAtlas, *Selected);
			// }
			
		}


		DebugDraw(
			// LINE("%.0f\t%.0f", MousePosition.x, MousePosition.y),
			LINE("%.0f\t%.0f", GetMousePosition().x, GetMousePosition().y)
		);


		EndDrawing();

	}

	UnloadTexture(SpadesAtlas);

	CloseWindow();


	return 0;
}
