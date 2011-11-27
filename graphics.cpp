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
	glTexCoordPointer( 2, GL_FLOAT, 0, models[0].textureCoordinates );

	glDrawArrays( GL_TRIANGLES, 0, models[0].num );

	SDL_GL_SwapBuffers();
}


void Graphic::Messages()
{
	while( logistics.MessageCount( "graphic" ) )
	{
		Logistic::Message message( logistics.GetMessage( "graphic" ) );
		// TO DO: Do something
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
#undef READ

	Texture texture( info_head.image_size, info_head.width, info_head.height );

	in.seekg( head.offset );
	in.read( (char*)texture.t, info_head.image_size );

	in.close();
	return texture;
}

static void Add( std::string line, std::vector<float>& v )
{
	std::string number;
	for( int i(0); i < line.size(); i++ )
		if( line[i] == ' ' ) {
			v.push_back( atof( number.c_str() ) );
			number.clear();
		} else {
			number.append( 1, line[i] );
		}
	if( number != "" )
		v.push_back( atof( number.c_str() ) );
}
Graphic::Model Graphic::LoadObj( std::string name )
{
	std::fstream in;
	in.open( name.c_str(), std::ios::in );
	assert( in.is_open() );

	std::vector<float> vertexs, normals, textureCoordinates;
	std::vector<unsigned int> faces;

	while( !in.eof() )
	{
		std::string line;
		getline( in, line );

		if( line[0] == 'v' )
			if( line[1] == 't' )
				// Texture Coordinate
				Add( line.substr(3), textureCoordinates );
			else if( line[1] == 'n' )
				// Normal
				Add( line.substr(3), normals );
			else
				// Vertex
				Add( line.substr(2), vertexs );
		else if( line[0] == 'f' )
		{
			// Face
			std::string number;
			std::vector<unsigned int> temp;
			for( int i(2); i < line.size(); i++ )
				if( line[i] == ' ' || line[i] == '/' ) {
					temp.push_back( atoi( number.c_str() ) );
					number.clear();
				} else {
					number.append( 1, line[i] );
				}
			if( number != "" )
				temp.push_back( atof( number.c_str() ) );
			if( temp.size() == 3 * 3 ) {
				for( int i(0); i < temp.size(); i++ )
					faces.push_back( temp[i] );
			} else if( temp.size() == 4 * 3 ) {
				// TO DO: Splitt into 2 triangles
			} else {
				assert( 2 + 2 == 5 );
			}
		}
	}

	in.close();

	Model model( faces.size() / 3 );
	for( int i(0); i < faces.size() / 3; i++ )
	{
		for( int j(0); j < 3; j++ )
		{
			model.vertexs[ i * 3 + j ] = vertexs[ ( faces[ i * 3 + 0 ] - 1 ) * 3 + j ];
			model.normals[ i * 3 + j ] = normals[ ( faces[ i * 3 + 2 ] - 1 ) * 3 + j ];
		}
		for( int j(0); j < 2; j++ )
			model.textureCoordinates[ i * 2 + j ] = textureCoordinates[ ( faces[ i * 3 + 1 ] - 1 ) * 2 + j ];
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
		out << textureCoordinates[ i ] << "  ";
	}
	out.close(); */

	return model;
}

