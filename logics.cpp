#include "logics.h"

Logic::Logic( Logistic& _logistics ) : logistics(_logistics)
{
}

Logic::~Logic()
{
}

void Logic::Initialize()
{
}

void Logic::Update( unsigned int diffTime )
{
}

void Logic::Input( SDL_Event event )
{
}

void Logic::Messages()
{
	for( int i(0); i < logistics.messages.size(); i++ )
	{
		if( logistics.messages[i][0] == Logistic::logic )
		{
			// Message for graphic
			// Do something
			logistics.messages.erase( logistics.messages.begin() + i);
		}
	}
}

