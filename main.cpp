#include "physics.h"
#include "graphics.h"
#include "use_sdl.h"
#include "use_opengl.h"

const Uint32 fps = 60;
bool done = false;
Physic physics;
Graphic graphics;

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
	SDL_Init( SDL_INIT_VIDEO );

	SDL_SetVideoMode( 800, 600, 0, SDL_OPENGL | SDL_HWSURFACE );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	
	glHint( GL_POINT_SMOOTH_HINT,GL_NICEST );
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	glEnable( GL_DEPTH_TEST ); 
	glEnable( GL_BLEND );

	glViewport (0, 0, 800, 600);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);

	glMatrixMode (GL_PROJECTION);
	glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 1000.0);
	glMatrixMode (GL_MODELVIEW);

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
}

void Update( Uint32 diffTime )
{
	SDL_Delay( 10 );
	physics.Update( diffTime );
	graphics.Update( diffTime );
}

