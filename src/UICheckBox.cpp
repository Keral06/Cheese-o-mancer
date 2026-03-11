#include "UICheckBox.h"
#include "Render.h"
#include "Engine.h"
#include "Audio.h"

UICheckBox::UICheckBox(int id, SDL_Rect bounds, const char* text) : UIElement(UIElementType::CHECKBOX, id)
{
	this->bounds = bounds;
	this->text = text;

	drawBasic = false;
}

UICheckBox::~UICheckBox()
{

}

bool UICheckBox::Update(float dt)
{
	if (!visible) return true;
	if (state != UIElementState::DISABLED)
	{
		Vector2D mousePos = Engine::GetInstance().input->GetMousePosition();
		SDL_Point mousePoint = { (int)mousePos.getX(), (int)mousePos.getY() };

		if (SDL_PointInRect(&mousePoint, &bounds))
		{
			/*state = UIElementState::FOCUSED;*/

			KeyState leftMouse = Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT);

			if (leftMouse == KEY_DOWN )
			{
				if (state == UIElementState::SELECTED)
					state = UIElementState::NORMAL;
				else 
					state = UIElementState::SELECTED;
			}
			

			if (leftMouse == KEY_UP)
			{
				NotifyObserver();
			}
		}
		
	}

	return true;
}
bool UICheckBox::Draw()
{
	if (!visible) return true;
	SDL_Color colorNormal = { 0, 121, 241, 255 };   
	
	SDL_Color colorSelected = { 200, 200, 200, 255 }; 

	SDL_Color drawColor = colorNormal;

	switch (state)
	{
	case UIElementState::NORMAL:  drawColor = colorNormal; break;
	
	case UIElementState::SELECTED: drawColor = colorSelected; break;
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
bool UICheckBox::CleanUp()
{
	pendingToDelete = true;
	return true;
}