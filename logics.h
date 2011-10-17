#pragma once
#include "logistics.h"
#include "use_sdl.h"

class Logic
{
public:
	Logic( Logistic& _logistics );
	~Logic();
	void Initialize();
	void Update( Uint32 diffTime );
	void Input( SDL_Event event );
private:
	Logistic& logistics;
	void Messages();
	struct Object
	{
	};
	std::vector<Object> objects;
};

