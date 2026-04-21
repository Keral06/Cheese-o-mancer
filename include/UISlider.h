#pragma once

#include "UIElement.h"

class UISlider : public UIElement
{
public:

	UISlider(int id, SDL_Rect bounds, const char* text, SDL_Texture* bar =nullptr, SDL_Texture* thumbNormal=nullptr, SDL_Texture* thumbPressed=nullptr);
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
	SDL_Texture* sliderBarTexture = nullptr;
	SDL_Texture* thumbNormalTexture = nullptr;
	SDL_Texture* thumbPressedTexture = nullptr;
	bool isDragging = false;
};
