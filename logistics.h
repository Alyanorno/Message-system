#pragma once
#include <vector>

struct Logistic
{
	Logistic();
	~Logistic();
	void Initialize();

	enum { physic, graphic, logic };
	struct Object
	{
		struct Id
		{
			Id( int _owner, int _id ) : owner(_owner), id(_id) {}
			int owner, id;
		};
		std::vector< Id > ids;
	};
	std::vector< Object > objects;
	std::vector< std::vector< int > > messages;
};

