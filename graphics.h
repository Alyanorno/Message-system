#pragma once
#include "use_sdl.h"
#include "use_opengl.h"
#include "vector.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <vector>

class Graphic
{
public:
	Graphic();
	~Graphic();
	void Initialize();
	void Update( Uint32 diffTime );
private:
	struct Texture
	{
		Texture( int size, int _width, int _height ) : width(_width), height(_height)
			{ t = new BYTE[size]; }
		~Texture() { delete[] t; }

		BYTE* t;
		int width, height;
	};
	struct Model
	{
		Model( int size ) : num(size)
		{
			vertexs = new float[ size * 3 ];
			normals = new float[ size * 3 ];
			texCoords = new float[ size * 2 ];
		}
		~Model() {}
		
		int num;
		float* vertexs;
		float* normals;
		float* texCoords;
	};
	Texture LoadBmp( std::string name );
	Model LoadObj( std::string name );
	std::vector< Texture > textures;
	std::vector< Model > models;
};

