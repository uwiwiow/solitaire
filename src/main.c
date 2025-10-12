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
		Deck[i] = (Card) {i % 13, true, 0, {0, 0}};
	}

	static Card *Selected = nullptr;
	bool Holding = false;
	Pile TempPile = {nullptr, nullptr, nullptr};

	constexpr int PileSize = 7;
	Pile* Piles[PileSize] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
	int CardIndex = 0;
	for (int pile = 0; pile < PileSize; pile++) {
		for (int pileAmount = 0; pileAmount <= pile; pileAmount++) {
			if (pileAmount == pile)
				Deck[CardIndex].show = true;
			Deck[CardIndex].position = (Vector2) { 80 + (float) pile * 160, 50 + (float) pileAmount * 25};
			Deck[CardIndex].zindex = (char) pileAmount;
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

	Vector2 MouseOffset = {0, 0};
	Vector2 CardOffset = {0, 0};

	while(!WindowShouldClose()) {

		Debug();

		const Vector2 MousePosition = GetMousePosition();

		// checar solo en la pile en la que este el mouse, ver con ubicacion
		if (!Holding)
			for (int i = 0; i < DeckSize; i++) {
				if (!Deck[i].show)
					continue;

				const Rectangle CardRect = {Deck[i].position.x, Deck[i].position.y, CARD_WIDTH, CARD_HEIGHT};
				if (CheckCollisionPointRec(MousePosition, CardRect)) {
					Selected = &Deck[i];
					break;
				}
				Selected = nullptr;
			}

		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && Selected) {
			Holding = true;
			CardOffset = Vector2Subtract(MousePosition, Selected->position);
		}

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && Holding) {
			Selected->position = Vector2Add(Vector2Subtract(Selected->position, CardOffset), Vector2Subtract(MousePosition, MouseOffset));
			MouseOffset = Selected->position;
		}

		// maybe Release and && Selected
		if (IsMouseButtonUp(MOUSE_LEFT_BUTTON)) {
			Holding = false;
		}


		BeginDrawing();

		ClearBackground((Color) {51, 87, 171, 255});

		TempPile = *Stock;
		while (TempPile.card != NULL) {
			DrawCard(SpadesAtlas, *TempPile.card);
			if (TempPile.next != NULL)
				TempPile = *TempPile.next;
			else
				break;
		}

		// draw cards per pile
		for (int pile = 0; pile < PileSize; pile++) {

			// temp pile for getting every card at the loop
			TempPile = *Piles[pile];

			// looping through all the cards
			while (TempPile.card != NULL) {

				// checking if a card is holding, then don't draw it
				if (Holding)
					// check if the selected card is the held one
					if (Vector2Equals(TempPile.card->position, Selected->position)) {
						// go next card or if there aren't more cards, then break
						if (TempPile.next != NULL) {
							TempPile = *TempPile.next;
							continue;
						}
							break;
					}

				DrawCard(SpadesAtlas, *TempPile.card);

				// draw border if the card is selected
				if (Selected)
					// check if the selected card is the selected one
					if (Vector2Equals(Selected->position, TempPile.card->position))
						DrawCardBorder(SpadesAtlas, *Selected);

				// go next card or if there aren't more cards, then break
				if (TempPile.next != NULL)
					TempPile = *TempPile.next;
				else
					break;

			}
		}

		// if there's a held card, then draw it over everything else
		if (Holding) {
			DrawCard(SpadesAtlas, *Selected);
			DrawCardBorder(SpadesAtlas, *Selected);
		}


		DebugDraw(
			LINE("%.0f\t%.0f", CardOffset.x, CardOffset.y),
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
