#pragma once

#include "UIElement.h"
#include "Vector2D.h"

class UICheckBox : public UIElement
{

public:

	UICheckBox(int id, SDL_Rect bounds, const char* text);
	virtual ~UICheckBox();

	// Called each loop iteration
	bool Update(float dt);
	bool Draw() override;
	bool CleanUp() override;

private:
	
	bool canClick = true;
	bool drawBasic = false;
};

