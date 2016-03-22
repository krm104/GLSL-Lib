/**************************************************************
Name: Kenny Moser	krm104@gmail.com
Course: ECE8990
File:   glModel.cpp

This source file contains the definitions of the gl_Model
class. As well as helper/friend functions.

Use all you like, but if you break it, don't reference me >.>

**************************************************************/


/************************
Required Includes
************************/
#include "glModel.h"

/***********************
Additional Includes
***********************/


/********************************************
Point3D/Point2D Suppoer Function Definitions
********************************************/
/**************************
Dot Product of 2 Vectors
**************************/
double Dot( Point3D p1, Point3D p2 )
{
    return p1.x*p2.x + p1.y*p2.y + p1.z*p2.z;
}

/**************************
Cross Product of 2 Vectors
**************************/
Point3D Cross( Point3D p1, Point3D p2 )
{
    return Point3D();
}

/**********************************
gl_Camera Function Definitions
**********************************/
/**********************************
Default Constructor
**********************************/
gl_Model::gl_Model( )
{
    loaded = false;
}

        
/**********************************
Default Destructor
**********************************/
gl_Model::~gl_Model()
{
    loaded = false;

    for ( unsigned int i = 0; i < blocks.size( ); i++ )
    {
        glBindVertexArray( 0 );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    
        glDeleteVertexArrays( 1, &(blocks[i].vertex_array_handle) );
        glDeleteBuffers( 1, &(blocks[i].vertex_buffer_handle) );
        glDeleteBuffers( 1, &(blocks[i].index_buffer_handle) );
        glDeleteBuffers( 1, &(blocks[i].normal_buffer_handle) );
        glDeleteBuffers( 1, &(blocks[i].tangent_buffer_handle) );
        glDeleteBuffers( 1, &(blocks[i].tex_cord_buffer_handle) );

        glDeleteBuffers( blocks[i].attribute_buffer_handles.size( ), &(blocks[i].attribute_buffer_handles[0]) );
    }
}


/************************************
Checks that a model has been loaded
correctly without error
************************************/
bool gl_Model::is_Valid( )
{
    return loaded;
}


/***************************************
Function that loads a model (obj) file
and creates the appropriate materials
***************************************/
bool gl_Model::Load_OBJ( GLchar* model_file )
{
    if ( model_file  && !loaded )
    {
        unsigned int file_name_length = strlen( model_file );
        if ( strlen( model_file ) > 4 )
        {
            if ( !strcmp( model_file + (file_name_length - 4), ".obj" ) )
            {
                ifstream model(model_file);

                if ( model.good( ) )
                {
                    float  min_x = 0, max_x = 0, min_y = 0, max_y = 0,
                        min_z = 0, max_z = 0;
                    bool min_max_flag = false;

                    vector<float> vertices;
                    vector<float> normals;
                    vector<float> textures;
                    vector<float> vattribs;

                    while ( !model.eof( ) )
                    {
                        string token;
                        model >> token;
                        if ( token == "#" )
                        {
                            model.ignore( LONG_MAX, '\n' );
                        }
                        else if ( token == "mtllib" )
                        {
                            string material_file;
                            model >> material_file;

                            if ( !Load_Material( material_file.c_str( ) ) )
                            {
                                loaded = false;
                                break;
                            }
                        }
                        else if ( token == "usemtl" )
                        {
                            string material;
                            model >> material;
                            blocks.push_back( BLOCK( ) );
                            blocks.back( ).mat = material;
                        }
                        else if ( token == "o" )
                        {
                            model.ignore( LONG_MAX, '\n' );
                        }
                        else if ( token == "s" )
                        {
                            model.ignore( LONG_MAX, '\n' );
                        }
                        else if ( token == "g" )
                        {
                            model.ignore( LONG_MAX, '\n' );
                        }
                        else if ( token == "a" )
                        {
                            //blocks.back( ).attribute_buffer_handles.push_back(mats[blocks.back().mat].attrib_handle);
                        }
                        else if ( token == "va" )
                        {
                            float num;
                            model >> num;
                            vattribs.push_back( num );
                            model >> num;
                            vattribs.push_back( num );
                            model >> num;
                            vattribs.push_back( num );
                        }
                        else if ( token == "v" )
                        {
                            float num;
                            model >> num;
                            vertices.push_back( num );
                            model >> num;
                            vertices.push_back( num );
                            model >> num;
                            vertices.push_back( num );
                        }
                        else if ( token == "vn" )
                        {
                            float num;
                            model >> num;
                            normals.push_back( num );
                            model >> num;
                            normals.push_back( num );
                            model >> num;
                            normals.push_back( num );
                        }
                        else if ( token == "vt" )
                        {
                            float num;
                            model >> num;
                            textures.push_back( num );
                            model >> num;
                            textures.push_back( num );
                        }
                        else if ( token == "f" )
                        {
                            if ( !blocks.size( ) )
                            {
                                blocks.push_back( BLOCK() );
                            }

                            int v_index, vt_index, vn_index;
                            char slash;

                            if ( normals.size( ) && textures.size( ) )
                            {
                                //first vertex//
                                model >> v_index;
                                model >> slash;
                                model >> vt_index;
                                model >> slash;
                                model >> vn_index;

                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 ] );
                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 + 1] );
                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 + 2] );

                                if ( !min_max_flag )
                                {
                                    min_max_flag = true;

                                    min_x = vertices[ ( v_index - 1 )*3 ];
                                    min_y = vertices[ ( v_index - 1 )*3 + 1 ];
                                    min_z = vertices[ ( v_index - 1 )*3 + 2 ];

                                    max_x = min_x;
                                    max_y = min_y;
                                    max_z = max_z;
                                }

                                min_x = vertices[ ( v_index - 1 )*3 ] < min_x ? vertices[ ( v_index - 1 )*3 ] : min_x;
                                min_y = vertices[ ( v_index - 1 )*3 + 1 ] < min_y ? vertices[ ( v_index - 1 )*3 + 1 ] : min_y;
                                min_z = vertices[ ( v_index - 1 )*3 + 2 ] < min_z ? vertices[ ( v_index - 1 )*3 + 2 ] : min_z;
                                
                                max_x = vertices[ ( v_index - 1 )*3 ] > max_x ? vertices[ ( v_index - 1 )*3 ] : max_x;
                                max_y = vertices[ ( v_index - 1 )*3 + 1 ] > max_y ? vertices[ ( v_index - 1 )*3 + 1 ] : max_y;
                                max_z = vertices[ ( v_index - 1 )*3 + 2 ] > max_z ? vertices[ ( v_index - 1 )*3 + 2 ] : max_z;

                                blocks.back().norms.push_back( normals[ ( vn_index - 1 )*3 ] );
                                blocks.back().norms.push_back( normals[ ( vn_index - 1 )*3 + 1] ); 
                                blocks.back().norms.push_back( normals[ ( vn_index - 1 )*3 + 2] );

                                blocks.back().vts.push_back( textures[ ( vt_index - 1 )*2 ] );
                                blocks.back().vts.push_back( textures[ ( vt_index - 1 )*2 + 1 ] );

                                if ( vattribs.size( ) )
                                {
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 ] );
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 + 1] );
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 + 2] );
                                }

                                blocks.back().indexs.push_back( blocks.back().verts.size( )/3 - 1 );

                                //second vertex//
                                model >> v_index;
                                model >> slash;
                                model >> vt_index;
                                model >> slash;
                                model >> vn_index;

                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 ] );
                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 + 1] );
                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 + 2] );

                                min_x = vertices[ ( v_index - 1 )*3 ] < min_x ? vertices[ ( v_index - 1 )*3 ] : min_x;
                                min_y = vertices[ ( v_index - 1 )*3 + 1 ] < min_y ? vertices[ ( v_index - 1 )*3 + 1 ] : min_y;
                                min_z = vertices[ ( v_index - 1 )*3 + 2 ] < min_z ? vertices[ ( v_index - 1 )*3 + 2 ] : min_z;
                                
                                max_x = vertices[ ( v_index - 1 )*3 ] > max_x ? vertices[ ( v_index - 1 )*3 ] : max_x;
                                max_y = vertices[ ( v_index - 1 )*3 + 1 ] > max_y ? vertices[ ( v_index - 1 )*3 + 1 ] : max_y;
                                max_z = vertices[ ( v_index - 1 )*3 + 2 ] > max_z ? vertices[ ( v_index - 1 )*3 + 2 ] : max_z;

                                blocks.back().norms.push_back( normals[ ( vn_index - 1 )*3 ] );
                                blocks.back().norms.push_back( normals[ ( vn_index - 1 )*3 + 1] ); 
                                blocks.back().norms.push_back( normals[ ( vn_index - 1 )*3 + 2] );

                                blocks.back().vts.push_back( textures[ ( vt_index - 1 )*2 ] );
                                blocks.back().vts.push_back( textures[ ( vt_index - 1 )*2 + 1 ] );

                                if ( vattribs.size( ) )
                                {
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 ] );
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 + 1] );
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 + 2] );
                                }

                                blocks.back().indexs.push_back( blocks.back().verts.size( )/3 - 1 );

                                //third vertex//
                                model >> v_index;
                                model >> slash;
                                model >> vt_index;
                                model >> slash;
                                model >> vn_index;

                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 ] );
                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 + 1] );
                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 + 2] );

                                min_x = vertices[ ( v_index - 1 )*3 ] < min_x ? vertices[ ( v_index - 1 )*3 ] : min_x;
                                min_y = vertices[ ( v_index - 1 )*3 + 1 ] < min_y ? vertices[ ( v_index - 1 )*3 + 1 ] : min_y;
                                min_z = vertices[ ( v_index - 1 )*3 + 2 ] < min_z ? vertices[ ( v_index - 1 )*3 + 2 ] : min_z;
                                
                                max_x = vertices[ ( v_index - 1 )*3 ] > max_x ? vertices[ ( v_index - 1 )*3 ] : max_x;
                                max_y = vertices[ ( v_index - 1 )*3 + 1 ] > max_y ? vertices[ ( v_index - 1 )*3 + 1 ] : max_y;
                                max_z = vertices[ ( v_index - 1 )*3 + 2 ] > max_z ? vertices[ ( v_index - 1 )*3 + 2 ] : max_z;

                                blocks.back().norms.push_back( normals[ ( vn_index - 1 )*3 ] );
                                blocks.back().norms.push_back( normals[ ( vn_index - 1 )*3 + 1] ); 
                                blocks.back().norms.push_back( normals[ ( vn_index - 1 )*3 + 2] );

                                blocks.back().vts.push_back( textures[ ( vt_index - 1 )*2 ] );
                                blocks.back().vts.push_back( textures[ ( vt_index - 1 )*2 + 1 ] );

                                if ( vattribs.size( ) )
                                {
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 ] );
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 + 1] );
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 + 2] );
                                }

                                blocks.back().indexs.push_back( blocks.back().verts.size( )/3 - 1 );
                            }
                            else if ( normals.size( ) )
                            {
                                //first vertex//
                                model >> v_index;
                                model >> slash;
                                model >> slash;
                                model >> vn_index;

                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 ] );
                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 + 1] );
                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 + 2] );

                                if ( !min_max_flag )
                                {
                                    min_max_flag = true;

                                    min_x = vertices[ ( v_index - 1 )*3 ];
                                    min_y = vertices[ ( v_index - 1 )*3 + 1 ];
                                    min_z = vertices[ ( v_index - 1 )*3 + 2 ];

                                    max_x = min_x;
                                    max_y = min_y;
                                    max_z = max_z;
                                }

                                min_x = vertices[ ( v_index - 1 )*3 ] < min_x ? vertices[ ( v_index - 1 )*3 ] : min_x;
                                min_y = vertices[ ( v_index - 1 )*3 + 1 ] < min_y ? vertices[ ( v_index - 1 )*3 + 1 ] : min_y;
                                min_z = vertices[ ( v_index - 1 )*3 + 2 ] < min_z ? vertices[ ( v_index - 1 )*3 + 2 ] : min_z;
                                
                                max_x = vertices[ ( v_index - 1 )*3 ] > max_x ? vertices[ ( v_index - 1 )*3 ] : max_x;
                                max_y = vertices[ ( v_index - 1 )*3 + 1 ] > max_y ? vertices[ ( v_index - 1 )*3 + 1 ] : max_y;
                                max_z = vertices[ ( v_index - 1 )*3 + 2 ] > max_z ? vertices[ ( v_index - 1 )*3 + 2 ] : max_z;

                                blocks.back().norms.push_back( normals[ ( vn_index - 1 )*3 ] );
                                blocks.back().norms.push_back( normals[ ( vn_index - 1 )*3 + 1] ); 
                                blocks.back().norms.push_back( normals[ ( vn_index - 1 )*3 + 2] );

                                if ( vattribs.size( ) )
                                {
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 ] );
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 + 1] );
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 + 2] );
                                }

                                blocks.back().indexs.push_back( blocks.back().verts.size( )/3 - 1 );

                                //second vertex//
                                model >> v_index;
                                model >> slash;
                                model >> slash;
                                model >> vn_index;

                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 ] );
                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 + 1] );
                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 + 2] );

                                min_x = vertices[ ( v_index - 1 )*3 ] < min_x ? vertices[ ( v_index - 1 )*3 ] : min_x;
                                min_y = vertices[ ( v_index - 1 )*3 + 1 ] < min_y ? vertices[ ( v_index - 1 )*3 + 1 ] : min_y;
                                min_z = vertices[ ( v_index - 1 )*3 + 2 ] < min_z ? vertices[ ( v_index - 1 )*3 + 2 ] : min_z;
                                
                                max_x = vertices[ ( v_index - 1 )*3 ] > max_x ? vertices[ ( v_index - 1 )*3 ] : max_x;
                                max_y = vertices[ ( v_index - 1 )*3 + 1 ] > max_y ? vertices[ ( v_index - 1 )*3 + 1 ] : max_y;
                                max_z = vertices[ ( v_index - 1 )*3 + 2 ] > max_z ? vertices[ ( v_index - 1 )*3 + 2 ] : max_z;

                                blocks.back().norms.push_back( normals[ ( vn_index - 1 )*3 ] );
                                blocks.back().norms.push_back( normals[ ( vn_index - 1 )*3 + 1] ); 
                                blocks.back().norms.push_back( normals[ ( vn_index - 1 )*3 + 2] );

                                if ( vattribs.size( ) )
                                {
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 ] );
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 + 1] );
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 + 2] );
                                }

                                blocks.back().indexs.push_back( blocks.back().verts.size( )/3 - 1 );

                                //third vertex//
                                model >> v_index;
                                model >> slash;
                                model >> slash;
                                model >> vn_index;

                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 ] );
                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 + 1] );
                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 + 2] );

                                min_x = vertices[ ( v_index - 1 )*3 ] < min_x ? vertices[ ( v_index - 1 )*3 ] : min_x;
                                min_y = vertices[ ( v_index - 1 )*3 + 1 ] < min_y ? vertices[ ( v_index - 1 )*3 + 1 ] : min_y;
                                min_z = vertices[ ( v_index - 1 )*3 + 2 ] < min_z ? vertices[ ( v_index - 1 )*3 + 2 ] : min_z;
                                
                                max_x = vertices[ ( v_index - 1 )*3 ] > max_x ? vertices[ ( v_index - 1 )*3 ] : max_x;
                                max_y = vertices[ ( v_index - 1 )*3 + 1 ] > max_y ? vertices[ ( v_index - 1 )*3 + 1 ] : max_y;
                                max_z = vertices[ ( v_index - 1 )*3 + 2 ] > max_z ? vertices[ ( v_index - 1 )*3 + 2 ] : max_z;

                                blocks.back().norms.push_back( normals[ ( vn_index - 1 )*3 ] );
                                blocks.back().norms.push_back( normals[ ( vn_index - 1 )*3 + 1] ); 
                                blocks.back().norms.push_back( normals[ ( vn_index - 1 )*3 + 2] );

                                if ( vattribs.size( ) )
                                {
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 ] );
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 + 1] );
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 + 2] );
                                }

                                blocks.back().indexs.push_back( blocks.back().verts.size( )/3 - 1 );
                            }
                            else if ( textures.size( ) )
                            {
                                //first vertex//
                                model >> v_index;
                                model >> slash;
                                model >> vt_index;
                                model >> slash;

                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 ] );
                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 + 1] );
                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 + 2] );

                                if ( !min_max_flag )
                                {
                                    min_max_flag = true;

                                    min_x = vertices[ ( v_index - 1 )*3 ];
                                    min_y = vertices[ ( v_index - 1 )*3 + 1 ];
                                    min_z = vertices[ ( v_index - 1 )*3 + 2 ];

                                    max_x = min_x;
                                    max_y = min_y;
                                    max_z = max_z;
                                }

                                min_x = vertices[ ( v_index - 1 )*3 ] < min_x ? vertices[ ( v_index - 1 )*3 ] : min_x;
                                min_y = vertices[ ( v_index - 1 )*3 + 1 ] < min_y ? vertices[ ( v_index - 1 )*3 + 1 ] : min_y;
                                min_z = vertices[ ( v_index - 1 )*3 + 2 ] < min_z ? vertices[ ( v_index - 1 )*3 + 2 ] : min_z;
                                
                                max_x = vertices[ ( v_index - 1 )*3 ] > max_x ? vertices[ ( v_index - 1 )*3 ] : max_x;
                                max_y = vertices[ ( v_index - 1 )*3 + 1 ] > max_y ? vertices[ ( v_index - 1 )*3 + 1 ] : max_y;
                                max_z = vertices[ ( v_index - 1 )*3 + 2 ] > max_z ? vertices[ ( v_index - 1 )*3 + 2 ] : max_z;

                                blocks.back().vts.push_back( textures[ ( vt_index - 1 )*2 ] );
                                blocks.back().vts.push_back( textures[ ( vt_index - 1 )*2 + 1 ] );

                                if ( vattribs.size( ) )
                                {
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 ] );
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 + 1] );
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 + 2] );
                                }

                                blocks.back().indexs.push_back( blocks.back().verts.size( )/3 - 1 );

                                //second vertex//
                                model >> v_index;
                                model >> slash;
                                model >> vt_index;
                                model >> slash;

                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 ] );
                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 + 1] );
                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 + 2] );

                                min_x = vertices[ ( v_index - 1 )*3 ] < min_x ? vertices[ ( v_index - 1 )*3 ] : min_x;
                                min_y = vertices[ ( v_index - 1 )*3 + 1 ] < min_y ? vertices[ ( v_index - 1 )*3 + 1 ] : min_y;
                                min_z = vertices[ ( v_index - 1 )*3 + 2 ] < min_z ? vertices[ ( v_index - 1 )*3 + 2 ] : min_z;
                                
                                max_x = vertices[ ( v_index - 1 )*3 ] > max_x ? vertices[ ( v_index - 1 )*3 ] : max_x;
                                max_y = vertices[ ( v_index - 1 )*3 + 1 ] > max_y ? vertices[ ( v_index - 1 )*3 + 1 ] : max_y;
                                max_z = vertices[ ( v_index - 1 )*3 + 2 ] > max_z ? vertices[ ( v_index - 1 )*3 + 2 ] : max_z;

                                blocks.back().vts.push_back( textures[ ( vt_index - 1 )*2 ] );
                                blocks.back().vts.push_back( textures[ ( vt_index - 1 )*2 + 1 ] );

                                if ( vattribs.size( ) )
                                {
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 ] );
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 + 1] );
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 + 2] );
                                }

                                blocks.back().indexs.push_back( blocks.back().verts.size( )/3 - 1 );

                                //third vertex//
                                model >> v_index;
                                model >> slash;
                                model >> vt_index;
                                model >> slash;

                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 ] );
                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 + 1] );
                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 + 2] );

                                min_x = vertices[ ( v_index - 1 )*3 ] < min_x ? vertices[ ( v_index - 1 )*3 ] : min_x;
                                min_y = vertices[ ( v_index - 1 )*3 + 1 ] < min_y ? vertices[ ( v_index - 1 )*3 + 1 ] : min_y;
                                min_z = vertices[ ( v_index - 1 )*3 + 2 ] < min_z ? vertices[ ( v_index - 1 )*3 + 2 ] : min_z;
                                
                                max_x = vertices[ ( v_index - 1 )*3 ] > max_x ? vertices[ ( v_index - 1 )*3 ] : max_x;
                                max_y = vertices[ ( v_index - 1 )*3 + 1 ] > max_y ? vertices[ ( v_index - 1 )*3 + 1 ] : max_y;
                                max_z = vertices[ ( v_index - 1 )*3 + 2 ] > max_z ? vertices[ ( v_index - 1 )*3 + 2 ] : max_z;

                                blocks.back().vts.push_back( textures[ ( vt_index - 1 )*2 ] );
                                blocks.back().vts.push_back( textures[ ( vt_index - 1 )*2 + 1 ] );

                                if ( vattribs.size( ) )
                                {
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 ] );
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 + 1] );
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 + 2] );
                                }

                                blocks.back().indexs.push_back( blocks.back().verts.size( )/3 - 1 );
                            }
                            else
                            {
                                //first vertex//
                                model >> v_index;
                                model >> slash;
                                model >> slash;

                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 ] );
                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 + 1] );
                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 + 2] );

                                if ( !min_max_flag )
                                {
                                    min_max_flag = true;

                                    min_x = vertices[ ( v_index - 1 )*3 ];
                                    min_y = vertices[ ( v_index - 1 )*3 + 1 ];
                                    min_z = vertices[ ( v_index - 1 )*3 + 2 ];

                                    max_x = min_x;
                                    max_y = min_y;
                                    max_z = max_z;
                                }

                                min_x = vertices[ ( v_index - 1 )*3 ] < min_x ? vertices[ ( v_index - 1 )*3 ] : min_x;
                                min_y = vertices[ ( v_index - 1 )*3 + 1 ] < min_y ? vertices[ ( v_index - 1 )*3 + 1 ] : min_y;
                                min_z = vertices[ ( v_index - 1 )*3 + 2 ] < min_z ? vertices[ ( v_index - 1 )*3 + 2 ] : min_z;
                                
                                max_x = vertices[ ( v_index - 1 )*3 ] > max_x ? vertices[ ( v_index - 1 )*3 ] : max_x;
                                max_y = vertices[ ( v_index - 1 )*3 + 1 ] > max_y ? vertices[ ( v_index - 1 )*3 + 1 ] : max_y;
                                max_z = vertices[ ( v_index - 1 )*3 + 2 ] > max_z ? vertices[ ( v_index - 1 )*3 + 2 ] : max_z;

                                if ( vattribs.size( ) )
                                {
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 ] );
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 + 1] );
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 + 2] );
                                }

                                blocks.back().indexs.push_back( blocks.back().verts.size( )/3 - 1 );

                                //second vertex//
                                model >> v_index;
                                model >> slash;
                                model >> slash;

                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 ] );
                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 + 1] );
                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 + 2] );

                                min_x = vertices[ ( v_index - 1 )*3 ] < min_x ? vertices[ ( v_index - 1 )*3 ] : min_x;
                                min_y = vertices[ ( v_index - 1 )*3 + 1 ] < min_y ? vertices[ ( v_index - 1 )*3 + 1 ] : min_y;
                                min_z = vertices[ ( v_index - 1 )*3 + 2 ] < min_z ? vertices[ ( v_index - 1 )*3 + 2 ] : min_z;
                                
                                max_x = vertices[ ( v_index - 1 )*3 ] > max_x ? vertices[ ( v_index - 1 )*3 ] : max_x;
                                max_y = vertices[ ( v_index - 1 )*3 + 1 ] > max_y ? vertices[ ( v_index - 1 )*3 + 1 ] : max_y;
                                max_z = vertices[ ( v_index - 1 )*3 + 2 ] > max_z ? vertices[ ( v_index - 1 )*3 + 2 ] : max_z;

                                if ( vattribs.size( ) )
                                {
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 ] );
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 + 1] );
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 + 2] );
                                }

                                blocks.back().indexs.push_back( blocks.back().verts.size( )/3 - 1 );

                                //third vertex//
                                model >> v_index;
                                model >> slash;
                                model >> slash;

                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 ] );
                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 + 1] );
                                blocks.back().verts.push_back( vertices[ ( v_index - 1 )*3 + 2] );

                                min_x = vertices[ ( v_index - 1 )*3 ] < min_x ? vertices[ ( v_index - 1 )*3 ] : min_x;
                                min_y = vertices[ ( v_index - 1 )*3 + 1 ] < min_y ? vertices[ ( v_index - 1 )*3 + 1 ] : min_y;
                                min_z = vertices[ ( v_index - 1 )*3 + 2 ] < min_z ? vertices[ ( v_index - 1 )*3 + 2 ] : min_z;
                                
                                max_x = vertices[ ( v_index - 1 )*3 ] > max_x ? vertices[ ( v_index - 1 )*3 ] : max_x;
                                max_y = vertices[ ( v_index - 1 )*3 + 1 ] > max_y ? vertices[ ( v_index - 1 )*3 + 1 ] : max_y;
                                max_z = vertices[ ( v_index - 1 )*3 + 2 ] > max_z ? vertices[ ( v_index - 1 )*3 + 2 ] : max_z;

                                if ( vattribs.size( ) )
                                {
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 ] );
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 + 1] );
                                    //blocks.back().vas.push_back( vattribs[ ( v_index - 1 )*3 + 2] );
                                }

                                blocks.back().indexs.push_back( blocks.back().verts.size( )/3 - 1 );
                            }
                        }
                    }

                    if ( blocks.size( ) )
                    {
                        /***************************
                        Resize and Position Model
                        ***************************/
                        double model_width = (max_x - min_x)/2.0;
                        double model_height = (max_y - min_y)/2.0;
                        double model_breadth = (max_z - min_z)/2.0;

                        float scale_factor = 5.0 / fmaxf( model_width, fmaxf( model_height, model_breadth ) );

                        double adjustment_x = max_x - model_width;
                        double adjustment_y = max_y - model_height;
                        double adjustment_z = max_z - model_breadth;

                        loaded = true;
                        for ( unsigned int i = 0; i < blocks.size( ); i++ )
                        {
                            for ( unsigned int j = 0; j < blocks[i].verts.size( ); j += 3)
                            {
                                blocks[i].verts[j] = (blocks[i].verts[j] - adjustment_x)*scale_factor;
                                blocks[i].verts[j + 1] = (blocks[i].verts[j + 1] - adjustment_y)*scale_factor;
                                blocks[i].verts[j + 2] = (blocks[i].verts[j + 2] - adjustment_z)*scale_factor;
                            }

                            //create vertex array
                            glGenVertexArrays( 1, &(blocks[i].vertex_array_handle) );
                            glBindVertexArray( blocks[i].vertex_array_handle );

                            //create vertex buffer object
                            glGenBuffers( 1, &(blocks[i].vertex_buffer_handle) );
                            glBindBuffer( GL_ARRAY_BUFFER, blocks[i].vertex_buffer_handle );
                            glBufferData( GL_ARRAY_BUFFER, sizeof(GLfloat)*blocks[i].verts.size( ), &(blocks[i].verts[0]), GL_STATIC_DRAW );
                            glVertexPointer( 3, GL_FLOAT, 0, NULL );
                            glEnableClientState( GL_VERTEX_ARRAY );

                            //Create index buffer
                            glGenBuffers( 1, &(blocks[i].index_buffer_handle) );
                            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, blocks[i].index_buffer_handle );
                            glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*blocks[i].indexs.size( ), &(blocks[i].indexs[0]), GL_STATIC_DRAW );

                            if ( blocks[i].norms.size( ) )
                            {
                                //Create and enable normal Buffer
                                glGenBuffers( 1, &(blocks[i].normal_buffer_handle) );
                                glBindBuffer( GL_ARRAY_BUFFER, blocks[i].normal_buffer_handle );
                                glBufferData( GL_ARRAY_BUFFER, sizeof(GLfloat)*blocks[i].norms.size( ), &(blocks[i].norms[0]), GL_STATIC_DRAW );
                                glNormalPointer(GL_FLOAT, 0, NULL);
                                glEnableClientState( GL_NORMAL_ARRAY );

                                if ( blocks[i].vts.size( ) )
                                {
                                    /*******************************************************
                                    Create Tangent Vectors for each vertex
                                    *******************************************************/
                                    vector<float> tan1;
                                    vector<float> tan2;
                                    tan1.resize(blocks[i].verts.size());
                                    tan2.resize(tan1.size());
                                    for ( unsigned int a = 0; a < tan1.size(); a++)
                                    {
                                        tan1[a] = 0.0f;
                                        tan2[a] = 0.0f;
                                    }

                                    for ( unsigned int a = 0; a < blocks[i].indexs.size()/3; a++ )
                                    {
                                        long i1 = blocks[i].indexs[a*3];
                                        long i2 = blocks[i].indexs[a*3 + 1];
                                        long i3 = blocks[i].indexs[a*3 + 2];

                                        const Point3D v1( blocks[i].verts[i1], blocks[i].verts[i1 + 1], blocks[i].verts[i1 +2] );
                                        const Point3D v2( blocks[i].verts[i2], blocks[i].verts[i2 + 1], blocks[i].verts[i2 +2] );
                                        const Point3D v3( blocks[i].verts[i3], blocks[i].verts[i3 + 1], blocks[i].verts[i3 +2] );

                                        const Point2D w1( blocks[i].vts[i1], blocks[i].vts[i1 + 1] );
                                        const Point2D w2( blocks[i].vts[i2], blocks[i].vts[i2 + 1] );
                                        const Point2D w3( blocks[i].vts[i3], blocks[i].vts[i3 + 1] );

                                        float x1 = v2.x - v1.x;
                                        float x2 = v3.x - v1.x;
                                        float y1 = v2.y - v1.y;
                                        float y2 = v3.y - v1.y;
                                        float z1 = v2.z - v1.z;
                                        float z2 = v3.z - v1.z;

                                        float s1 = w2.x - w1.x;
                                        float s2 = w3.x - w1.x;
                                        float t1 = w2.y - w1.y;
                                        float t2 = w3.y - w1.y;

                                        float r = 1.0f / (s1 * t2 - s2 * t1 );

                                        Point3D sdir( (t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
                                                        (t2 * z1 - t1 * z2) * r );
                                        Point3D tdir( (s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r );

                                        tan1[i1] += sdir.x;
                                        tan1[i1 + 1] += sdir.y;
                                        tan1[i1 + 2] += sdir.z;

                                        tan1[i2] += sdir.x;
                                        tan1[i2 + 1] += sdir.y;
                                        tan1[i2 + 2] += sdir.z;

                                        tan1[i3] += sdir.x;
                                        tan1[i3 + 1] += sdir.y;
                                        tan1[i3 + 2] += sdir.z;

                                        tan2[i1] += tdir.x;
                                        tan2[i1 + 1] += tdir.y;
                                        tan2[i1 + 2] += tdir.z;

                                        tan2[i2] += tdir.x;
                                        tan2[i2 + 1] += tdir.y;
                                        tan2[i2 + 2] += tdir.z;

                                        tan2[i3] += tdir.x;
                                        tan2[i3 + 1] += tdir.y;
                                        tan2[i3 + 2] += tdir.z;
                                    }

                                    for ( unsigned int a = 0; a < blocks[i].verts.size()/3; a++ )
                                    {
                                        const Point3D n( blocks[i].norms[a*3], blocks[i].norms[a*3 + 1],
                                                            blocks[i].norms[a*3 + 2] );
                                        const Point3D t( tan1[a*3], tan1[a*3 + 1], tan1[a*3 + 2] );

                                        Point3D tangent = (t - n * Dot( n, t ) ).Normalize();


                                        blocks[i].tangents.push_back( tangent.x );
                                        blocks[i].tangents.push_back( tangent.y );
                                        blocks[i].tangents.push_back( tangent.z );

                                        //Point3D temp_tan2 = Point3D(tan2[a*3], tan2[a*3+1], tan2[a*3+2]);
                                        //blocks[i].tangents.push_back( Dot(Cross(n, t), temp_tan2) < 0.0f ? -1.0f : 1.0f);

                                        //std::cout << tangent.x << std::endl;
                                        //std::cout << tangent.y << std::endl;
                                        //std::cout << tangent.z << std::endl;
                                    }

                                    /***************************************************
                                    Generate Tangent Attribute Buffer Objects
                                    ***************************************************/
                                    glGenBuffers( 1, &blocks[i].tangent_buffer_handle );
                                    glBindBuffer( GL_ARRAY_BUFFER, blocks[i].tangent_buffer_handle );
                                    glBufferData( GL_ARRAY_BUFFER, sizeof(GLfloat)*blocks[i].tangents.size( ), &(blocks[i].tangents[0]), GL_STATIC_DRAW );
                                    glColorPointer( 3, GL_FLOAT, 0, NULL );
                                    glEnableClientState( GL_COLOR_ARRAY );

                                    //Create and Enable Tangent Buffer  
                                    for ( map< string, MATERIAL>::iterator j = mats.begin(); j != mats.end(); j++ )
                                    {
                                        if ( j->second.bump != "" && j->second.shader != "" )
                                        {
                                            shaders[j->second.shader].bind_attribute( "tangents", blocks[i].tangent_buffer_handle );
                                            std::cout << "linked Attribue" << std::endl;
                                        }
                                    }
                                }
                            }

                            if ( blocks[i].vas.size( ) )
                            {
                                //glGenBuffers( 1, &(blocks[i].attribute_buffer_handle) );
                                //glBindBuffer( GL_ARRAY_BUFFER, blocks[i].attribute_buffer_handle );
                                //glBufferData( GL_ARRAY_BUFFER, sizeof(GLfloat)*blocks[i].vas.size( ), &(blocks[i].vas[0]), GL_STATIC_DRAW );
                                //glColorPointer( 3, GL_FLOAT, 0, NULL );
                                //glEnableClientState( GL_COLOR_ARRAY );
                            }

                            if ( blocks[i].vts.size( ) )
                            {
                                //Create and enable tex cord buffer
                                glGenBuffers( 1, &(blocks[i].tex_cord_buffer_handle) );
                                glBindBuffer( GL_ARRAY_BUFFER, blocks[i].tex_cord_buffer_handle );
                                glBufferData( GL_ARRAY_BUFFER, sizeof(GLfloat)*blocks[i].vts.size( ), &(blocks[i].vts[0]), GL_STATIC_DRAW );
                                glTexCoordPointer( 2, GL_FLOAT, 0, NULL );
                                glEnableClientState( GL_TEXTURE_COORD_ARRAY );
                            }

                            glBindVertexArray(0);
                            glBindBuffer( GL_ARRAY_BUFFER, 0);
                            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

                            glDisableClientState( GL_VERTEX_ARRAY );
                            glDisableClientState( GL_NORMAL_ARRAY );
                            glDisableClientState( GL_TEXTURE_COORD_ARRAY );
                            glDisableClientState( GL_COLOR_ARRAY );
                        }
                    }
                }
                else 
                {
                    loaded = false;
                }

                model.close( );
                model.clear( );
            }
            else
            {
                loaded = false;
            }
        }
        else
        {
            loaded = false;
        }
    }
    else
    {
        loaded = false;
    }
   
    return loaded;
}


/********************************************
Function to load the respective shaders and
textures and call the drawelements method
********************************************/
bool gl_Model::Draw( int mode )
{
    switch ( mode )
    {
        case 0:
            if ( !loaded )
                return false;

            for ( unsigned int i = 0; i < blocks.size( ); i++ )
            {
                if ( blocks[i].mat != "" )
                {
                    if ( mats[blocks[i].mat].shader != "" )
                    {
                        Load_Uniform( 1, mats[blocks[i].mat].shader, "Ns", mats[blocks[i].mat].Ns );
                        Load_Uniform( 3, mats[blocks[i].mat].shader, "Ka", mats[blocks[i].mat].Ka_x,
                            mats[blocks[i].mat].Ka_y, mats[blocks[i].mat].Ka_z );
                        Load_Uniform( 3, mats[blocks[i].mat].shader, "Kd", mats[blocks[i].mat].Kd_x,
                            mats[blocks[i].mat].Kd_y, mats[blocks[i].mat].Kd_z );
                        Load_Uniform( 3, mats[blocks[i].mat].shader, "Ks", mats[blocks[i].mat].Ks_x,
                            mats[blocks[i].mat].Ks_y, mats[blocks[i].mat].Ks_z );
                        Load_Uniform( 1, mats[blocks[i].mat].shader, "Ni", mats[blocks[i].mat].Ni );
                        Load_Uniform( 1, mats[blocks[i].mat].shader, "d", mats[blocks[i].mat].d );
                        Load_Uniform( 5, mats[blocks[i].mat].shader, "illum", mats[blocks[i].mat].illum );

                        shaders[mats[blocks[i].mat].shader].Activate( );
                    }

                    if ( mats[blocks[i].mat].bump != "" )
                    {
                        glActiveTexture( GL_TEXTURE0 + mats[blocks[i].mat].textures.size( ) );
                        if ( mats[blocks[i].mat].shader == "" )
                        {
                            glEnable( GL_TEXTURE_2D );
                        }
                        glBindTexture( GL_TEXTURE_2D, bumps[mats[blocks[i].mat].bump].texture_buffer_handle );
                    }

                    if ( mats[blocks[i].mat].textures.size( ) )
                    {
                        for ( unsigned int j = 0; j < mats[blocks[i].mat].textures.size( ); j++ )
                        {
                            glActiveTexture( GL_TEXTURE0 + j );
                            if ( mats[blocks[i].mat].shader == "" )
                            {
                                glEnable( GL_TEXTURE_2D );
                            }
                            glBindTexture( GL_TEXTURE_2D, textures[mats[blocks[i].mat].textures[j]].texture_buffer_handle );
                        }
                    }

                    if ( mats[blocks[i].mat].cubetextures.size( ) )
                    {
                        int b_offset = mats[blocks[i].mat].bump != "" ? 1 : 0;

                        for ( unsigned int j = 0; j < mats[blocks[i].mat].cubetextures.size( ); j++ )
                        {
                            glActiveTexture( GL_TEXTURE0 + mats[blocks[i].mat].textures.size( ) + b_offset + j );
                            if ( mats[blocks[i].mat].shader == "" )
                            {
                                glEnable( GL_TEXTURE_CUBE_MAP );
                            }
                            glBindTexture( GL_TEXTURE_CUBE_MAP, cubetextures[mats[blocks[i].mat].cubetextures[j]].texture_buffer_handle );
                        }
                    }
                }

                glEnableClientState( GL_VERTEX_ARRAY );
                glEnableClientState( GL_NORMAL_ARRAY );
                glEnableClientState( GL_TEXTURE_COORD_ARRAY );
                //glEnableClientState( GL_COLOR_ARRAY );

                //if ( blocks[i].tangent_buffer_handle != 0 )
                //{
                //    glEnableVertexAttribArray( blocks[i].tangent_buffer_handle );
                //    glVertexAttribPointer( blocks[i].tangent_buffer_handle, 3, GL_FLOAT, GL_FALSE, 0, &blocks[i].tangents[0] );
                //}

                //if ( blocks[i].attribute_buffer_handle != 0 )
                //{
                //    glEnableVertexAttribArray( blocks[i].attribute_buffer_handle );
                //    glVertexAttribPointer( blocks[i].attribute_buffer_handle, 3, GL_FLOAT, GL_FALSE, 0, &blocks[i].vas[0] );
                //}

                glBindVertexArray( blocks[i].vertex_array_handle );
                glDrawElements( GL_TRIANGLES, blocks[i].indexs.size( ), GL_UNSIGNED_INT, NULL );
                glBindVertexArray( 0 );
        
                //if ( blocks[i].attribute_buffer_handle != 0 )
                //{
                //    glDisableVertexAttribArray( blocks[i].attribute_buffer_handle );
                //}
        
                //if( blocks[i].tangent_buffer_handle != 0 )
                //{
                //    glDisableVertexAttribArray( blocks[i].tangent_buffer_handle );
                //}
                
                //glDisableClientState( GL_COLOR_ARRAY );
                glDisableClientState( GL_TEXTURE_COORD_ARRAY );
                glDisableClientState( GL_NORMAL_ARRAY );
                glDisableClientState( GL_VERTEX_ARRAY );
                
                if ( mats[blocks[i].mat].shader != "" )
                {
                    shaders[mats[blocks[i].mat].shader].DeActivate( );
                }

                if ( mats[blocks[i].mat].textures.size( ) )
                {
                    for ( unsigned int j = 0; j < mats[blocks[i].mat].textures.size( ); j++ )
                    {
                        glActiveTexture( GL_TEXTURE0 + j );
                        if ( mats[blocks[i].mat].shader == "" )
                        {
                            glDisable( GL_TEXTURE_2D );
                        }
                        glBindTexture( GL_TEXTURE_2D, 0 );
                    }
                }

                if ( mats[blocks[i].mat].cubetextures.size( ) )
                {
                    int b_offset = mats[blocks[i].mat].bump != "" ? 1 : 0;

                    for ( unsigned int j = 0; j < mats[blocks[i].mat].cubetextures.size( ); j++ )
                    {
                        glActiveTexture( GL_TEXTURE0 + mats[blocks[i].mat].textures.size( ) + b_offset );
                        if ( mats[blocks[i].mat].shader == "" )
                        {
                            glDisable( GL_TEXTURE_CUBE_MAP );
                        }
                        glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
                    }
                }

                if ( mats[blocks[i].mat].bump != "" )
                {
                    glActiveTexture( GL_TEXTURE0 + mats[blocks[i].mat].textures.size( ) );
                    if ( mats[blocks[i].mat].shader == "" )
                    {
                        glDisable( GL_TEXTURE_2D );
                    }
                    glBindTexture( GL_TEXTURE_2D, 0 );
                }
            }
            break;
    }
    return true;
}


/******************************************************
Function to save the model in krm format a binary
format that includes the vertex, normal, and index
values along with supplemental information for blocks
and materials and shader names.
******************************************************/
bool gl_Model::Save( char* save_path, float scale_f )
{
    //if ( save_path != NULL && loaded )
    //{
    //    string sp;
    //    sp.assign( save_path );
    //    sp.append( ".krm" );
    //    ofstream save_file;
    //    save_file.open( sp.c_str( ) , ios::binary );

    //    unsigned int ui_var = 0;
    //    float f_var = 0.0f;
    //    short s_var = 0;
    //    int i_var = 0;
    //
    //    /**************************************
    //    Write Header Info
    //    **************************************/
    //    /**********************
    //    # of blocks
    //    **********************/
    //    ui_var = blocks.size();
    //    save_file.write( (char *) &ui_var, sizeof(unsigned int) );

    //    /**********************
    //    # of mats
    //    **********************/
    //    ui_var = mats.size();
    //    save_file.write( (char*) &ui_var, sizeof(unsigned int) );

    //    /**********************
    //    # of shaders
    //    **********************/
    //    ui_var = shaders.size();
    //    save_file.write( (char*) &ui_var, sizeof(unsigned int) );

    //    /**********************
    //    # of textures
    //    **********************/
    //    ui_var = textures.size();
    //    save_file.write( (char*) &ui_var, sizeof(unsigned int) );

    //    /**********************
    //    # of cubetextures
    //    **********************/
    //    ui_var = cubetextures.size();
    //    save_file.write( (char*) &ui_var, sizeof(unsigned int) );

    //    /**********************
    //    # of bumps
    //    **********************/
    //    ui_var = bumps.size();
    //    save_file.write( (char*) &ui_var, sizeof(unsigned int) );

    //    /**************************************
    //    Begin Writting Blocks
    //    **************************************/
    //    for ( unsigned int i = 0; i < blocks.size(); i++ )
    //    {
    //        /*****************************
    //        Print Block Material Name
    //        0 tag
    //        *****************************/
    //        s_var = 0;
    //        save_file.write( (char*) &s_var, sizeof(short) );
    //        //Name Size
    //        ui_var = strlen( blocks[i].mat.c_str() );
    //        save_file.write( (char*) &ui_var, sizeof(unsigned int) );
    //        //Mat Name
    //        save_file.write( blocks[i].mat.c_str(), sizeof(char) * ui_var );

    //        /*****************************
    //        Print Block Verts
    //        1 tag
    //        *****************************/
    //        s_var = 1;
    //        save_file.write( (char*) &s_var, sizeof(short) );
    //        //Number of Verts
    //        ui_var = blocks[i].verts.size();
    //        save_file.write( (char*) &ui_var, sizeof(unsigned int) );
    //        //Verts Data
    //        for ( unsigned int v = 0; v < blocks[i].verts.size(); v++ )
    //        {
    //            f_var = blocks[i].verts[v];
    //            save_file.write( (char*) &f_var, sizeof(float) );
    //        }

    //        /******************************
    //        Print Block Norms
    //        2 tag
    //        ******************************/
    //        s_var = 2;
    //        save_file.write( (char*) &s_var, sizeof(short) );
    //        //Nummber of Norms
    //        ui_var = blocks[i].norms.size();
    //        save_file.write( (char*) &ui_var, sizeof(unsigned int) );
    //        //Norms Data
    //        for ( unsigned int n = 0; n < blocks[i].norms.size(); n++ )
    //        {
    //            f_var = blocks[i].norms[n];
    //            save_file.write( (char*) &f_var, sizeof(float) );
    //        }

    //        /*******************************
    //        Print Block Tangents
    //        3 tag
    //        *******************************/
    //        s_var = 3;
    //        save_file.write( (char*) &s_var, sizeof(short) );
    //        //Number of Tangents
    //        ui_var = blocks[i].tangents.size();
    //        save_file.write( (char*) &ui_var, sizeof(unsigned int) );
    //        //Tangents Data
    //        for ( unsigned int t = 0; t < blocks[i].tangents.size(); t++ )
    //        {
    //            f_var = blocks[i].tangents[t];
    //            save_file.write( (char*) &f_var, sizeof(float) );
    //        }

    //        /******************************
    //        Print Block Texture Coords
    //        4 tag
    //        ******************************/
    //        s_var = 4;
    //        save_file.write( (char*) &s_var, sizeof(short) );
    //        //Number of VTs
    //        ui_var = blocks[i].vts.size();
    //        save_file.write( (char*) &ui_var, sizeof(unsigned int) );
    //        //VT Data
    //        for ( unsigned int vt = 0; vt < blocks[i].vts.size(); vt++ )
    //        {
    //            f_var = blocks[i].vts[vt];
    //            save_file.write( (char*) &f_var, sizeof(float) );
    //        }

    //        /*******************************
    //        Print Block Indices
    //        5 tag
    //        *******************************/
    //        s_var = 5;
    //        save_file.write( (char*) &s_var, sizeof(short) );
    //        //Number of Indices
    //        ui_var = blocks[i].indexs.size();
    //        save_file.write( (char*) &ui_var, sizeof(unsigned int) );
    //        //Index Data
    //        for ( unsigned int in = 0; in < blocks[i].indexs.size(); in++ )
    //        {
    //            ui_var = blocks[i].indexs[in];
    //            save_file.write( (char*) &ui_var, sizeof(unsigned int) );
    //        }
    //    }

    //    save_file.close( );
    //    save_file.clear( );

    //    return true;
    //}

    return false;
}


/*************************************************
Upload a Uniform value into the specified shader
0 - 2D sampler
1 - 1f
2 - 2f
3 - 3f
4 - 4f

5 - 1i
6 - 2i
7 - 3i
8 - 4i
*************************************************/
bool gl_Model::Load_Uniform( GLuint u_type, string shader_name, string uniform_name,
            float v_1, float v_2, float v_3, float v_4 )
{
    map<string, gl_Shader>::iterator sit = shaders.find( shader_name );
    
    if ( sit != shaders.end( ) )
    {
        bool flag = sit->second.Is_Active();
        sit->second.Activate( );

        bool good;
        switch ( u_type )
        {
            case 0:
                good = sit->second.set_uniform_sampler_2d( uniform_name.c_str( ), (int)v_1 );
                break;
            case 1:
                good = sit->second.set_uniform_1f( uniform_name.c_str( ), v_1 );
                break;
            case 2:
                good = sit->second.set_uniform_2f( uniform_name.c_str( ), v_1, v_2 );
                break;
            case 3:
                good = sit->second.set_uniform_3f( uniform_name.c_str( ), v_1, v_2, v_3 );
                break;
            case 4:
                good = sit->second.set_uniform_4f( uniform_name.c_str( ), v_1, v_2, v_3, v_4 );
                break;
            case 5:
                good = sit->second.set_uniform_1i( uniform_name.c_str( ), (int)v_1 );
                break;
            case 6:
                good = sit->second.set_uniform_2i( uniform_name.c_str( ), (int)v_1, (int)v_2 );
                break;
            case 7:
                good = sit->second.set_uniform_3i( uniform_name.c_str( ), (int)v_1, (int)v_2, (int)v_3 );
                break;
            case 8:
                good = sit->second.set_uniform_4i( uniform_name.c_str( ), (int)v_1, (int)v_2, (int)v_3, (int)v_4 );
                break;
            default:
                good = false;
                break;
        }

        if ( !flag )
            sit->second.DeActivate( );

        return good;
    }

    return false;
}


/*****************************************
Load and Parse Material File
*****************************************/
bool gl_Model::Load_Material( const char* material_file )
{
    bool flag = true;
    ifstream material( material_file);

    if ( material.good( ) )
    {
        string mat_name = "";
        gl_Shader new_shader;

        while ( !material.eof( ) )
        {
            string token = "";
            material >> token;

            if ( token == "#" )
            {
                 material.ignore( LONG_MAX, '\n' );
            }
            else if ( token == "newmtl" )
            {
                material >> mat_name;
                mats[mat_name] = MATERIAL( );
                new_shader = gl_Shader();
            }
            else if ( token == "attribute" )
            {
                string attribute;
                material >> attribute;
                //glGenBuffers( 1, &mats[mat_name].attrib_handle );
                //new_shader.bind_attribute( attribute.c_str( ), mats[mat_name].attrib_handle );
            }
            else if ( token == "shader" )
            {
                if ( mat_name == "" )
                {
                    flag = false;
                    break;
                }

                string s_name, vert, frag;
                material >> s_name;
                material >> vert;
                material >> frag;

                map<string, gl_Shader>::iterator sit = shaders.find( s_name );

                if ( sit == shaders.end( ) )
                {
                    if ( !new_shader.create_shaders( (GLchar*)vert.c_str( ), (GLchar*)frag.c_str( ) ) )
                    {
                        shaders[s_name] = new_shader;
                    }
                    else
                    {
                        flag = false;
                        break;
                    }
                }

                mats[mat_name].shader = s_name;
            }
            else if ( token == "texture" )
            {
                if ( mat_name == "" )
                {
                    flag = false;
                    break;
                }

                string t_name, tex;
                material >> t_name;
                material >> tex;

                map<string, TEXTURE_2D_RGB>::iterator mit = textures.find( t_name );

                if ( mit == textures.end( ) )
                {
                    textures[t_name] = TEXTURE_2D_RGB( );

                    if ( textures[t_name].LoadBitmapFile( (GLchar*)tex.c_str( ) ) )
                    {
                        glGenTextures( 1, &(textures[t_name].texture_buffer_handle) );
	                    glBindTexture( GL_TEXTURE_2D, textures[t_name].texture_buffer_handle );
	                    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	                    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	                    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
                        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
                        //glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	                    //glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
                        //glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB,textures[t_name].width, textures[t_name].height,
                        //0, GL_RGB, GL_UNSIGNED_BYTE, &textures[t_name].rgb_array[0] );
                        gluBuild2DMipmaps( GL_TEXTURE_2D, 3, textures[t_name].width, textures[t_name].height,
                                GL_RGB, GL_UNSIGNED_BYTE, &textures[t_name].rgb_array[0] );
                        glBindTexture( GL_TEXTURE_2D, 0 );
                    }
                    else
                    {
                        flag = false;
                        break;
                    }
                }

                mats[mat_name].textures.push_back( t_name );
            }
            else if ( token == "cube" )
            {
                if ( mat_name == "" )
                {
                    flag = false;
                    break;
                }

                string t_name, tex;
                material >> t_name;
                material >> tex;

                map<string, TEXTURE_CUBE>::iterator mit = cubetextures.find( t_name );

                if ( mit == cubetextures.end( ) )
                {
                    cubetextures[t_name] = TEXTURE_CUBE( );

                    if ( cubetextures[t_name].LoadBitmapFile( (GLchar*)tex.c_str( ) ) )
                    {
                        unsigned int tw = cubetextures[t_name].width;
                        unsigned int th = cubetextures[t_name].height;

                        glGenTextures( 1, &(cubetextures[t_name].texture_buffer_handle) );
	                    glBindTexture( GL_TEXTURE_CUBE_MAP, cubetextures[t_name].texture_buffer_handle );
	                    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP );
	                    glTexParameteri ( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP );
	                    glTexParameteri ( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	                    glTexParameteri ( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
                        glTexImage2D ( GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, tw, th,
                            0, GL_RGB, GL_UNSIGNED_BYTE, &cubetextures[t_name].rgb_array[0] );
                        glTexImage2D ( GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, tw, th,
                            0, GL_RGB, GL_UNSIGNED_BYTE, &cubetextures[t_name].rgb_array[tw*th*3] );
                        glTexImage2D ( GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, tw, th,
                            0, GL_RGB, GL_UNSIGNED_BYTE, &cubetextures[t_name].rgb_array[tw*th*3*2] );
                        glTexImage2D ( GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, tw, th,
                            0, GL_RGB, GL_UNSIGNED_BYTE, &cubetextures[t_name].rgb_array[tw*th*3*3] );
                        glTexImage2D ( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, tw, th,
                            0, GL_RGB, GL_UNSIGNED_BYTE, &cubetextures[t_name].rgb_array[tw*th*3*4] );
                        glTexImage2D ( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, tw, th,
                            0, GL_RGB, GL_UNSIGNED_BYTE, &cubetextures[t_name].rgb_array[tw*th*3*5] );
                        glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
                    }
                    else
                    {
                        flag = false;
                        break;
                    }

					mats[mat_name].cubetextures.push_back( t_name );
                }

                
            }
            else if ( token == "bump" )
            {
                if ( mat_name == "" )
                {
                    flag = false;
                    break;
                }

                string t_name, tex;
                material >> t_name;
                material >> tex;

                map<string, TEXTURE_2D_RGB>::iterator mit = bumps.find( t_name );

                if ( mit == bumps.end( ) )
                {
                    bumps[t_name] = TEXTURE_2D_RGB( );

                    if ( bumps[t_name].LoadBitmapFile( (GLchar*)tex.c_str( ) ) )
                    {
                        glGenTextures( 1, &(bumps[t_name].texture_buffer_handle) );
	                    glBindTexture( GL_TEXTURE_2D, bumps[t_name].texture_buffer_handle );
	                    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	                    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	                    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	                    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
                        glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, bumps[t_name].width, bumps[t_name].height,
                            0, GL_RGB, GL_UNSIGNED_BYTE, &bumps[t_name].rgb_array[0] );
                        glBindTexture( GL_TEXTURE_2D, 0 );
                    }
                    else
                    {
                        flag = false;
                        break;
                    }

                }

                mats[mat_name].bump = t_name;
            }
            else if ( token == "Ns" )
            {
                if ( mat_name == "" )
                {
                    flag = false;
                    break;
                }

                material >> mats[mat_name].Ns;
            }
            else if ( token == "Ka" )
            {
                if ( mat_name == "" )
                {
                    flag = false;
                    break;
                }

                material >> mats[mat_name].Ka_x;
                material >> mats[mat_name].Ka_y;
                material >> mats[mat_name].Ka_z;
            }
            else if ( token == "Kd" )
            {
                if ( mat_name == "" )
                {
                    flag = false;
                    break;
                }
            
                material >> mats[mat_name].Kd_x;
                material >> mats[mat_name].Kd_y;
                material >> mats[mat_name].Kd_z;
            }
            else if ( token == "Ks" )
            {
                if ( mat_name == "" )
                {
                    flag = false;
                    break;
                }
            
                material >> mats[mat_name].Ks_x;
                material >> mats[mat_name].Ks_y;
                material >> mats[mat_name].Ks_z;
            }
            else if ( token == "Ni" )
            {
                if ( mat_name == "" )
                {
                    flag = false;
                    break;
                }
                
                material >> mats[mat_name].Ni;
            }
            else if ( token == "d" )
            {
                if ( mat_name == "" )
                {
                    flag = false;
                    break;
                }
                
                material >> mats[mat_name].d;
            }
            else if ( token == "illum" )
            {
                if ( mat_name == "" )
                {
                    flag = false;
                    break;
                }
                
                material >> mats[mat_name].illum;
            }
        }
    }
    else
    {
        flag = false;
    }

    material.close( );
    material.clear( );

    return flag;
}

