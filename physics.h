#pragma once
#include "logistics.h"
#include "vector.h"

class Physic
{
public:
	Physic( Logistic& _logistics );
	~Physic();
	void Initialize();
	void Update( unsigned int diffTime );
private:
	Logistic& logistics;
	void Messages();
	struct Object
	{
	};
	std::vector<Object> objects;
};

