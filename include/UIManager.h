#pragma once

#include "Module.h"
#include "UIElement.h"

#include <list>

class UIManager : public Module
{
public:

	// Constructor
	UIManager();

	// Destructor
	virtual ~UIManager();

	// Called before the first frame
	 bool Start();

	 // Called each loop iteration
	 bool Update(float dt);

	 bool PostUpdate() override;
	// Called before quitting
	bool CleanUp();

	// Additional methods
	std::shared_ptr<UIElement> CreateUIElement(UIElementType type, int id, const char* text, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds = { 0,0,0,0 }, SDL_Texture* texture1 = nullptr, SDL_Texture* texture2 = nullptr, SDL_Texture* texture3=nullptr);

public:

	std::list<std::shared_ptr<UIElement>> UIElementsList;
	SDL_Texture* texture;

};
