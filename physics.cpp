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
	while( logistics.MessageCount( "physic" ) )
	{
		Logistic::Message message( logistics.GetMessage( "physic" ) );
		// TO DO: Do something
	}
}

