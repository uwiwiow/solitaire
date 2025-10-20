#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <time.h>
#include "card.h"
#include "pile.h"
#include "pool.h"
#include "debug.h"

constexpr int SCREEN_WIDTH = 1240;
constexpr int SCREEN_HEIGHT = 720;



int main() {

	// init
	SetTraceLogLevel(LOG_WARNING);
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Solitaire");



	// textures
	constexpr int CardTexturesCount = 15;
	Texture2D SpadeTextures[CardTexturesCount] = {};
	for (int i = 0; i < CardTexturesCount; i++) {
		LoadTextureCard(&SpadeTextures[i], "res/Poker/Spades-88x124.png", i);
	}

	const Texture2D CursorTextures[3] = {
		LoadTexture("res/kenney_cursor-pack/hand_thin_point.png"),
		LoadTexture("res/kenney_cursor-pack/hand_thin_open.png"),
		LoadTexture("res/kenney_cursor-pack/hand_thin_closed.png")
	};



	// cursor
	enum CursorTypes { POINT, OPEN, CLOSE };
	const Texture2D *Cursor;
	HideCursor();



	// deck
	constexpr int DeckSize = 52;
	Card Deck[DeckSize] = {};
	for (int i = 0; i < DeckSize; i++) {
		Deck[i] = (Card) {i % 13, false, {0, 0}};
	}

	const Card *SelectedCard = nullptr;
	bool Holding = false;

	srand(time(nullptr));

	for (int i = DeckSize - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		Card temp = Deck[i];
		Deck[i] = Deck[j];
		Deck[j] = temp;
	}



	// pile
	const Pile *SelectedPile = nullptr;
	const Pile *TempPile;

	constexpr int PileSize = 7;
	Pile* Piles[PileSize] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
	int CardIndex = 0;
	for (int pile = 0; pile < PileSize; pile++) {
		for (int pileAmount = 0; pileAmount <= pile; pileAmount++) {
			if (pileAmount == pile)
				Deck[CardIndex].show = true;
			AppendCardToPile(&Piles[pile], &Deck[CardIndex]);
			CardIndex++;
		}
	}



	// stock
	int StockSize = 24;
	Pile* Stock = {};
	for (int pileAmount = 0; pileAmount < StockSize; pileAmount++) {
		Deck[CardIndex].position = (Vector2) {SCREEN_WIDTH - 200, SCREEN_HEIGHT - 228 - (float) pileAmount * -2};
		AppendCardToPile(&Stock, &Deck[CardIndex]);
		CardIndex++;
	}



	// pool
	int SelectedPool = -1;

	constexpr int PoolSize = PileSize;
	Pool Pools[PoolSize] = {};
	for (int pool = 0; pool < PoolSize; pool++) {
		Pools[pool] = (Pool) {.position = (Vector2) {PADDING_X + (float) pool * OFFSET_X, PADDING_Y}, .gap = (int) OFFSET_Y, .pile = &Piles[pool]};
		SetPositionCardFromPool(&Pools[pool]);
	}

	constexpr int WinPoolSize = DeckSize/K;
	Pool WinPools[WinPoolSize] = {};
	Pile* WinPiles[WinPoolSize] = {nullptr, nullptr, nullptr, nullptr};
	for (int pool = 0; pool < WinPoolSize; pool++) {
		WinPools[pool] = (Pool) {.position = (Vector2) {PADDING_X + (float) pool * OFFSET_X, SCREEN_HEIGHT - 180}, .gap = -2, .pile = &WinPiles[pool]};
	}



	// card movement
	Vector2 MouseOffset = {0, 0};
	Vector2 CardOffset = {0, 0};



	// timer
	double Timer = 0.0f;
	double TimeStart = 0.0f;
	double TimeFinish = 0.0f;
	bool TimeStarted = false;
	bool TimeFinished = false;


	while(!WindowShouldClose()) {

		Debug();

		const Vector2 MousePosition = GetMousePosition();

		Timer = GetTime();



		// selected card
		if (!Holding)
			for (int pile = 0; pile < PileSize; pile++) {

				if (*Pools[pile].pile == NULL) continue;

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
						SelectedPool = pile;
						pile = PileSize;
						break;
					}
					SelectedCard = nullptr;
				 	SelectedPile = nullptr;
				 	SelectedPool = -1;

					if (TempPile->prev != NULL)
						TempPile = TempPile->prev;
					else
						break;
				} while (!Vector2Equals(TempPile->card->position,(*Pools[pile].pile)->prev->card->position));
			}

		if (*WinPools[WinPoolSize - 1].pile != nullptr) {
			SelectedCard = nullptr;
			if (!TimeFinished) TimeFinish = Timer - TimeStart;
			TimeFinished = true;
		}


		// stock
		if (StockSize)
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(MousePosition, (Rectangle)
				{Stock->prev->card->position.x, Stock->prev->card->position.y, CARD_WIDTH, CARD_HEIGHT})) {
				for (int pile = 0; pile < PileSize; pile++) {

					Card *TempCard = Stock->card;
					TempCard->show = true;
					RemovePile(&Stock, Stock);
					AppendCardToPile(Pools[pile].pile, TempCard);
					SetPositionCardFromPool(&Pools[pile]);
					StockSize--;
					if (StockSize == 0) break;
				}
			}



		// card movement
		//		press
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && SelectedCard != nullptr) {

			if (!TimeStarted) {
				TimeStart = Timer;
				TimeStarted = true;
			}

			Holding = true;


			TempPile = SelectedPile;
			while (TempPile->card != NULL) {
				if (TempPile->next != NULL) {
					if (TempPile->card->number -1 != TempPile->next->card->number) Holding = false;
					TempPile = TempPile->next;
				}
				else
					break;
			}

			CardOffset = Vector2Subtract(MousePosition, SelectedCard->position);
		}

		//		hold
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && Holding) {
			TempPile = SelectedPile;
			while (TempPile->card != NULL) {
				TempPile->card->position = Vector2Add(Vector2Subtract(TempPile->card->position, CardOffset), Vector2Subtract(MousePosition, MouseOffset));
				if (TempPile->next != NULL)
					TempPile = TempPile->next;
				else
					break;
			}

			MouseOffset = SelectedCard->position;
		}

		//		release
		if (IsMouseButtonUp(MOUSE_LEFT_BUTTON)) {
			if (SelectedPool != -1 && Holding) {
				for (int pool = 0; pool < PoolSize; pool++) {
					if (pool == SelectedPool) continue;

					if (*Pools[pool].pile != nullptr) {
						if ((*Pools[pool].pile)->prev->card->number -1 != SelectedCard->number) continue;
					}

					Rectangle TempRect = {*Pools[pool].pile == nullptr? Pools[pool].position.x : (*Pools[pool].pile)->prev->card->position.x,
	*Pools[pool].pile == nullptr? Pools[pool].position.y : (*Pools[pool].pile)->prev->card->position.y, CARD_WIDTH, CARD_HEIGHT};

					if (CheckCollisionPointRec((Vector2) {SelectedCard->position.x + (float) CARD_WIDTH / 2, SelectedCard->position.y}, TempRect)) {
						MoveCardsToPile(&Pools[SelectedPool], SelectedCard, &Pools[pool]);
						CheckAKPool(&Pools[pool], WinPools);
						SetPositionCardFromPool(&Pools[pool]);
					}
				}
				SetPositionCardFromPool(&Pools[SelectedPool]);
			}
			Holding = false;
		}



		// mouse
		if (SelectedCard) {
			Cursor = &CursorTextures[OPEN];
			if (Holding)
				Cursor = &CursorTextures[CLOSE];
		}
		else
			Cursor = &CursorTextures[POINT];



		// draw
		BeginDrawing();

		ClearBackground((Color) {51, 87, 171, 255});


		// stock
		if (StockSize) {
			TempPile = Stock->prev;
			 do {
			 	DrawTextureV(SpadeTextures[BACK], TempPile->card->position, WHITE);
				TempPile = TempPile->prev;
			} while (TempPile->card != Stock->prev->card);
		}


		// deck
		// draw cards per pool
		for (int pool = 0; pool < PoolSize; pool++) {

			if (*Pools[pool].pile == NULL) continue;

			// temp pile for getting every card at the loop
			TempPile = *Pools[pool].pile;

			// looping through all the cards
			while (TempPile->card != NULL) {

				if (TempPile->card == SelectedCard && Holding)
					break;

				DrawTextureV(SpadeTextures[TempPile->card->show? TempPile->card->number: BACK], TempPile->card->position, WHITE);

				// draw border if the card is selected
				if (SelectedCard)
					// check if the selected card is the selected one
					if (Vector2Equals(SelectedCard->position, TempPile->card->position))
						DrawTextureV(SpadeTextures[BORDER], TempPile->card->position, WHITE);


				// go next card or if there aren't more cards, then break
				if (TempPile->next != NULL)
					TempPile = TempPile->next;
				else
					break;
			}

		}

		for (int pool = 0; pool < WinPoolSize; pool++) {

			if (*WinPools[pool].pile == NULL) continue;

			// temp pile for getting every card at the loop
			TempPile = *WinPools[pool].pile;

			// looping through all the cards
			while (TempPile->card != NULL) {

				DrawTextureV(SpadeTextures[TempPile->card->number], TempPile->card->position, WHITE);

				// go next card or if there aren't more cards, then break
				if (TempPile->next != NULL)
					TempPile = TempPile->next;
				else
					break;
			}

		}


		// draw everything that's being held
		if (Holding) {
			TempPile = SelectedPile;

			// looping through all the cards
			while (TempPile->card != NULL) {

				DrawTextureV(SpadeTextures[TempPile->card->number], TempPile->card->position, WHITE);

				// go next card or if there aren't more cards, then break
				if (TempPile->next != NULL)
					TempPile = TempPile->next;
				else
					break;
			}
		}

		DrawText(TextFormat("%02d:%02.f",  (TimeFinished? (int) TimeFinish : (int) Timer - TimeStarted) / 60, (double)((int) (TimeFinished? TimeFinish : Timer - TimeStarted) % 60)), 10, 10, 24, BLACK);

		if (TimeFinished)
			DrawText("YOU WON", (SCREEN_WIDTH - MeasureText("YOU WON", 64)) / 2 , (SCREEN_HEIGHT - 64)/2, 64, GOLD);


		// mouse
		DrawTexture(*Cursor, (int) MousePosition.x - Cursor->width/4, (int) MousePosition.y - Cursor->height/4, WHITE);


		DebugDraw(
			LINE("%.0f\t%.0f", SelectedCard != nullptr? SelectedCard->position.x : 0, SelectedCard != nullptr ? SelectedCard->position.y : 0),
			LINE("%.0f\t%.0f", GetMousePosition().x, GetMousePosition().y)
		);


		EndDrawing();

	}


	// unload textures
	for (int i = 0; i < CardTexturesCount; i++) {
		UnloadTexture(SpadeTextures[i]);
	}

	for (int i = 0; i < 3; i++)
		UnloadTexture(CursorTextures[i]);


	CloseWindow();


	// free piles and stock
	for (int i = 0; i < PileSize; i++) {
		Pile* current = *Pools[i].pile;
		while (current != NULL) {
			Pile* next = current->next;
			free(current);
			current = next;
		}
		*Pools[i].pile = nullptr;
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
