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

	enum CursorTypes { POINT, OPEN, CLOSE };

	const Texture2D CursorTextures[3] = {
		LoadTexture("res/kenney_cursor-pack/PNG/Outline/Default/hand_thin_point.png"),
		LoadTexture("res/kenney_cursor-pack/PNG/Outline/Default/hand_thin_open.png"),
		LoadTexture("res/kenney_cursor-pack/PNG/Outline/Default/hand_thin_closed.png")
	};

	const Texture2D *Cursor;

	HideCursor();

	constexpr int DeckSize = 52;
	Card Deck[DeckSize] = {};
	for (int i = 0; i < DeckSize; i++) {
		Deck[i] = (Card) {i % 13, false, {0, 0}};
	}

	static Card *SelectedCard = nullptr;
	static Pile *SelectedPile = nullptr;
	bool Holding = false;
	Pile *TempPile = nullptr;

	constexpr int PileSize = 7;
	Pile* Piles[PileSize] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
	int CardIndex = 0;
	for (int pile = 0; pile < PileSize; pile++) {
		for (int pileAmount = 0; pileAmount <= pile; pileAmount++) {
			if (pileAmount == pile)
				Deck[CardIndex].show = true;
			Deck[CardIndex].position = (Vector2) { 80 + (float) pile * 160, 50 + (float) pileAmount * 30};
			AppendCard(&Piles[pile], &Deck[CardIndex]);
			CardIndex++;
		}
	}

	int StockSize = 24;
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
			for (int pile = 0; pile < PileSize; pile++) {
				TempPile = Piles[pile]->prev;

				 do {
					if (!TempPile->card->show) {
						if (TempPile->prev != NULL) {
							TempPile = TempPile->prev;
								continue;
							}
						break;
					}

					const Rectangle CardRect = {
						TempPile->card->position.x, TempPile->card->position.y, CARD_WIDTH, CARD_HEIGHT
					};
					if (CheckCollisionPointRec(MousePosition, CardRect)) {
						SelectedCard = TempPile->card;
						SelectedPile = TempPile;
						pile = PileSize;
						break;
					}
					SelectedCard = nullptr;
				 	SelectedPile = nullptr;

					if (TempPile->prev != NULL)
						TempPile = TempPile->prev;
					else
						break;
				} while (!Vector2Equals(TempPile->card->position,Piles[pile]->prev->card->position));
			}

		if (StockSize)
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(MousePosition, (Rectangle)
				{Stock->card->position.x, Stock->card->position.y, CARD_WIDTH, CARD_HEIGHT})) {
				for (int pile = 0; pile < PileSize; pile++) {
					Card *TempCard = Stock->card;
					TempCard->position.x = Piles[pile]->prev->card->position.x;
					TempCard->position.y = Piles[pile]->prev->card->position.y + 30;
					TempCard->show = true;
					RemoveCard(&Stock, Stock);
					AppendCard(&Piles[pile], TempCard);
					StockSize--;
					if (StockSize == 0) break;
				}
			}

		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && SelectedCard) {
			Holding = true;
			CardOffset = Vector2Subtract(MousePosition, SelectedCard->position);
		}

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && Holding) {
			TempPile = SelectedPile;
			while (TempPile->card != NULL) {
				SelectedCard->position = Vector2Add(Vector2Subtract(SelectedCard->position, CardOffset), Vector2Subtract(MousePosition, MouseOffset));
				if (TempPile->next != NULL)
					TempPile = TempPile->next;
				else
					break;
			}

			MouseOffset = SelectedCard->position;
		}

		if (IsMouseButtonUp(MOUSE_LEFT_BUTTON)) {
			Holding = false;
		}

		if (SelectedCard) {
			Cursor = &CursorTextures[OPEN];
			if (Holding)
				Cursor = &CursorTextures[CLOSE];
		}
		else
			Cursor = &CursorTextures[POINT];



		BeginDrawing();

		ClearBackground((Color) {51, 87, 171, 255});

		if (StockSize) {
			TempPile = Stock;
			while (TempPile->card != NULL) {
				DrawCard(SpadesAtlas, *TempPile->card);
				if (TempPile->next != NULL)
					TempPile = TempPile->next;
				else
					break;
			}
		}

		// draw cards per pile
		for (int pile = 0; pile < PileSize; pile++) {

			// temp pile for getting every card at the loop
			TempPile = Piles[pile];

			// looping through all the cards
			while (TempPile->card != NULL) {
				// checking if a card is holding, then don't draw it
				if (Holding)
					// check if the selected card is the held one
					if (Vector2Equals(TempPile->card->position, SelectedCard->position)) {
						// go next card or if there aren't more cards, then break
						if (TempPile->next != NULL) {
							TempPile = TempPile->next;
							continue;
						}
							break;
					}

				DrawCard(SpadesAtlas, *TempPile->card);

				// draw border if the card is selected
				if (SelectedCard)
					// check if the selected card is the selected one
					if (Vector2Equals(SelectedCard->position, TempPile->card->position))
						DrawCardBorder(SpadesAtlas, *SelectedCard);

				// go next card or if there aren't more cards, then break
				if (TempPile->next != NULL)
					TempPile = TempPile->next;
				else
					break;
			}

		}

		// if there's a held card, then draw it over everything else
		if (Holding) {
			DrawCard(SpadesAtlas, *SelectedCard);
			DrawCardBorder(SpadesAtlas, *SelectedCard);
		}

		DrawTexture(*Cursor, (int) MousePosition.x - Cursor->width/4, (int) MousePosition.y - Cursor->height/4, WHITE);


		DebugDraw(
			LINE("%.0f\t%.0f", CardOffset.x, CardOffset.y),
			LINE("%.0f\t%.0f", GetMousePosition().x, GetMousePosition().y)
		);


		EndDrawing();

	}

	UnloadTexture(SpadesAtlas);

	for (int i = 0; i < 3; i++)
		UnloadTexture(CursorTextures[i]);

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
