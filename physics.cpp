#include "physics.h"

Physic::Physic( Logistic& _logistics ) : logistics(_logistics)
{}

Physic::~Physic()
{}

void Physic::Initialize()
{}

void Physic::Update( unsigned int diffTime )
{}

void Physic::Messages()
{
	for( int i(0); i < logistics.messages.size(); i++ )
	{
		if( logistics.messages[i][0] == Logistic::physic )
		{
			// Message for graphic
			// Do something
			logistics.messages.erase( logistics.messages.begin() + i);
		}
	}
}

