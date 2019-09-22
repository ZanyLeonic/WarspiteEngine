#pragma once
#ifndef __Game__
#define __Game__

#include <SDL.h>
#include "TextureManager.h"
#include "GameObject.h"
#include "Player.h"
#include <vector>

class Game
{
public:

	Game() {}
	~Game() {}

	// simply set the running variable to true
	bool Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void Draw();
	void OnThink();
	void HandleEvents();
	void Destroy();

	// a function to access the private running variable
	bool IsRunning() { return m_bRunning; }

private:

	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;

	SDL_Texture* m_pTexture; // the new SDL_Texture variable
	SDL_Rect m_sourceRectangle; // the first rectangle
	SDL_Rect m_destinationRectangle; // another rectangle

	bool m_bRunning;
	
	int m_currentFrame;

	std::vector<GameObject*> m_gameObjects;

	GameObject m_bo;
	Player m_player;
};

#endif /* defined(__Game__) */