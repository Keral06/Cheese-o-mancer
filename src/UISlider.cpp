#include "UISlider.h"
#include "Engine.h"
#include "Input.h"
#include "Render.h"
#include "Audio.h"

UISlider::UISlider(int id, SDL_Rect bounds, const char* text) : UIElement(UIElementType::SLIDER, id)
{
	this->bounds = bounds;
	this->text = text;

	sliderBar = { bounds.x, bounds.y + (bounds.h / 2) - 2, bounds.w, 4 };

	int thumbWidth = 40;
	thumb = { bounds.x, bounds.y, thumbWidth, bounds.h };

	SetValue(0.5f);
}

UISlider::~UISlider()
{
}

bool UISlider::Update(float dt)
{
	if (!visible) return true;
	if (state != UIElementState::DISABLED)
	{
		Vector2D mousePos = Engine::GetInstance().input->GetMousePosition();
		SDL_Point mousePoint = { (int)mousePos.getX(), (int)mousePos.getY() };

		bool mouseInside = SDL_PointInRect(&mousePoint, &bounds);

		if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			if (mouseInside || isDragging)
			{
				state = UIElementState::FOCUSED;
				isDragging = true;

				float relativeX = (float)(mousePoint.x - sliderBar.x - (thumb.w / 2));
				float normalized = relativeX / (float)(sliderBar.w - thumb.w);

				float oldValue = value;
				SetValue(normalized);

				if (oldValue != value) {
					NotifyObserver(); 
				}
			}
		}
		else if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		{
			isDragging = false;
			state = UIElementState::NORMAL;
		}
		else
		{
			if (mouseInside) state = UIElementState::FOCUSED;
			else state = UIElementState::NORMAL;
		}
	}

	return true;
}

bool UISlider::Draw()
{
	if (!visible) return true;
	SDL_Color barColor = { 100, 100, 100, 255 };
	SDL_Color thumbColorNormal = { 0, 121, 241, 255 };
	SDL_Color thumbColorPressed = { 0, 82, 172, 255 };

	Engine::GetInstance().render->DrawRectangle(sliderBar, barColor.r, barColor.g, barColor.b, barColor.a, true, false);

	SDL_Color currentThumbColor = (state == UIElementState::PRESSED || isDragging) ? thumbColorPressed : thumbColorNormal;
	Engine::GetInstance().render->DrawRectangle(thumb, currentThumbColor.r, currentThumbColor.g, currentThumbColor.b, currentThumbColor.a, true, false);

	if (!text.empty())
	{
		Engine::GetInstance().render->DrawText(text.c_str(), bounds.x, bounds.y - 20, 0, 0, { 255, 255, 255, 255 });
	}

	return true;
}

void UISlider::SetValue(float val)
{
	if (val < 0.0f) val = 0.0f;
	else if (val > 1.0f) val = 1.0f;

	value = val;

	thumb.x = sliderBar.x + (int)(value * (float)(sliderBar.w - thumb.w));
}

float UISlider::GetValue() const
{
	return value;
}

bool UISlider::CleanUp()
{
	pendingToDelete = true;
	return true;
}