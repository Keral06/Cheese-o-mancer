#pragma once

#include "Module.h"
#include "Vector2D.h"
#include "SDL3/SDL.h"
#include <SDL3_ttf/SDL_ttf.h>

class Render : public Module
{
public:

	Render();
	// Destructor
	virtual ~Render();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();

	// Drawing
	bool DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float speed = 1.0f, double angle = 0, int pivotX = INT_MAX, int pivotY = INT_MAX, SDL_FlipMode flip = SDL_FLIP_NONE) const;
	bool DrawParallax(SDL_Texture* texture, int x, int y, int width, int height, const SDL_Rect* section, float speed, double angle, int pivotX, int pivotY, SDL_FlipMode flip) const;
	bool DrawTextureNoCamera(SDL_Texture* texture, int x, int y, int w, int h, float speed = 1.0f, double angle = 0, int pivotX = INT_MAX, int pivotY = INT_MAX, SDL_FlipMode flip = SDL_FLIP_NONE) const;
	bool DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool useCamera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;
	bool DrawText(const char* text, int x, int y, int w, int h, SDL_Color color) const;
	// Set background color
	void SetBackgroundColor(SDL_Color color);

	bool IsOnScreenWorldRect(float x, float y, float w, float h, int margin = 0) const;

	void SetZoomSmooth(float newZoom, float duration);

	float EaseInOut(float t);

	void SetCameraFocusSmooth(float worldX, float worldY, float newZoom, float duration);

public:

	SDL_Renderer* renderer;
	SDL_Rect camera;
	SDL_Rect viewport;
	SDL_Color background;
	float zoom = 0.3f;

	float startZoom = 1.0f;
	float targetZoom = 1.0f;

	float zoomTime = 0.0f;
	float zoomDuration = 0.0f;

	bool zooming = false;
	bool cameraMoving = false;

	float camStartX = 0.0f;
	float camStartY = 0.0f;

	float camTargetX = 0.0f;
	float camTargetY = 0.0f;

	float camStartZoom = 0.0f;
	float camTargetZoom = 1.0f;

	float camTime = 0.0f;
	float camDuration = 1.0f;

private:
	bool vsync = false;
	TTF_Font* font;

};
