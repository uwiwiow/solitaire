#include <raylib.h>
#include <raymath.h>
#include "card.h"
#include "debug.h"

int main(int argc, char** argv) {

	// SetTraceLogLevel(LOG_WARNING);
	InitWindow(900, 600, "Solitaire");

	Texture2D SpadesAtlas = {};
	LoadTextureCard(&SpadesAtlas, "res/Poker/Top-Down/Cards/Spades-88x124.png");

	Card Deck = {A, true, {50, 50}};
	Card *Selected = nullptr;

	Vector2 MousePosition = {0, 0};
	bool MouseHold = false;
	Vector2 MouseOffset = {0, 0};

	while(!WindowShouldClose()) {

		Debug();

		if (CheckCollisionPointRec(GetMousePosition(), (Rectangle) {Deck.position.x, Deck.position.y, 88, 124})) {
			Selected = &Deck;
		}
		else {
			if (!MouseHold)
				Selected = nullptr;
		}

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && Selected) {
			if (!MouseHold){
				MouseOffset = Vector2Subtract(Selected->position, GetMousePosition());
				MouseHold = true;
			} 
			Selected->position = Vector2Add(GetMousePosition(), MouseOffset);
		}
		
		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && Selected) {
			MouseHold = false;
		}

		BeginDrawing();

		ClearBackground((Color) {51, 87, 171, 255});

		DrawCard(SpadesAtlas, Deck);

		if (Selected)
			DrawCardBorder(SpadesAtlas, *Selected);


		DebugDraw(
			LINE("%.0f\t%.0f", MousePosition.x, MousePosition.y),
			LINE("%.0f\t%.0f", GetMousePosition().x, GetMousePosition().y)
		);


		EndDrawing();

	}

	UnloadTexture(SpadesAtlas);

	CloseWindow();


	return 0;
}
