/********************************************
Name: Kenny Moser	krm104@gmail.com
Course: ECE8990
File:	glsl.cpp

This source file contains the definitions of the gl_Shader
class. As well as helper/friend functions

It is built off of the glew and freeglut libraries.

Use all you like, but if you break it, don't reference me >.>
**************************************************************/


/*************************************
Neccessary Includes
*************************************/
#include "glsl.h"


/**********************************\
Additional Includes
**********************************/


/*********************************************
gl_Program Static Member Instantiation
*********************************************/
GLuint gl_Shader::active_handle = 0;


/**********************************************
gl_Program Class Method Definitions
**********************************************/
/**************************************
Static Deactivate
**************************************/
bool gl_Shader::Deactivate_Current( )
{
    glUseProgram( 0 );

    active_handle = 0;

    return true;
}


/**************************************
Standard Constructor
**************************************/
gl_Shader::gl_Shader()
{
    vert_shader_handle = 0;
    frag_shader_handle = 0;
    prog_handle = 0;
}


/**************************************
Parameter Constructor
**************************************/
gl_Shader::gl_Shader( GLchar* vert, GLchar* frag )
{
    vert_shader_handle = 0;
    frag_shader_handle = 0;
    prog_handle = 0;

    create_shaders(vert, frag);
}


/*************************************
Standard Destructor
*************************************/
gl_Shader::~gl_Shader()
{
    if ( active_handle == prog_handle )
        glUseProgram( 0 );

    active_handle = 0;

    prog_handle = 0;
    vert_shader_handle = 0;
    frag_shader_handle = 0;
}


/*************************************************
Creats vertex and fragment shaders based on the 
passed in string to the shader files. They are
opened, read, compiled, linked, and attached
to a GLProgram object and made ready for use.
The return value specifies an error code.
-1 program error
-2 vertex shader error
-3 fragment shader error
*************************************************/
GLint gl_Shader::create_shaders( GLchar* vert, GLchar* frag )
{
    //Create program object
    if ( !prog_handle )
    {
        //Create program object
        prog_handle = glCreateProgram( );
        if ( !prog_handle )
            return -1;
    }

    //misc flags
    GLint flag = 0;

    //Get handles to new shader objects
    vert_shader_handle = glCreateShader(GL_VERTEX_SHADER);
    frag_shader_handle = glCreateShader(GL_FRAGMENT_SHADER);

    if ( !vert_shader_handle )
        return -2;

    if ( !frag_shader_handle )
        return -3;

    const GLchar* vertText = readShader(vert);
    if ( !vertText )
        return -2;

    const GLchar* fragText = readShader(frag);
    if ( !fragText )
        return -3;

    //Create shader source on GPU
    glShaderSource(vert_shader_handle, 1, &vertText, NULL);
    glShaderSource(frag_shader_handle, 1, &fragText, NULL);

    //Compile the vertex shader
    glCompileShader(vert_shader_handle);
    printOpenGLError( __FILE__, __LINE__ );
    glGetShaderiv(vert_shader_handle, GL_COMPILE_STATUS, &flag);
    if ( !flag )
    {
        printShaderInfoLog( 0 );
        return -2;
    }

    //Compile the fragment shader
    glCompileShader(frag_shader_handle);
    printOpenGLError( __FILE__, __LINE__ );
    glGetShaderiv(frag_shader_handle, GL_COMPILE_STATUS, &flag);
    if ( !flag )
    {
        printShaderInfoLog( 1 );
        return -3;
    }


    //attach shaders to program
    glAttachShader(prog_handle, vert_shader_handle);
    glAttachShader(prog_handle, frag_shader_handle);

    // Link the shader in the program object
    glLinkProgram(prog_handle);
    printOpenGLError( __FILE__, __LINE__ );
    glGetProgramiv(prog_handle, GL_LINK_STATUS, &flag);
    printProgramInfoLog( );

    if ( !flag )
        return -1;

    return 0;
}


/*********************************************
Sets the active glProgram instance to the one
represented by the handle "program_handle".
Checks for valid program and shader handles.
*********************************************/
bool gl_Shader::Activate()
{
    if ( !prog_handle || ( !vert_shader_handle || !frag_shader_handle ) )
        return false;

    glUseProgram(prog_handle);

    active_handle = prog_handle;

    return true;
}


/**********************************************
Sets the active glProgram instance to the
default GL pipeline.
**********************************************/
bool gl_Shader::DeActivate()
{
    if ( active_handle != prog_handle )
        return false;

    glUseProgram(0);

    active_handle = 0;

    return true;
}


/************************************************
Determines if the calling shader is the
active shader.
************************************************/
bool gl_Shader::Is_Active()
{
    return (active_handle == prog_handle) && (prog_handle != 0);
}

/********************************************************
Set float uniform variables
********************************************************/
/*****************************************************
Set value of a uniform float
*****************************************************/
bool gl_Shader::set_uniform_1f( const GLchar* var, float v1 )
{
    if ( active_handle != prog_handle )
        return false;

    glUniform1f(glGetUniformLocation(prog_handle, var), v1);
    
    return true;
}


/*****************************************************
Set value of a uniform float vec2 
*****************************************************/
bool gl_Shader::set_uniform_2f( const GLchar* var, float v1, float v2 )
{
    if ( active_handle != prog_handle )
        return false;

    glUniform2f(glGetUniformLocation(prog_handle, var), v1, v2);

    return true;
}


/*****************************************************
Set value of a uniform float vec3
*****************************************************/
bool gl_Shader::set_uniform_3f( const GLchar* var, float v1, float v2, float v3 )
{
    if ( active_handle != prog_handle )
        return false;

    glUniform3f(glGetUniformLocation(prog_handle, var), v1, v2, v3);

    return true;
}


/*****************************************************
Set value of a uniform float vec4
*****************************************************/
bool gl_Shader::set_uniform_4f( const GLchar* var, float v1, float v2, float v3, float v4 )
{
    if ( active_handle != prog_handle )
        return false;

    glUniform4f(glGetUniformLocation(prog_handle, var), v1, v2, v3, v4);

    return true;
}


/****************************************************
Set integer uniform variables
****************************************************/
/*****************************************************
Set value of a uniform int
*****************************************************/
bool gl_Shader::set_uniform_1i( const GLchar* var, int v1 )
{
    if ( active_handle != prog_handle )
        return false;

    glUniform1i(glGetUniformLocation(prog_handle, var), v1);

    return true;
}


/*****************************************************
Set value of a uniform int vec2
*****************************************************/
bool gl_Shader::set_uniform_2i( const GLchar* var, int v1, int v2 )
{
    if ( active_handle != prog_handle )
        return false;

    glUniform2i(glGetUniformLocation(prog_handle, var), v1, v2);

    return true;
}


/*****************************************************
Set value of a uniform int vec3
*****************************************************/
bool gl_Shader::set_uniform_3i( const GLchar* var, int v1, int v2, int v3 )
{
    if ( active_handle != prog_handle )
        return false;

    glUniform3i(glGetUniformLocation(prog_handle, var), v1, v2, v3);

    return true;
}


/*****************************************************
Set value of a uniform int vec4
*****************************************************/
bool gl_Shader::set_uniform_4i( const GLchar* var, int v1, int v2, int v3, int v4 )
{
    if ( active_handle != prog_handle )
        return false;

    glUniform4i( glGetUniformLocation( prog_handle, var ), v1, v2, v3, v3 );

    return true;
}


/*******************************************************
Set value of uniform sampler
*******************************************************/
bool gl_Shader::set_uniform_sampler_2d( const GLchar* var, int tex_ident )
{
    if ( active_handle != prog_handle )
        return false;

    glUniform1i( glGetUniformLocation( prog_handle, var ), tex_ident );

    return true;
}


/****************************************************
Bind an Attribute to the Shader
****************************************************/
bool gl_Shader::bind_attribute( const GLchar* name, GLuint attr_handle )
{
    if ( !prog_handle )
    {
        //Create program object
        prog_handle = glCreateProgram( );
        if ( !prog_handle )
            return false;
    }

    glBindAttribLocationARB( prog_handle, attr_handle, name );

    return true;
}


/******************************************
Print the information log for a shader
0 = vert shader; 1 = frag shader
******************************************/
void gl_Shader::printShaderInfoLog( short shader )
{
    int infologLength = 0;
    int charsWritten  = 0;
    GLchar *infoLog;

    printOpenGLError( __FILE__, __LINE__ );  // Check for OpenGL errors

    GLuint shader_handle = 0;

    switch ( shader)
    {
        case 0:
            shader_handle = vert_shader_handle;
            break;

        case 1:
            shader_handle = frag_shader_handle;
            break;

        default:
            break;
    }

    glGetShaderiv( shader_handle, GL_INFO_LOG_LENGTH, &infologLength );

    printOpenGLError( __FILE__, __LINE__ );  // Check for OpenGL errors

    if (infologLength > 0)
    {
        infoLog = (GLchar *)malloc(infologLength);
        if (infoLog == NULL)
        {
            printf("ERROR: Could not allocate InfoLog buffer\n");
            exit(1);
        }
        glGetShaderInfoLog( shader_handle, infologLength, &charsWritten, infoLog );
        printf("Shader InfoLog:\n%s\n\n", infoLog);
        free( infoLog );
    }

    printOpenGLError( __FILE__, __LINE__ );
}


/********************************************
Print the information log for the program
********************************************/
void gl_Shader::printProgramInfoLog( )
{
    int infologLength = 0;
    int charsWritten  = 0;
    GLchar *infoLog;

    printOpenGLError( __FILE__, __LINE__ );  // Check for OpenGL errors

    glGetProgramiv(prog_handle, GL_INFO_LOG_LENGTH, &infologLength);

    printOpenGLError( __FILE__, __LINE__ );  // Check for OpenGL errors

    if (infologLength > 0)
    {
        infoLog = (GLchar *)malloc(infologLength);
        if ( infoLog == NULL )
        {
            printf( "ERROR: Could not allocate InfoLog buffer\n" );
            exit( 1 );
        }
        glGetProgramInfoLog( prog_handle, infologLength, &charsWritten, infoLog );
        printf( "Program InfoLog:\n%s\n\n", infoLog );
        free( infoLog );
    }
    printOpenGLError( __FILE__, __LINE__ );
}


/********************************************
Print the error logs for the shader
********************************************/
int gl_Shader::printOpenGLError( char* file, int line )
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}


/****************************************
TEXTURE Object Method Definitions
****************************************/
/***********************************
Default Constructor
***********************************/
TEXTURE_2D_RGB::TEXTURE_2D_RGB( )    
{
    //rgb_array = NULL;
    texture_buffer_handle = 0;
    width = height = 0;
}    


/***********************************
Parameter Constructor
***********************************/
TEXTURE_2D_RGB::TEXTURE_2D_RGB( char* image_file )
{
    //rgb_array = NULL;
    texture_buffer_handle = 0;
    width = height = 0;

    LoadBitmapFile( image_file );
}


/*********************************
Default Destructor
*********************************/
TEXTURE_2D_RGB::~TEXTURE_2D_RGB( )
{
    /*
    if ( rgb_array )
        delete [] rgb_array;
 
    rgb_array = NULL;
    */

    texture_buffer_handle = 0;
    width = height = 0;
}


/**********************************************
Load Bitmap File from RGB TEXTURE_2D_RGB struct 
**********************************************/
bool TEXTURE_2D_RGB::LoadBitmapFile( char *filename )
{
    FILE *filePtr; //our file pointer
    L_BITMAPINFOHEADER bitmapInfoHeader; //bitmap infor header
    L_BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header
    
    //unsigned char *bitmapImage; //store image data
    
    int imageIdx = 0; //image index counter
    unsigned char tempRGB; //our swap variable

    //open filename in read binary mode
    filePtr = fopen( filename, "rb" );
    if ( filePtr == NULL )
        return false;

    //read the bitmap file header
    fread( &bitmapFileHeader.bfType, sizeof( unsigned short int ), 1, filePtr );
    fread( &bitmapFileHeader.bfSize, sizeof( unsigned int), 1, filePtr );
    fread( &bitmapFileHeader.bfReserved1, sizeof( unsigned short int), 1, filePtr );
    fread( &bitmapFileHeader.bfReserved2, sizeof( unsigned short int), 1, filePtr );
    fread( &bitmapFileHeader.bfOffBits, sizeof( unsigned int), 1, filePtr );

    //verify that this is a bmp file by check bitmap id
    if ( bitmapFileHeader.bfType != 0x4D42 )
    {
        fclose( filePtr );
        return false;
    }

    //read the bitmap info header
    fread( &bitmapInfoHeader.biSize, sizeof( unsigned int ), 1, filePtr );
    fread( &bitmapInfoHeader.biWidth, sizeof( int ), 1, filePtr );
    fread( &bitmapInfoHeader.biHeight, sizeof( int ), 1, filePtr );
    fread( &bitmapInfoHeader.biPlanes, sizeof( unsigned short int), 1, filePtr );
    fread( &bitmapInfoHeader.biBitCount, sizeof( unsigned short int), 1, filePtr );
    fread( &bitmapInfoHeader.biCompression, sizeof( unsigned int ), 1, filePtr );
    fread( &bitmapInfoHeader.biSizeImage, sizeof( unsigned int ), 1, filePtr );
    fread( &bitmapInfoHeader.biXPelsPerMeter, sizeof( int ), 1, filePtr );
    fread( &bitmapInfoHeader.biYPelsPerMeter, sizeof( int ), 1, filePtr );
    fread( &bitmapInfoHeader.biClrUsed, sizeof( unsigned int ), 1, filePtr );
    fread( &bitmapInfoHeader.biClrImportant, sizeof( unsigned int ), 1, filePtr );

    //move file point to the beggining of bitmap data
    fseek( filePtr, bitmapFileHeader.bfOffBits, SEEK_SET );

    //allocate enough memory for the bitmap image data
    rgb_array.resize( bitmapInfoHeader.biWidth*bitmapInfoHeader.biHeight*3 );

    //read in the bitmap image data
    fread( &rgb_array[0], sizeof( char ), rgb_array.size( ), filePtr );

    width = bitmapInfoHeader.biWidth;
    height = bitmapInfoHeader.biHeight;

    //swap the r and b values to get RGB (bitmap is BGR)
    for ( imageIdx = 0; imageIdx < rgb_array.size( ); imageIdx += 3 )
    {
        tempRGB = rgb_array[imageIdx];
        rgb_array[imageIdx] = rgb_array[imageIdx + 2];
        rgb_array[imageIdx + 2] = tempRGB;
    }

    //close file and return bitmap iamge data
    fclose( filePtr );

    return true;
}


/**********************************
TEXTURE_CUBE Methods
**********************************/
/***********************************
Default Constructor
***********************************/
TEXTURE_CUBE::TEXTURE_CUBE( )    
{
    //rgb_array = NULL;
    texture_buffer_handle = 0;
    width = height = 0;
}    


/***********************************
Parameter Constructor
***********************************/
TEXTURE_CUBE::TEXTURE_CUBE( char* image_file )
{
    //rgb_array = NULL;
    texture_buffer_handle = 0;
    width = height = 0;

    LoadBitmapFile( image_file );
}


/*********************************
Default Destructor
*********************************/
TEXTURE_CUBE::~TEXTURE_CUBE( )
{
    /*
    if ( rgb_array )
        delete [] rgb_array;
 
    rgb_array = NULL;
    */

    texture_buffer_handle = 0;
    width = height = 0;
}


/**********************************************
Load Bitmap File from RGB TEXTURE_CUBE struct 
**********************************************/
bool TEXTURE_CUBE::LoadBitmapFile( char *filename )
{
    FILE *filePtr; //our file pointer
    L_BITMAPINFOHEADER bitmapInfoHeader; //bitmap infor header
    L_BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header
    
    //unsigned char *bitmapImage; //store image data
    
    int imageIdx = 0; //image index counter
    unsigned char tempRGB; //our swap variable

    //open filename in read binary mode
    filePtr = fopen( filename, "rb" );
    if ( filePtr == NULL )
        return false;

    //read the bitmap file header
    fread( &bitmapFileHeader.bfType, sizeof( unsigned short int ), 1, filePtr );
    fread( &bitmapFileHeader.bfSize, sizeof( unsigned int), 1, filePtr );
    fread( &bitmapFileHeader.bfReserved1, sizeof( unsigned short int), 1, filePtr );
    fread( &bitmapFileHeader.bfReserved2, sizeof( unsigned short int), 1, filePtr );
    fread( &bitmapFileHeader.bfOffBits, sizeof( unsigned int), 1, filePtr );

    //verify that this is a bmp file by check bitmap id
    if ( bitmapFileHeader.bfType != 0x4D42 )
    {
        fclose( filePtr );
        return false;
    }

    //read the bitmap info header
    fread( &bitmapInfoHeader.biSize, sizeof( unsigned int ), 1, filePtr );
    fread( &bitmapInfoHeader.biWidth, sizeof( int ), 1, filePtr );
    fread( &bitmapInfoHeader.biHeight, sizeof( int ), 1, filePtr );
    fread( &bitmapInfoHeader.biPlanes, sizeof( unsigned short int), 1, filePtr );
    fread( &bitmapInfoHeader.biBitCount, sizeof( unsigned short int), 1, filePtr );
    fread( &bitmapInfoHeader.biCompression, sizeof( unsigned int ), 1, filePtr );
    fread( &bitmapInfoHeader.biSizeImage, sizeof( unsigned int ), 1, filePtr );
    fread( &bitmapInfoHeader.biXPelsPerMeter, sizeof( int ), 1, filePtr );
    fread( &bitmapInfoHeader.biYPelsPerMeter, sizeof( int ), 1, filePtr );
    fread( &bitmapInfoHeader.biClrUsed, sizeof( unsigned int ), 1, filePtr );
    fread( &bitmapInfoHeader.biClrImportant, sizeof( unsigned int ), 1, filePtr );

    //move file point to the beggining of bitmap data
    fseek( filePtr, bitmapFileHeader.bfOffBits, SEEK_SET );

    //Create Temporary Storage
    vector<GLubyte> temp_rgb_array;

    //allocate enough memory for the bitmap image data
    temp_rgb_array.resize( bitmapInfoHeader.biWidth*bitmapInfoHeader.biHeight*3 );

    //read in the bitmap image data
    fread( &temp_rgb_array[0], sizeof( char ), temp_rgb_array.size( ), filePtr );

    width = bitmapInfoHeader.biWidth;
    height = bitmapInfoHeader.biHeight;

    //swap the r and b values to get RGB (bitmap is BGR)
    for ( imageIdx = 0; imageIdx < temp_rgb_array.size( ); imageIdx += 3 )
    {
        tempRGB = temp_rgb_array[imageIdx];
        temp_rgb_array[imageIdx] = temp_rgb_array[imageIdx + 2];
        temp_rgb_array[imageIdx + 2] = tempRGB;
    }

    unsigned long face_width = width/4;
    unsigned long face_height = height/3;

    //Top Face Map
    for ( unsigned int fh = 0; fh < face_height; fh++ )
    {
        for ( unsigned int fw = 0; fw < face_width; fw++ )
        {
            rgb_array.push_back( temp_rgb_array[ fh * width * 3 + face_width *3 + fw*3 ] );
            rgb_array.push_back( temp_rgb_array[ fh * width * 3 + face_width *3 + fw*3 + 1] );
            rgb_array.push_back( temp_rgb_array[ fh * width * 3 + face_width *3 + fw*3 + 2] );
        }
    }

    //Left Face Map
    for ( unsigned int fh = 0; fh < face_height; fh++ )
    {
        for ( unsigned int fw = 0; fw < face_width; fw++ )
        {
            rgb_array.push_back( temp_rgb_array[ face_height * width * 3 + fh * width * 3 + fw*3 ] );
            rgb_array.push_back( temp_rgb_array[ face_height * width * 3 + fh * width * 3 + fw*3 + 1] );
            rgb_array.push_back( temp_rgb_array[ face_height * width * 3 + fh * width * 3 + fw*3 + 2] );
        }
    }

    //Front Face Map
    for ( unsigned int fh = 0; fh < face_height; fh++ )
    {
        for ( unsigned int fw = 0; fw < face_width; fw++ )
        {
            rgb_array.push_back( temp_rgb_array[ face_height * width * 3 + fh * width * 3 + face_width * 3 + fw*3 ] );
            rgb_array.push_back( temp_rgb_array[ face_height * width * 3 + fh * width * 3 + face_width * 3 + fw*3 + 1] );
            rgb_array.push_back( temp_rgb_array[ face_height * width * 3 + fh * width * 3 + face_width * 3 + fw*3 + 2] );
        }
    }

    //Right Face Map
    for ( unsigned int fh = 0; fh < face_height; fh++ )
    {
        for ( unsigned int fw = 0; fw < face_width; fw++ )
        {
            rgb_array.push_back( temp_rgb_array[ face_height * width * 3 + fh * width * 3 + face_width * 2 * 3 + fw*3 ] );
            rgb_array.push_back( temp_rgb_array[ face_height * width * 3 + fh * width * 3 + face_width * 2 * 3 + fw*3 + 1] );
            rgb_array.push_back( temp_rgb_array[ face_height * width * 3 + fh * width * 3 + face_width * 2 * 3 + fw*3 + 2] );
        }
    }

    //Back Face Map
    for ( unsigned int fh = 0; fh < face_height; fh++ )
    {
        for ( unsigned int fw = 0; fw < face_width; fw++ )
        {
            rgb_array.push_back( temp_rgb_array[ face_height * width * 3 + fh * width * 3 + face_width * 3 * 3 + fw*3 ] );
            rgb_array.push_back( temp_rgb_array[ face_height * width * 3 + fh * width * 3 + face_width * 3 * 3 + fw*3 + 1] );
            rgb_array.push_back( temp_rgb_array[ face_height * width * 3 + fh * width * 3 + face_width * 3 * 3 + fw*3 + 2] );
        }
    }

    //Bottom Face Map
    for ( unsigned int fh = 0; fh < face_height; fh++ )
    {
        for ( unsigned int fw = 0; fw < face_width; fw++ )
        {
            rgb_array.push_back( temp_rgb_array[ face_height * 2 * width * 3 + fh * width * 3 + face_width * 3 + fw*3 ] );
            rgb_array.push_back( temp_rgb_array[ face_height * 2 * width * 3 + fh * width * 3 + face_width * 3 + fw*3 + 1] );
            rgb_array.push_back( temp_rgb_array[ face_height * 2 * width * 3 + fh * width * 3 + face_width * 3 + fw*3 + 2] );
        }
    }

    width = face_width;
    height = face_height;

    //close file and return bitmap immge data
    fclose( filePtr );

    return true;
}


/**************************************************
Helper functions
**************************************************/
/***************************************************
Returns the length of a text (shader) file in bytes
Returns -1 on error

Original code is from 3dshaders.com
(modified code follows)
***************************************************/
int shaderSize( GLchar* fileName )
{
    int fd;
    int count = -1;

    // Open the file, seek to the end to find its length
    #ifdef WIN32 /*[*/
        fd = _open(fileName, _O_RDONLY);
        if (fd != -1)
        {
            count = _lseek(fd, 0, SEEK_END) + 1;
            _close(fd);
        }
    #else /*][*/
        fd = open(fileName, O_RDONLY);
        if (fd != -1)
        {
            count = lseek(fd, 0, SEEK_END) + 1;
            close(fd);
        }
    #endif /*]*/

    return count;
}


/********************************************************
Reads in a text (shader) source file, and saves it as
a string of characters.
Returns pointer to the char array on success
Returns NULL on error
********************************************************/
GLchar* readShader( GLchar* fileName )
{
    FILE *fh;
    int count;

    fh = fopen(fileName, "r");
    if (!fh)
        return NULL;

    count = shaderSize( fileName );

    GLchar* shaderText = (GLchar *) malloc(count);
    
    // Get the shader from a file.
    fseek(fh, 0, SEEK_SET);
    count = (int) fread(shaderText, 1, count, fh);
    
    shaderText[count] = '\0';

    //Check for error
    if (ferror(fh))
    {
        free(shaderText);
        return NULL;
    }

    fclose(fh);

    return shaderText;
}

