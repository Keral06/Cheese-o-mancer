#pragma once

#include "UIElement.h"

class UISlider : public UIElement
{
public:

	UISlider(int id, SDL_Rect bounds, const char* text);
	virtual ~UISlider();

	bool Update(float dt) override;
	bool Draw() override;
	bool CleanUp() override;

	void SetValue(float value);
	float GetValue() const;

private:

	SDL_Rect sliderBar;
	SDL_Rect thumb;
	float value = 0.5f;

	bool isDragging = false;
};
