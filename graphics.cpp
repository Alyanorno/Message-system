#include "graphics.h"

Graphic::Graphic( Logistic& _logistics ) : logistics(_logistics)
{}

Graphic::~Graphic()
{}

void Graphic::Initialize()
{
	SDL_Init( SDL_INIT_VIDEO );

	SDL_SetVideoMode( 800, 600, 0, SDL_OPENGL | SDL_HWSURFACE );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	glHint( GL_POINT_SMOOTH_HINT,GL_NICEST );
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	glEnable( GL_DEPTH_TEST ); 
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_LINE_SMOOTH );
	glShadeModel( GL_SMOOTH );

	glViewport (0, 0, 800, 600);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glDepthFunc(GL_LEQUAL);

	glMatrixMode (GL_PROJECTION);
	glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 1000.0);
	glMatrixMode (GL_MODELVIEW);

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	textures.push_back( LoadBmp( "hej.bmp" ) );
	models.push_back( LoadObj( "hej.obj" ) );
}

void Graphic::Update( unsigned int diffTime )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glTranslatef( 10, 10, -100 );
	glRotatef( 0, 1.0, 0.0, 0.0 );
	glRotatef( 0, 0.0, 1.0, 0.0 );
	glRotatef( 0, 0.0, 0.0, 1.0 );
	
	GLuint texture;
	glGenTextures( 1, &texture );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glBindTexture( GL_TEXTURE_2D, texture );

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	glTexImage2D( GL_TEXTURE_2D, 0, 3, textures[0].width, textures[0].height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, textures[0].t );
	
	glBindTexture( GL_TEXTURE_2D, texture );

	glVertexPointer( 3, GL_FLOAT, 0, models[0].vertexs );
	glNormalPointer( GL_FLOAT, 0, models[0].normals );
	glTexCoordPointer( 2, GL_FLOAT, 0, models[0].texCoords );

	glDrawArrays( GL_TRIANGLES, 0, models[0].num );

	SDL_GL_SwapBuffers();
}


void Graphic::Messages()
{
	for( int i(0); i < logistics.messages.size(); i++ )
	{
		if( logistics.messages[i][0] == Logistic::graphic )
		{
			// Message for graphic
			// Do something
			logistics.messages.erase( logistics.messages.begin() + i);
		}
	}
}

Graphic::Texture Graphic::LoadBmp( std::string name )
{
	struct
	{
		unsigned short type;
		unsigned int size;
		unsigned short nothing1;
		unsigned short nothing2;
		unsigned int offset;
	} head;

	struct
	{
		unsigned int size;
		unsigned int width;
		unsigned int height;
		unsigned short one;
		unsigned short bit_per_pixel;
		unsigned int compression;
		unsigned int image_size;
		unsigned int pixels_per_meter_x;
		unsigned int pixels_per_meter_y;
		unsigned int colors_used;
		unsigned int important_colors;
	} info_head;

	std::fstream in;
	in.open( name.c_str(), std::ios::in | std::ios::binary );
	assert( in.is_open() );

#define READ( SIZE ) \
	in.read( (char*)&SIZE, sizeof(SIZE) );
	
	READ( head.type );
	READ( head.size );
	READ( head.nothing1 );
	READ( head.nothing2 );
	READ( head.offset );

	READ( info_head.size );
	READ( info_head.width );
	READ( info_head.height );
	READ( info_head.one );
	READ( info_head.bit_per_pixel );
	READ( info_head.compression );
	READ( info_head.image_size );
	READ( info_head.pixels_per_meter_x );
	READ( info_head.pixels_per_meter_y );
	READ( info_head.colors_used );
	READ( info_head.important_colors );

	Texture texture( info_head.image_size, info_head.width, info_head.height );

	in.seekg( head.offset );
	in.read( (char*)texture.t, info_head.image_size );

	in.close();
/*
	glGenTextures( 1, &texture );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glBindTexture( GL_TEXTURE_2D, texture );

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	glTexImage2D( GL_TEXTURE_2D, 0, 3, info_head.width, info_head.height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp ); */
	
	return texture;
}

Graphic::Model Graphic::LoadObj( std::string name )
{
	std::fstream in;
	in.open( name.c_str(), std::ios::in );
	assert( in.is_open() );

	int num_vertexs = 0, num_normals = 0, num_texCoords = 0, num_faces = 0;

	while( !in.eof() )
	{
		std::string line;
		getline( in, line );

		if( line[0] == 'v' )
		{
			if( line[1] == 't' )
				num_texCoords++;
			else if( line[1] == 'n' )
				num_normals++;
			else
				num_vertexs++;
		}
		else if( line[0] == 'f' )
			num_faces++;
	}

	in.close();

	float* tempVertexs = new float[ num_vertexs * 3 ];
	float* tempNormals = new float[ num_normals * 3 ];
	float* tempTexCoords = new float[ num_texCoords * 2 ];
	unsigned int* tempFaces = new unsigned int[ num_faces * 3 * 3 ];

	num_vertexs = num_normals = num_texCoords = num_faces = 0;

	in.clear();
	in.open( name.c_str(), std::ios::in );
	assert( in.is_open() );

	while( !in.eof() )
	{
		std::string line;
		getline( in, line );

		if( line[0] == 'v' )
		{
			if( line[1] == 't' )
			{
				std::string* numbers = new std::string[ line.size() ];

				int count = 0;
				for( int i(3); i <= line.size() && count < 2; i++ )
				{
					if( line[i] == ' ' )
						count++;
					else
						numbers[count].append( 1, line[i] );
				}
				assert( count + 1 == 2 ); \
				for( int i(0); i <= count; i++ ) \
					tempTexCoords[ num_texCoords * 2 + i ] = atof( numbers[i].c_str() );

				delete numbers;
				num_texCoords++; 
			}
			else if( line[1] == 'n' )
			{
				std::string* numbers = new std::string[ line.size() ];

				int count = 0;
				for( int i(3); i <= line.size() && count < 3; i++ )
				{
					if( line[i] == ' ' )
						count++;
					else
						numbers[count].append( 1, line[i] );
				}
				assert( count + 1 == 3 ); \
				for( int i(0); i <= count; i++ ) \
					tempNormals[ num_normals * 3 + i ] = atof( numbers[i].c_str() );

				delete numbers;
				num_normals++; 
			}
			else
			{
				std::string* numbers = new std::string[ line.size() ];

				int count = 0;
				for( int i(2); i <= line.size() && count < 3; i++ )
				{
					if( line[i] == ' ' )
						count++;
					else
						numbers[count].append( 1, line[i] );
				}
				assert( count + 1 == 3 ); \
				for( int i(0); i <= count; i++ ) \
					tempVertexs[ num_vertexs * 3 + i ] = atof( numbers[i].c_str() );

				delete numbers;
				num_vertexs++; 
			}
		}
		else if( line[0] == 'f' )
		{
			std::string number = "";
			int l(0);
			for( int i(2); i <= line.size(); i++ )
			{
				if( line[i] == ' ' )
				{
					tempFaces[ num_faces * 3 + l ] = atoi( number.c_str() ) - 1;
					number.clear();
					num_faces++;
					l = 0;
				}
				else if( line[i] == '/' )
				{
					tempFaces[ num_faces * 3 + l++ ] = atoi( number.c_str() ) - 1;
					number.clear();
				}
				else
					number.append( 1, line[i] );
			}
			if( line[ line.size() - 1 ] != ' ' )
			{
				tempFaces[ num_faces * 3 + l++ ] = atoi( number.c_str() ) - 1;
				num_faces++;
			}
		}
	}

	in.close();

	Model model( num_faces );
	for( int i(0), l(0), k(0); l < num_faces; i += 3, k += 2, l++ )
	{
		model.vertexs[ i + 0 ] = tempVertexs[ tempFaces[ i ] * 3 + 0 ];
		model.vertexs[ i + 1 ] = tempVertexs[ tempFaces[ i ] * 3 + 1 ];
		model.vertexs[ i + 2 ] = tempVertexs[ tempFaces[ i ] * 3 + 2 ];
		model.texCoords[ k + 0 ] = tempTexCoords[ tempFaces[ i + 1 ] * 2 + 0 ];
		model.texCoords[ k + 1 ] = tempTexCoords[ tempFaces[ i + 1 ] * 2 + 1 ];
		model.normals[ i + 0 ] = tempNormals[ tempFaces[ i + 2 ] * 3 + 0 ];
		model.normals[ i + 1 ] = tempNormals[ tempFaces[ i + 2 ] * 3 + 1 ];
		model.normals[ i + 2 ] = tempNormals[ tempFaces[ i + 2 ] * 3 + 2 ];
	}

	// For debugging
/*	ofstream out;
	out.open("something.txt");
	out << "Vertexs: " << endl;
	for( int i(0); i < num_obj * 3; i++ )
	{
		if( i % 3 == 0 )
			out << endl;
		out << vertexs[ i ] << "  ";
	}
	out << "Normals: " << endl;
	for( int i(0); i < num_obj * 3; i++ )
	{
		if( i % 3 == 0 )
			out << endl;
		out << normals[ i ] << "  ";
	}
	out << "Texture Coordinates: " << endl;
	for( int i(0); i < num_obj * 2; i++ )
	{
		if( i % 2 == 0 )
			out << endl;
		out << texCoords[ i ] << "  ";
	}
	out.close(); */

	delete tempVertexs;
	delete tempNormals;
	delete tempTexCoords;
	delete tempFaces;

	return model;
}

