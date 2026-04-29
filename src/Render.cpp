#include "Engine.h"
#include "Window.h"
#include "Render.h"
#include "Log.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Render::Render() : Module()
{
	name = "render";
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
Render::~Render()
{
}

// Called before render is available
bool Render::Awake()
{
	LOG("Create SDL rendering context");
	bool ret = true;

	int scale = Engine::GetInstance().window->GetScale() / 6;
	SDL_Window* window = Engine::GetInstance().window->window;
	//L05 TODO 5 - Load the configuration of the Render module
	
	// SDL3: no flags; create default renderer and set vsync separately
	renderer = SDL_CreateRenderer(window, nullptr);

	if (renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		if (configParameters.child("vsync").attribute("value").as_bool())
		{
			if (!SDL_SetRenderVSync(renderer, 1))
			{
				LOG("Warning: could not enable vsync: %s", SDL_GetError());
			}
			else
			{
				LOG("Using vsync");
			}
		}
		SDL_SetRenderLogicalPresentation(renderer, 1280, 720, SDL_LOGICAL_PRESENTATION_LETTERBOX);
		camera.w = Engine::GetInstance().window->width;
		camera.h = Engine::GetInstance().window->height;
		camera.x = 0;
		camera.y = 0;
		
	}

	if (!TTF_Init())
	{
		LOG("TTF_Init failed: %s", SDL_GetError());
		ret = false;
	}
	else
	{
		font = TTF_OpenFont("resources/UI/UI_Fonts/CheesomancerText2.ttf", 24);

		if (font == nullptr)
		{
			LOG("Failed to load font! SDL_ttf Error: %s", SDL_GetError());
		}
	}


	return ret;
}

// Called before the first frame
bool Render::Start()
{
	LOG("render start");
	// back background
	if (!SDL_GetRenderViewport(renderer, &viewport))
	{
		LOG("SDL_GetRenderViewport failed: %s", SDL_GetError());
	}
	return true;
}

// Called each loop iteration
bool Render::PreUpdate()
{
	SDL_RenderClear(renderer);
	return true;
}

bool Render::Update(float dt)
{
	if (zooming)
	{
		zoomTime += dt;

		float t = zoomTime / zoomDuration;

		if (t >= 1.0f)
		{
			t = 1.0f;
			zooming = false;
		}

		
		float easedT = EaseInOut(t);
		zoom = startZoom + (targetZoom - startZoom) * easedT;
	}
	return true;
}

bool Render::PostUpdate()
{
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

// Called before quitting
bool Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

void Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_SetRenderViewport(renderer, &rect);
}

void Render::ResetViewPort()
{
	SDL_SetRenderViewport(renderer, &viewport);
}

// Blit to screen
bool Render::DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, double angle, int pivotX, int pivotY, SDL_FlipMode flip) const
{
	int scale = Engine::GetInstance().window->GetScale();

	SDL_FRect rect;

	
	float world_x = (camera.x + x * scale) * zoom;
	float world_y = (camera.y + y * scale) * zoom;

	rect.x = world_x;
	rect.y = world_y;

	
	if (section)
	{
		rect.w = section->w * scale * zoom;
		rect.h = section->h * scale * zoom;
	}
	else
	{
		float tw, th;
		SDL_GetTextureSize(texture, &tw, &th);

		rect.w = tw * scale * zoom;
		rect.h = th * scale * zoom;
	}

	
	SDL_FPoint* p = nullptr;
	SDL_FPoint pivot;

	if (pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot = {
			pivotX * scale * zoom,
			pivotY * scale * zoom
		};
		p = &pivot;
	}

	
	const SDL_FRect* src = nullptr;
	SDL_FRect srcRect;

	if (section)
	{
		srcRect = {
			(float)section->x,
			(float)section->y,
			(float)section->w,
			(float)section->h
		};
		src = &srcRect;
	}

	return SDL_RenderTextureRotated(
		renderer,
		texture,
		src,
		&rect,
		angle,
		p,
		flip
	);
}
bool Render::DrawTextureNoCamera(SDL_Texture* texture, int x, int y, int w, int h, float speed, double angle, int pivotX, int pivotY, SDL_FlipMode flip) const
{
	bool ret = true;


	// SDL3 uses float rects for rendering
	SDL_FRect rect;
	

	rect.x = x ;
	rect.y = y;


	
		rect.w = w;
		rect.h = h;
	

	const SDL_FRect* src = NULL;
	SDL_FRect srcRect;
	

	SDL_FPoint* p = NULL;
	SDL_FPoint pivot;
	if (pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot = { (float)pivotX, (float)pivotY};
		p = &pivot;
	}

	// SDL3: returns bool; map to int-style check
	int rc = SDL_RenderTextureRotated(renderer, texture, src, &rect, angle, p, flip) ? 0 : -1;
	if (rc != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderTextureRotated error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}
bool Render::DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	int scale = Engine::GetInstance().window->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_FRect rec;

	float world_x = (camera.x + rect.x * scale) * zoom;
	float world_y = (camera.y + rect.y * scale) * zoom;

	if (use_camera)
	{
		rec.x = world_x;
		rec.y = world_y;
	}
	else
	{
		rec.x = rect.x * scale;
		rec.y = rect.y * scale;
	}

	rec.w = rect.w * scale * zoom;
	rec.h = rect.h * scale * zoom;

	return filled ?
		SDL_RenderFillRect(renderer, &rec) :
		SDL_RenderRect(renderer, &rec);
}

bool Render::DrawText(const char* text, int x, int y, int w, int h, SDL_Color color) const
{
	if (!font || !renderer || !text) {
		LOG("DrawText: invalid font/renderer/text");
		return false;
	}

	// Render the text to a surface
	// SDL3_ttf: length can be 0 for null-terminated strings
	SDL_Surface* surface = TTF_RenderText_Solid(font, text, 0, color);
	if (!surface) {
		LOG("DrawText: TTF_RenderText_Solid failed: %s", SDL_GetError());
		return false;
	}

	// Create a texture from the surface
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) {
		LOG("DrawText: SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
		SDL_DestroySurface(surface);
		return false;
	}

	// Optional but often needed when using alpha/text
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

	// If w/h are 0, use the text�s natural size
	float fw = (w > 0) ? (float)w : (float)surface->w;
	float fh = (h > 0) ? (float)h : (float)surface->h;

	SDL_FRect dstrect = { (float)x, (float)y, fw, fh };

	// Render the texture to the current render target
	if (!SDL_RenderTexture(renderer, texture, nullptr, &dstrect)) {
		LOG("DrawText: SDL_RenderTexture failed: %s", SDL_GetError());
	}

	// Cleanup
	SDL_DestroyTexture(texture);
	SDL_DestroySurface(surface);

	return true;
}

bool Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	int scale = Engine::GetInstance().window->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	float X1, Y1, X2, Y2;

	if (use_camera)
	{
		float center_x = (float)(camera.w / 2);
		float center_y = (float)(camera.h / 2);

		float world_x1 = (float)(camera.x + x1 * scale) * zoom;
		float world_y1 = (float)(camera.y + y1 * scale) * zoom;
		X1 = center_x + (world_x1 - center_x);
		Y1 = center_y + (world_y1 - center_y);

		float world_x2 = (float)(camera.x + x2 * scale) * zoom;
		float world_y2 = (float)(camera.y + y2 * scale) * zoom;
		X2 = center_x + (world_x2 - center_x);
		Y2 = center_y + (world_y2 - center_y);
	}
	else
	{
		X1 = (float)(x1 * scale);
		Y1 = (float)(y1 * scale);
		X2 = (float)(x2 * scale);
		Y2 = (float)(y2 * scale);
	}

	int result = SDL_RenderLine(renderer, X1, Y1, X2, Y2) ? 0 : -1;

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderLine error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	int scale = Engine::GetInstance().window->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_FPoint points[360];

	float rad = radius * scale * zoom;

	float cx, cy;

	float screenX = camera.w * 0.5f;
	float screenY = camera.h * 0.5f;

	if (use_camera)
	{
		cx = (camera.x + x * scale) * zoom + screenX;
		cy = (camera.y + y * scale) * zoom + screenY;
	}
	else
	{
		cx = x * scale + screenX;
		cy = y * scale + screenY;
		rad = radius * scale;
	}

	float factor = (float)M_PI / 180.0f;

	for (int i = 0; i < 360; i++)
	{
		points[i].x = cx + cosf(i * factor) * rad;
		points[i].y = cy + sinf(i * factor) * rad;
	}

	return SDL_RenderPoints(renderer, points, 360);
}
bool Render::IsOnScreenWorldRect(float x, float y, float w, float h, int margin) const
{
	bool result = false;

	// --- 1. Compute camera rectangle in WORLD space ---

	// camera.x and camera.y are render offsets (negative world position),
	// so we negate them to get the camera position in world coordinates.
	// The margin expands the camera rectangle to activate objects earlier.
	float camLeft = -camera.x - margin;
	float camTop = -camera.y - margin;

	// Right and bottom edges of the camera rectangle
	float camRight = camLeft + camera.w + margin * 2;
	float camBottom = camTop + camera.h + margin * 2;

	// --- 2. Compute object rectangle in WORLD space ---

	float objLeft = x;
	float objTop = y;
	float objRight = x + w;
	float objBottom = y + h;

	// --- 3. Axis-Aligned Bounding Box (AABB) overlap test ---

	result = objRight >= camLeft &&
		objLeft <= camRight &&
		objBottom >= camTop &&
		objTop <= camBottom;

	return result;
}

void Render::SetZoomSmooth(float newZoom, float duration)
{
	startZoom = zoom;
	targetZoom = newZoom;

	zoomDuration = duration;
	zoomTime = 0.0f;

	zooming = true;
}
float Render::EaseInOut(float t)
{
	return t * t * (3.0f - 2.0f * t);
}