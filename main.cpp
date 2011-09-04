#include "physics.h"
#include "use_sdl.h"
#include "use_opengl.h"

bool done = false;

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
}

void Input( void );
void Update( void );
void Draw( void );
void Run( void )
{
	Uint32 time = SDL_GetTicks();
	Uint32 secondTime = SDL_GetTicks();
	while( !done )
	{
		while( time + 10 > secondTime)
		{
			SDL_Delay(1);
			secondTime = SDL_GetTicks();
		}

		Input();
		Update();
		Draw();
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

void Update( void )
{
}

void Draw( void )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glTranslatef( 0.0, 0.0, -100.0 );
	glRotatef( 45, 1.0, 1.0, 1.0 );

	SDL_GL_SwapBuffers();
}
