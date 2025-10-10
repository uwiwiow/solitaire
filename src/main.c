#include <raylib.h>
#include <raymath.h>
#include "card.h"
#include "debug.h"

constexpr int WIDTH = 900;
constexpr int HEIGHT = 600;

int main(int argc, char** argv) {

	SetTraceLogLevel(LOG_WARNING);
	InitWindow(WIDTH, HEIGHT, "Solitaire");

	Texture2D SpadesAtlas = {};
	LoadTextureCard(&SpadesAtlas, "res/Poker/Top-Down/Cards/Spades-88x124.png");

	Card Deck[2] = {{A, false, {50, 50}}, {2, true, {100, 50}}};
	Card *Selected = nullptr;

	Vector2 MousePosition = {0, 0};
	bool MouseHold = false;
	Vector2 MouseOffset = {0, 0};

	while(!WindowShouldClose()) {

		Debug();

		for (int i = 0; i < sizeof(Deck) / sizeof(Card); i++){
			if (CheckCollisionPointRec(GetMousePosition(), (Rectangle) {Deck[i].position.x, Deck[i].position.y, 88, 124})) {
				if (Deck[i].show){
					Selected = &Deck[i];
					break;
				}
			}
			else {
				if (!MouseHold)
					Selected = nullptr;
			}
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

		for (int i = 0; i < sizeof(Deck) / sizeof(Card); i++){
			DrawCard(SpadesAtlas, Deck[i]);
		}
		
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
