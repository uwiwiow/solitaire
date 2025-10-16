#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <time.h>
#include "card.h"
#include "pile.h"
#include "pool.h"
#include "debug.h"

constexpr int WIDTH = 1240;
constexpr int HEIGHT = 720;



int main(int argc, char** argv) {

	// init
	SetTraceLogLevel(LOG_WARNING);
	InitWindow(WIDTH, HEIGHT, "Solitaire");



	// textures
	Texture2D SpadesAtlas = {};
	LoadTextureCard(&SpadesAtlas, "res/Poker/Top-Down/Cards/Spades-88x124.png");
	const Texture2D CursorTextures[3] = {
		LoadTexture("res/kenney_cursor-pack/PNG/Outline/Default/hand_thin_point.png"),
		LoadTexture("res/kenney_cursor-pack/PNG/Outline/Default/hand_thin_open.png"),
		LoadTexture("res/kenney_cursor-pack/PNG/Outline/Default/hand_thin_closed.png")
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

	// for (int i = DeckSize - 1; i > 0; i--) {
	// 	int j = rand() % (i + 1);
	// 	Card temp = Deck[i];
	// 	Deck[i] = Deck[j];
	// 	Deck[j] = temp;
	// }



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
		Deck[CardIndex].position = (Vector2) {WIDTH - 200, HEIGHT - 180 - (float) pileAmount * 2};
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
		WinPools[pool] = (Pool) {.position = (Vector2) {PADDING_X + (float) pool * (OFFSET_X / 2), HEIGHT - PADDING_Y *2}, .gap = 2, .pile = &WinPiles[pool]};
	}

	// card movement
	Vector2 MouseOffset = {0, 0};
	Vector2 CardOffset = {0, 0};


	while(!WindowShouldClose()) {

		Debug();

		const Vector2 MousePosition = GetMousePosition();



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
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && SelectedCard) {
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
					} else {
						if (SelectedCard->number != K) continue;
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
			TempPile = Stock;
			while (TempPile->card != NULL) {
				DrawCard(SpadesAtlas, *TempPile->card);
				if (TempPile->next != NULL)
					TempPile = TempPile->next;
				else
					break;
			}
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

		for (int pool = 0; pool < WinPoolSize; pool++) {

			if (*WinPools[pool].pile == NULL) continue;

			// temp pile for getting every card at the loop
			TempPile = *WinPools[pool].pile;

			// looping through all the cards
			while (TempPile->card != NULL) {

				DrawCard(SpadesAtlas, *TempPile->card);

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

				DrawCard(SpadesAtlas, *TempPile->card);

				// go next card or if there aren't more cards, then break
				if (TempPile->next != NULL)
					TempPile = TempPile->next;
				else
					break;
			}
		}


		// mouse
		DrawTexture(*Cursor, (int) MousePosition.x - Cursor->width/4, (int) MousePosition.y - Cursor->height/4, WHITE);


		if (SelectedCard != nullptr)
			DebugDraw(
				LINE("%.0f\t%.0f", SelectedCard->position.x, SelectedCard->position.y),
				LINE("%.0f\t%.0f", GetMousePosition().x, GetMousePosition().y)
			);


		EndDrawing();

	}


	// unload textures
	UnloadTexture(SpadesAtlas);

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
