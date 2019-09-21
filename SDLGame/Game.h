#pragma once
#ifndef __Game__
#define __Game__

class Game
{
public:

	Game() {}
	~Game() {}

	// simply set the running variable to true
	void init() { m_bRunning = true; }

	void render() {}
	void tick() {}
	void handleEvents() {}
	void clean() {}

	// a function to access the private running variable
	bool running() { return m_bRunning; }

private:

	bool m_bRunning;
};

#endif /* defined(__Game__) */