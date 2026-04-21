#pragma once

#include "UIElement.h"
#include "Vector2D.h"

class UIButton : public UIElement
{

public:

	UIButton(int id, SDL_Rect bounds, const char* text);
	UIButton(int id, SDL_Rect bounds, const char* text, SDL_Texture* normalTexture, SDL_Texture* pressedTexture);
	virtual ~UIButton();

	// Called each loop iteration
	bool Update(float dt);
	bool Draw() override;
	bool CleanUp() override;

private:

	bool canClick = true;
	bool drawBasic = false;
	SDL_Texture* buttonPressedTexture = nullptr;
	SDL_Texture* buttonNormalTexture = nullptr;
};

#pragma once