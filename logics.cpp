#include "logics.h"

Logic::Logic( Logistic& _logistics ) : logistics(_logistics)
{
}

Logic::~Logic()
{
}

void Logic::Initialize()
{
	// TO DO: Load info about objects from some file
	// TO DO: Send commands to different parts about what to load
	// Ex: logistics.messages.push_back( Logistic::Message("graphic", "load", std::string("hej.obj") ) );
}

void Logic::Update( unsigned int diffTime )
{
}

void Logic::Input( SDL_Event event )
{
}

void Logic::Messages()
{
	while( logistics.MessageCount( "logic" ) )
	{
		Logistic::Message message( logistics.GetMessage( "logic" ) );
		// TO DO: Do something
	}
}

