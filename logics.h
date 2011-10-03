#pragma once
#include "physics.h"
#include "graphics.h"
#include "use_sdl.h"

class Logic
{
public:
	Logic( Physic& _physic, Graphic& _graphics );
	~Logic();
	void Initialize();
	void Update( Uint32 diffTime );
	void Input( SDL_Event event );
private:
	Physic& physics;
	Graphic& graphics;
};

