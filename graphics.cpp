#include "graphics.h"

Graphic::Graphic( Logistic& _logistics ) : logistics(_logistics), viewMatrix( glm::mat4( 1 ) )
{}

Graphic::~Graphic()
{}

void Graphic::Initialize()
{
	SDL_Init( SDL_INIT_VIDEO );

	SDL_SetVideoMode( 800, 600, 0, SDL_OPENGL | SDL_HWSURFACE );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	std::cout
		<< "OpenGL on "
		<< glGetString(GL_VENDOR)
		<< glGetString(GL_RENDERER) << '\n';
	std::cout
		<< "OpenGL version supported "
		<< glGetString(GL_VERSION) << '\n';
	std::cout
		<< "GLSL version supported "
		<< glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';
	std::cout
		<< "Will now set GL to version "
		<< major << minor << '\n';

	
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


	glm::vec3 eye(0.0f, 0.0f, 2.0f), centre(0, 0, -1.0f), up(0.0f, 1.0f, 0.0f);
	viewMatrix = glm::lookAt(eye, centre, up);


	GLuint vertexShader = LoadShader( "shader.vertex", Vertex );
	GLuint fragmentShader = LoadShader( "shader.fragment", Fragment );

	GLuint shaderProgram = glCreateProgram();

	glAttachShader( shaderProgram, vertexShader );
	glAttachShader( shaderProgram, fragmentShader );

	glLinkProgram( shaderProgram );
}

void Graphic::Update( unsigned int diffTime )
{
	Messages();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glUseProgram( shaderProgram );

	float angle = 90; // degres
	glm::mat4 modelMatrix( glm::mat4( 1.0f ) );
	modelMatrix = glm::rotate( modelMatrix, angle, glm::vec3( 0.0f, 1.0f, 0.0f ) );

	glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
	glUniformMatrix4fv( glGetUniformLocation( shaderProgram, "modelViewMatrix" ), 1, GL_FALSE, &modelViewMatrix[0][0] );

	glm::mat3 tempMatrix = glm::inverseTranspose( (glm::mat3)modelViewMatrix );
	glUniformMatrix3fv( glGetUniformLocation( shaderProgram, "normalInverseTranspose"), 1, GL_FALSE, &tempMatrix[0][0] );

	/// handle the light position
	glm::vec4 lightPosition( -1.0f, 1.0f, 0.0f, 1.0f );
	lightPosition = viewMatrix * lightPosition;
	glUniform1fv( glGetUniformLocation( shaderProgram, "lightPosition"), 1, &lightPosition[0] );

	// TEXTURE
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
	// END TEXTURE

	// MODEL
	GLuint Vbo[2];
	glGenBuffers(2, Vbo);

	int size = models[0].num * sizeof( float );

	// Vertex, normal, texture
	glBindBuffer(GL_ARRAY_BUFFER, Vbo[0]); 
	glBufferData(GL_ARRAY_BUFFER, size * 3, models[0].vertexs, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, Vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, size * 3, models[0].normals, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, Vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, size * 2, models[0].textureCoordinates, GL_STATIC_DRAW);

	// create 1 VAO
	GLuint Vao;
	glGenVertexArrays(1, &Vao);
	glBindVertexArray(Vao);

	// Vertex, normal, texture
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	GLubyte* null = 0;

	glBindBuffer(GL_ARRAY_BUFFER, Vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, null);

	glBindBuffer(GL_ARRAY_BUFFER, Vbo[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, null);

	glBindBuffer(GL_ARRAY_BUFFER, Vbo[2]);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, null);

	glBindVertexArray(Vao);
	glDrawArrays( GL_TRIANGLES, 0, models[0].num );
	// END MODEL

	glUseProgram(0);
	glBindVertexArray(0);

	SDL_GL_SwapBuffers();
}


void Graphic::Messages()
{
	while( logistics.MessageCount( "graphic" ) )
	{
		Logistic::Message message( logistics.GetMessage( "graphic" ) );
		
		if( message.info == "load" ) {
			std::string r = message.ReceiveMessage< std::string >();
			if( r.find( ".bmp" ) != std::string::npos ) {
				textures.push_back( LoadBmp( r ) );
			} else if ( r.find( ".obj" ) != std::string::npos ) {
				models.push_back( LoadObj( r ) );
			} else {
				throw std::string( "Cannot load file format!" );
			}
		}
		else
			// Must call ReceiveMessage for memory to be deleted
			throw std::string( "Must receive message!" );
	}
}


GLuint Graphic::LoadShader( std::string name, ShaderType type )
{
	std::fstream in( name.c_str() );
	std::string program( ( std::istreambuf_iterator<char>( in ) ), std::istreambuf_iterator<char>() );
	in.close();

	GLuint shader;
	switch( type )
	{
		case Vertex:
			shader = glCreateShader( GL_VERTEX_SHADER );
			break;
		case Fragment:
			shader = glCreateShader( GL_FRAGMENT_SHADER );
			break;
		case Geometry:
			shader = glCreateShader(GL_GEOMETRY_SHADER);
			break;
	}

	const char* ptr = program.c_str();
	glShaderSource( shader, 1, &ptr, 0 );
	glCompileShader( shader );

	return shader;
}


template < class T >
void Read( T& size, std::fstream& in ) 
	{ in.read( (char*)&size, sizeof(T) ); }
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

	Read( head.type, in );
	Read( head.size, in );
	Read( head.nothing1, in );
	Read( head.nothing2, in );
	Read( head.offset, in );

	Read( info_head.size, in );
	Read( info_head.width, in );
	Read( info_head.height, in );
	Read( info_head.one, in );
	Read( info_head.bit_per_pixel, in );
	Read( info_head.compression, in );
	Read( info_head.image_size, in );
	Read( info_head.pixels_per_meter_x, in );
	Read( info_head.pixels_per_meter_y, in );
	Read( info_head.colors_used, in );
	Read( info_head.important_colors, in );

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

