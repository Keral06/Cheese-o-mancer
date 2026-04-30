#include "UIButton.h"
#include "Render.h"
#include "Engine.h"
#include "Audio.h"

UIButton::UIButton(int id, SDL_Rect bounds, const char* text) : UIElement(UIElementType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	drawBasic = false;
	buttonNormalTexture = nullptr;
	buttonPressedTexture = nullptr;
}
UIButton::UIButton(int id, SDL_Rect bounds, const char* text, SDL_Texture* normalTexture, SDL_Texture* pressedTexture) : UIElement(UIElementType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
	this->buttonNormalTexture = normalTexture;
	this->buttonPressedTexture = pressedTexture;

	drawBasic = false;

}

UIButton::~UIButton()
{

}

bool UIButton::Update(float dt)
{
	if (!visible) return true;
	if (state != UIElementState::DISABLED)
	{
		Vector2D mousePos = Engine::GetInstance().input->GetMousePosition();
		SDL_Point mousePoint = { (int)mousePos.getX(), (int)mousePos.getY() };

		if (SDL_PointInRect(&mousePoint, &bounds))
		{
			state = UIElementState::FOCUSED;

			KeyState leftMouse = Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT);

			if (leftMouse == KEY_DOWN || leftMouse == KEY_REPEAT)
			{
				state = UIElementState::PRESSED;
			}

			if (leftMouse == KEY_UP)
			{
				NotifyObserver();
			}
		}
		else
		{
			state = UIElementState::NORMAL;
		}
	}

	return true;
}
bool UIButton::Draw()
{
	if (!visible) return true;
	if (buttonNormalTexture != nullptr && buttonPressedTexture != nullptr)
	{
		switch (state)
		{
		case UIElementState::NORMAL:  Engine::GetInstance().render->DrawTextureNoCamera(buttonNormalTexture, bounds.x, bounds.y, bounds.w, bounds.h); break;
		case UIElementState::FOCUSED: Engine::GetInstance().render->DrawTextureNoCamera(buttonPressedTexture, bounds.x, bounds.y, bounds.w, bounds.h); break;
		case UIElementState::PRESSED: Engine::GetInstance().render->DrawTextureNoCamera(buttonPressedTexture, bounds.x, bounds.y, bounds.w, bounds.h); break;
		case UIElementState::DISABLED: Engine::GetInstance().render->DrawTextureNoCamera(buttonNormalTexture, bounds.x, bounds.y, bounds.w, bounds.h); break;
		}
	}
	else
	{
		SDL_Color colorNormal = { 0, 121, 241, 255 };
		SDL_Color colorFocused = { 102, 191, 255, 255 };
		SDL_Color colorPressed = { 0, 82, 172, 255 };
		SDL_Color colorDisabled = { 200, 200, 200, 255 };

		SDL_Color drawColor = colorNormal;

		switch (state)
		{
		case UIElementState::NORMAL:  drawColor = colorNormal; break;
		case UIElementState::FOCUSED: drawColor = colorFocused; break;
		case UIElementState::PRESSED: drawColor = colorPressed; break;
		case UIElementState::DISABLED: drawColor = colorDisabled; break;
		}

		Engine::GetInstance().render->DrawRectangle(bounds, drawColor.r, drawColor.g, drawColor.b, drawColor.a, true, false);

		if (!text.empty())
		{
			int textX = bounds.x + 10;
			int textY = bounds.y + (bounds.h / 2) - 10;

			Engine::GetInstance().render->DrawText(text.c_str(), textX + 2, textY + 2, 0, 0, { 0, 0, 0, 255 });

			Engine::GetInstance().render->DrawText(text.c_str(), textX, textY, 0, 0, { 255, 255, 255, 255 });
		}

		return true;
	}
}
bool UIButton::CleanUp()
{
	pendingToDelete = true;
	return true;
}