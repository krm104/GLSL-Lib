/*************************************************************
Name: Kenny Moser	krm104@gmail.com
Course: ECE8990
File:	glsl.h

This source file contains the prototypes of the gl_Shader
class. As well as helper/friend functions.

It is built off of the glew and freeglut libraries.

Use all you like, but if you break it, don't reference me >.>
*************************************************************/


//duplicate include lock
#ifndef __GLSL_H__
#define __GLSL_H__


/*************************************************
Required Includes
*************************************************/

#define GLEW_STATIC 1
//gl extension wrangler
#include <GL/glew.h>    

//#define USE_FREEGLUT

//Determine glut library to use
#ifdef USE_FREEGLUT
    #include <GL/freeglut.h>
#else
    #include <GL/glut.h>
#endif


/**********************************************
additional includes
**********************************************/
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <cmath>

#include <iostream>

#ifdef _WIN32
    #include <io.h>
    typedef struct tagL_BITMAPFILEHEADER {
        unsigned short int  bfType;
        unsigned int bfSize;
        unsigned short int  bfReserved1;
        unsigned short int  bfReserved2;
        unsigned int   bfOffBits;
    } L_BITMAPFILEHEADER;

    typedef struct tagL_BITMAPINFOHEADER {
        unsigned int biSize;
        int  biWidth;
        int  biHeight;
        unsigned short int biPlanes;
        unsigned short int  biBitCount;
        unsigned int biCompression;
        unsigned int biSizeImage;
        int  biXPelsPerMeter;
        int  biYPelsPerMeter;
        unsigned int biClrUsed;
        unsigned int biClrImportant;
    } L_BITMAPINFOHEADER;
#else
    #include <cstdio>
    typedef struct tagL_BITMAPFILEHEADER {
        unsigned short int  bfType;
        unsigned int bfSize;
        unsigned short int  bfReserved1;
        unsigned short int  bfReserved2;
        unsigned int   bfOffBits;
    } L_BITMAPFILEHEADER;

    typedef struct tagL_BITMAPINFOHEADER {
        unsigned int biSize;
        int  biWidth;
        int  biHeight;
        unsigned short int biPlanes;
        unsigned short int  biBitCount;
        unsigned int biCompression;
        unsigned int biSizeImage;
        int  biXPelsPerMeter;
        int  biYPelsPerMeter;
        unsigned int biClrUsed;
        unsigned int biClrImportant;
    } L_BITMAPINFOHEADER;
#endif

using namespace std;


/***********************************************************************
The gl_Shader class facilitates the creation of gl shaders that can be
linked together to form a shader program. It provides methods for
reading in shader source code, compiling the shader, linking shaders,
activating the program to use, as well as modifying uniform variables
to the shader and printing the logs.
***********************************************************************/
class gl_Shader
{
    //public members and methods
    public:
        //Constructor
        gl_Shader();

        //Parameter Constructor
        gl_Shader( GLchar* vert, GLchar* frag );

        //Destructor
        ~gl_Shader();

        //Read in shader source code for compiling/linking/attaching
        GLint create_shaders( GLchar* vert, GLchar* frag );

        //Activate the shader/group for use
        bool Activate();

        //Implicit deactivation of a shader (return to default GL pipeline if
        //Shader is currently active, otherwise no change)
        bool DeActivate();

        //Determine if the calling shader is the active shader
        bool Is_Active();

        //Deactivates current shader (global, without implicit deactivation)
        static bool Deactivate_Current( );

        //Set a uniform float value in shader
        bool set_uniform_1f( const GLchar* var, float v1 );

        //Set a uniform float vec2 value in shader
        bool set_uniform_2f( const GLchar* var, float v1, float v2 );

        //Set a uniform float vec3 value in shader
        bool set_uniform_3f( const GLchar* var, float v1, float v2, float v3 );

        //Set a uniform float vec4 value in shader
        bool set_uniform_4f( const GLchar* var, float v1, float v2, float v3, float v4 );

        //Set a uniform int value in shader
        bool set_uniform_1i( const GLchar* var, int v1 );

        //Set a uniform int vec2 value in shader
        bool set_uniform_2i( const GLchar* var, int v1, int v2 );

        //Set a uniform int vec3 value in shader
        bool set_uniform_3i( const GLchar* var, int v1, int v2, int v3 );

        //Set a uniform int vec4 value in shader
        bool set_uniform_4i( const GLchar* var, int v1, int v2, int v3, int v4 );

        //Set a uniform sampler 2d value in the shader
        bool set_uniform_sampler_2d( const GLchar* var, int tex_ident );

        //Bind an attribute location
        bool bind_attribute( const GLchar* name, GLuint atr_handle );

        //Print the information log for a shader
        void printShaderInfoLog( short shader );

        //Print the information log for the program
        void printProgramInfoLog( );

    //protected members and methods
    protected:

        //Print the error log
        int printOpenGLError( char* file, int line );

        GLuint prog_handle;
        GLuint vert_shader_handle;
        GLuint frag_shader_handle;

        static GLuint active_handle;

    //private members and methods
    private:


};


/**********************
RGB Texture Object
**********************/
struct TEXTURE_2D_RGB
{
    //Data Members
    vector<GLubyte> rgb_array; //color data
    GLuint texture_buffer_handle; //texture buffer handle
    unsigned long width; //texture width
    unsigned long height; //texture height

    //Default Constructor
    TEXTURE_2D_RGB();

    //Parameter Constructor
    TEXTURE_2D_RGB( char* image_file );
 
    //Default Destructor
    ~TEXTURE_2D_RGB();

    bool LoadBitmapFile( char *filename );
};


/***********************
RGBA Texture Object
***********************/
struct TEXTURE_CUBE
{
    //Data Members
    vector<GLubyte> rgb_array; //color data
    GLuint texture_buffer_handle; //texture buffer handle
    unsigned long width; //texture width
    unsigned long height; //texture height

    //Default Constructor
    TEXTURE_CUBE();

    //Parameter Constructor
    TEXTURE_CUBE( char* image_file );
 
    //Default Destructor
    ~TEXTURE_CUBE();

    bool LoadBitmapFile( char *filename );
};


/*************************************************
Additional Helper Functions
*************************************************/
/*******************************************
Reads in a text (shader) file to determine
its length in bytes.
*******************************************/
int shaderSize( GLchar* fileName );


/*******************************************
Reads in a text (shader) file and stores
the contents in a GLchar array.
Returns a pointer to the array on success.
Returns NULL on error.
*******************************************/
GLchar* readShader( GLchar* fileName );


#endif //__GLSL_H__


