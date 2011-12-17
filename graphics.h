#pragma once
#include "logistics.h"
#include "use_sdl.h"
#include "use_opengl.h"
#include "vector.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <vector>
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\matrix_inverse.hpp"

class Graphic
{
public:
	Graphic( Logistic& _logistics );
	~Graphic();
	void Initialize();
	void Update( Uint32 diffTime );
private:
	Logistic& logistics;
	void Messages();
	struct Object
	{
		Object() : texture(0), model(0) {}
		unsigned int texture, model;
	};
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
			textureCoordinates = new float[ size * 2 ];
		}
		~Model() {}

		int num;
		float* vertexs;
		float* normals;
		float* textureCoordinates;
	};
	enum ShaderType { Vertex, Fragment, Geometry };
	GLuint LoadShader( std::string name, ShaderType type );
	Texture LoadBmp( std::string name );
	Model LoadObj( std::string name );

	std::vector< Object > objects;
	std::vector< Texture > textures;
	std::vector< Model > models;
	glm::mat4 viewMatrix;
	GLuint shaderProgram;
};

