#include "physics.h"
#include "graphics.h"
#include "logics.h"
#include "use_sdl.h"

const Uint32 fps = 60;
bool done = false;
Physic physics;
Graphic graphics;
Logic logics( physics, graphics );

void Initialize( void );
void Run( void );
int main( int argc, char *argv[])
{
	Initialize();
	Run();
	return 0;
}

void Initialize( void )
{
	logics.Initialize();
	physics.Initialize();
	graphics.Initialize();
}

void Input( void );
void Update( Uint32 diffTime );
void Run( void )
{
	Uint32 time, secondTime;
	time = secondTime = SDL_GetTicks();
	while( !done )
	{
		if( secondTime - time < 1000 / fps )
		{
			SDL_Delay( 1000 / fps - ( secondTime - time ) );
			secondTime = SDL_GetTicks();
		}

		Input();
		Update( secondTime - time );
		time = SDL_GetTicks();
	}
}

void Input( void )
{
	SDL_Event event;
	while( SDL_PollEvent(&event) )
		if(event.key.keysym.sym == SDLK_ESCAPE)
			done = true;
		else
			logics.Input( event );
}

void Update( Uint32 diffTime )
{
	SDL_Delay( 10 );
	logics.Update( diffTime );
	physics.Update( diffTime );
	graphics.Update( diffTime );
}

