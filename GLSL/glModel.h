/***************************************************************
Name: Kenny Moser	krm104@gmail.com
Course: ECE8990
File:   glModel.h

This source file contains the prototypes of the gl_model
class. As well as helper/friend functions.

Use all you like, but if you break it, don't reference me >.>

***************************************************************/


//duplicate include lock
#ifndef __GL_MODEL_H__
#define __GL_MODEL_H__


/****************************
Required Includes
****************************/
#include "glsl.h"

#include <vector>
#include <map>
#include <string>
#include <fstream>

using namespace std;


/***************************
Additional Includes
***************************/


/*******************************************
Model Object
Contains the data members and methods for
loading and manipulating a model
*******************************************/
class gl_Model
{   
    /*
    Struct that represents a model material
    */
    struct MATERIAL
    {
        vector<string> textures;
        vector<string> cubetextures;
        vector<GLuint> attrib_handle;
        string bump;
        string shader;
        float Ns;
        float Ka_x, Ka_y, Ka_z;
        float Kd_x, Kd_y, Kd_z;
        float Ks_x, Ks_y, Ks_z;
        float Ni;
        float d;
        int illum;

        MATERIAL( ) 
        {
            shader = "";
            bump = "";

            Ns = 0.0;
            Ka_x = 0.0;
            Ka_y = 0.0;
            Ka_z = 0.0;
            Kd_x = 0.0;
            Kd_y = 0.0;
            Kd_z = 0.0;
            Ks_x = 0.0;
            Ks_y = 0.0;
            Ks_z = 0.0;
            Ni = 0.0;
            d = 0.0;
            illum = 0.0;
        };
    };

    /*
    Strut that represents a model element
    */
    struct BLOCK
    {
        /**********************
        VBO Handles
        **********************/
        /**************************
        Handle to VBO
        **************************/
        GLuint vertex_array_handle;
        /**************************
        Handle to Vertex Data
        **************************/
        GLuint vertex_buffer_handle;
        /**************************
        Handle to Normal Data
        **************************/
        GLuint normal_buffer_handle;
        /*****************************
        Handle to Vertex Tangent Data
        *****************************/
        GLuint tangent_buffer_handle;
        /****************************
        Handle to Texture Coord Data
        ****************************/
        GLuint tex_cord_buffer_handle;
        /*******************************
        Handle to Triangle Index Values
        *******************************/
        GLuint index_buffer_handle;
        /***********************************
        Vector of Attribute Buffer Handles
        ***********************************/
        vector<GLuint> attribute_buffer_handles;

        /*****************************
        Vector Containing Vertex Data
        *****************************/
        vector<float> verts;
        /*****************************
        Vector Containing Normal Data
        *****************************/
        vector<float> norms;
        /*************************************
        Vector Containing Vertex Tangent Data
        *************************************/
        vector<float> tangents;
        /************************************
        Vector Containing Texture Coord Data
        ************************************/
        vector<float> vts;
        /*********************************
        Vector Containing Vectors of
        Attribute Data
        *********************************/
        vector< vector<float> > vas;
        /*******************************
        Vector of Triangle Index Values
        *******************************/
        vector<GLuint> indexs;
        
        /***********************
        Block Material Name
        ***********************/
        string mat;

        BLOCK( )
        {
            vertex_array_handle = 0;
            vertex_buffer_handle = 0;
            normal_buffer_handle = 0;
            tangent_buffer_handle = 0;
            tex_cord_buffer_handle = 0;
            index_buffer_handle = 0;
        }
    };

    //public members and methods
    public:
        /*Default Constructor*/
        gl_Model( );

        /*Default Destructor*/
        ~gl_Model();

        /*Checks that a model has been loaded correctly*/
        bool is_Valid( );

        /*Function that loads a model (obj) file
          and creates the appropriate materials*/
        bool Load_OBJ( GLchar* model_file );

        /*Function to load the respective shaders and
          textures and call the drawelements method
          Mode: 0 = default, 1 = Fixed Functionality Only*/
        bool Draw( int mode = 0 );

        /* Function to save the model in krm format a binary
        format that includes the vertex, normal, and index
        values along with supplemental information for blocks
        and materials and shader names. */
        bool Save( char* save_path, float scale_f = 0 );

        /**************************************************
		Upload a Uniform value into the specified shader
        0 - 2D sampler, 1 - 1f, 2 - 2f, 3 - 3f, 4 - 4f,
        5 - 1i, 6 - 2i, 7 - 3i, 8 - 4i
		*************************************************/
        bool Load_Uniform( GLuint u_type, string shader_name, string uniform_name,
            float v_1 = 0.0f, float v_2 = 0.0f, float v_3 = 0.0f, float v_4 = 0.0f );

    //private members and methods
    private:

        /*Load Material File*/
        bool Load_Material( const char* material_file );

        bool loaded;
        vector< BLOCK > blocks;
        map< string, MATERIAL > mats;
        map< string, gl_Shader > shaders;
        map< string, TEXTURE_2D_RGB > textures;
        map< string, TEXTURE_CUBE> cubetextures;
        map< string, TEXTURE_2D_RGB > bumps;
};


/***************************
Simple Structs for Points
***************************/
struct Point3D
{
    Point3D( float v1_v, float v2_v, float v3_v )
    {
        x = v1_v;
        y = v2_v;
        z = v3_v;
    }

    Point3D( )
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    Point3D operator-( const Point3D& p2 ) const
    {
        return Point3D( x - p2.x, y - p2.y, z  - p2.z );
    }

    Point3D operator*( const double num ) const
    {
        return Point3D( x * num, y * num, z * num );
    }

    Point3D Normalize( )
    {
        double r = sqrt( x*x + y*y + z*z );
        return Point3D( x/r, y/r, z/r );
    }

    float x;
    float y;
    float z;
};

double Dot( Point3D p1, Point3D p2 );
Point3D Cross( Point3D p1, Point3D p2 );

struct Point2D
{
    Point2D( float v1_v, float v2_v )
    {
        x = v1_v;
        y = v2_v;
    }

    Point2D( )
    {
        x = 0.0f;
        y = 0.0f;
    }

    float x;
    float y;
};

#endif //include lock

