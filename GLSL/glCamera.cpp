/**************************************************************
Name: Kenny Moser	krm104@gmail.com
Course: ECE8990
File:   glCamera.cpp

This source file contains the definitions of the gl_camera
class. As well as helper/friend functions.

Use all you like, but if you break it, don't reference me >.>

**************************************************************/


/************************
Required Includes
************************/
#include "glCamera.h"


/***********************
Additional Includes
***********************/


/**********************************
gl_Camera Function Definitions
**********************************/
/*************************
Default Constructor
*************************/
gl_Camera::gl_Camera( )
{
    position[0] = 0.0;
    position[1] = 0.0;
    position[2] = 1.0;

    x_axis[0] = 1.0;
    x_axis[1] = 0.0;
    x_axis[2] = 0.0;

    y_axis[0] = 0.0;
    y_axis[1] = 1.0;
    y_axis[2] = 0.0;

    z_axis[0] = 0.0;
    z_axis[1] = 0.0;
    z_axis[2] = 1.0;
        
    update_look( );

    transform_mat[0] = 1.0f;
    transform_mat[1] = 0.0f;
    transform_mat[2] = 0.0f;
    transform_mat[3] = 0.0f;
    transform_mat[4] = 1.0f;
    transform_mat[5] = 0.0f;
    transform_mat[6] = 0.0f;
    transform_mat[7] = 0.0f;
    transform_mat[8] = 1.0f;
}


/*****************************
Parameter Constructor
*****************************/
gl_Camera::gl_Camera( GLfloat pos_x, GLfloat pos_y, GLfloat pos_z,
        GLfloat look_x, GLfloat look_y, GLfloat look_z,
        GLfloat up_x, GLfloat up_y, GLfloat up_z )
{
    position[0] = pos_x;
    position[1] = pos_y;
    position[2] = pos_z;

    y_axis[0] = up_x;
    y_axis[1] = up_y;
    y_axis[2] = up_z;

    z_axis[0] = -look_x;
    z_axis[1] = -look_y;
    z_axis[2] = -look_z;

    x_axis[0] = y_axis[1]*z_axis[2] - y_axis[2]*z_axis[1];
    x_axis[1] = y_axis[2]*z_axis[0] - y_axis[0]*z_axis[2];
    x_axis[2] = y_axis[0]*z_axis[1] - y_axis[1]*z_axis[0];

    update_look( );

    transform_mat[0] = 1.0f;
    transform_mat[1] = 0.0f;
    transform_mat[2] = 0.0f;
    transform_mat[3] = 0.0f;
    transform_mat[4] = 1.0f;
    transform_mat[5] = 0.0f;
    transform_mat[6] = 0.0f;
    transform_mat[7] = 0.0f;
    transform_mat[8] = 1.0f;
}


/******************************
Default Destructor
******************************/
gl_Camera::~gl_Camera( )
{

}


/*****************************
Update Look At Point
*****************************/
bool gl_Camera::update_look( )
{
    look[0] = position[0] + .5f * -z_axis[0];
    look[1] = position[1] + .5f * -z_axis[1];
    look[2] = position[2] + .5f * -z_axis[2];

    return true;
}


/**************************************************
Translate the Camera Coordinates to a new location
**************************************************/
bool gl_Camera::Movef( GLfloat pos_x, GLfloat pos_y, GLfloat pos_z )
{
    position[0] = pos_x;
    position[1] = pos_y;
    position[2] = pos_z;

    return update_look( );
}


/*************************************************
Translate the Camera along the specified axis
0 = x; 1 = y; 2 = z
*************************************************/
bool gl_Camera::Translatef( GLfloat inc, GLshort axis )
{
    switch( axis )
    {
        //Camera X
        case 0:
            position[0] = position[0] + inc * x_axis[0];
            position[1] = position[1] + inc * x_axis[1];
            position[2] = position[2] + inc * x_axis[2];
            break;

        //Camera Y
        case 1:
            position[0] = position[0] + inc * y_axis[0];
            position[1] = position[1] + inc * y_axis[1];
            position[2] = position[2] + inc * y_axis[2];
            break;

        //Camera Z
        case 2:
            position[0] = position[0] + inc * z_axis[0];
            position[1] = position[1] + inc * z_axis[1];
            position[2] = position[2] + inc * z_axis[2];
            break;

        //World X
        case 3:
            position[0] = position[0] + inc * 1;
            //position[1] = position[1] + inc * 0;
            //position[2] = position[2] + inc * 0;
            break;

        //World Y
        case 4:
            //position[0] = position[0] + inc * 0;
            position[1] = position[1] + inc * 1;
            //position[2] = position[2] + inc * 0;
            break;

        //World Z
        case 5:
            //position[0] = position[0] + inc * 0;
            //position[1] = position[1] + inc * 0;
            position[2] = position[2] + inc * 1;
            break;

        default:
            break;
    }

    return update_look ( );
}


/**********************************************
Rotate the camera about its x axis
**********************************************/
bool gl_Camera::RotateCamerax( GLfloat ang )
{
    GLfloat x = x_axis[0];
    GLfloat y = x_axis[1];
    GLfloat z = x_axis[2];

    GLfloat cos_theta = cos( ang*3.14159f/180.0f );
    GLfloat sin_theta = sin( ang*3.14159f/180.0f );

    GLfloat R[9] = { (cos_theta + x*x*(1 - cos_theta)), (x*y*(1 - cos_theta) - z*sin_theta), (x*z*(1 - cos_theta) + y*sin_theta),
        (y*x*(1 - cos_theta) + z*sin_theta), (cos_theta + y*y*(1 - cos_theta)), (y*z*(1 - cos_theta) - x*sin_theta),
        (z*x*(1 - cos_theta) - y*sin_theta), (z*y*(1 - cos_theta) + x*sin_theta), (cos_theta + z*z*(1 - cos_theta)) };

    update_axis( R );

    mult_matrix( R );

    return update_look( );
}


/*********************************************
Rotate the camera about its y axis
*********************************************/
bool gl_Camera::RotateCameray( GLfloat ang )
{
    GLfloat x = y_axis[0];
    GLfloat y = y_axis[1];
    GLfloat z = y_axis[2];

    GLfloat cos_theta = cos( ang*3.14159f/180.0f );
    GLfloat sin_theta = sin( ang*3.14159f/180.0f );

    GLfloat R[9] = { (cos_theta + x*x*(1 - cos_theta)), (x*y*(1 - cos_theta) - z*sin_theta), (x*z*(1 - cos_theta) + y*sin_theta),
        (y*x*(1 - cos_theta) + z*sin_theta), (cos_theta + y*y*(1 - cos_theta)), (y*z*(1 - cos_theta) - x*sin_theta),
        (z*x*(1 - cos_theta) - y*sin_theta), (z*y*(1 - cos_theta) + x*sin_theta), (cos_theta + z*z*(1 - cos_theta)) };

    update_axis( R );

    mult_matrix( R );

    //update look at point
    return update_look( );
}


/**********************************************
Rotate the camera about its z axis
**********************************************/
bool gl_Camera::RotateCameraz( GLfloat ang )
{
    GLfloat x = z_axis[0];
    GLfloat y = z_axis[1];
    GLfloat z = z_axis[2];

    GLfloat cos_theta = cos( ang*3.14159f/180.0f );
    GLfloat sin_theta = sin( ang*3.14159f/180.0f );

    GLfloat R[9] = { (cos_theta + x*x*(1 - cos_theta)), (x*y*(1 - cos_theta) - z*sin_theta), (x*z*(1 - cos_theta) + y*sin_theta),
        (y*x*(1 - cos_theta) + z*sin_theta), (cos_theta + y*y*(1 - cos_theta)), (y*z*(1 - cos_theta) - x*sin_theta),
        (z*x*(1 - cos_theta) - y*sin_theta), (z*y*(1 - cos_theta) + x*sin_theta), (cos_theta + z*z*(1 - cos_theta)) };

    update_axis( R );

    mult_matrix( R );

    //update look at point
    return update_look( );
}


/***************************************************************************
Rotate the Camera about some other axis to look in a different direction
***************************************************************************/
bool gl_Camera::RotateArbitrary( GLfloat ang, GLfloat axis_x, GLfloat axis_y, GLfloat axis_z )
{
    GLfloat cos_theta = cos( ang*3.14159f/180.0f );
    GLfloat sin_theta = sin( ang*3.14159f/180.0f );

    GLfloat R[9] = { (cos_theta + axis_x*axis_x*(1 - cos_theta)), (axis_x*axis_y*(1 - cos_theta) - axis_z*sin_theta),
        (axis_x*axis_z*(1 - cos_theta) + axis_y*sin_theta), (axis_y*axis_x*(1 - cos_theta) + axis_z*sin_theta),
        (cos_theta + axis_y*axis_y*(1 - cos_theta)), (axis_y*axis_z*(1 - cos_theta) - axis_x*sin_theta),
        (axis_z*axis_x*(1 - cos_theta) - axis_y*sin_theta), (axis_z*axis_y*(1 - cos_theta) + axis_x*sin_theta),
        (cos_theta + axis_z*axis_z*(1 - cos_theta)) };

    update_axis( R );

    mult_matrix( R );

    //update look at point
    return update_look( );
}

/***********************************************
Reset the Rotation Matrix to Identity
***********************************************/
bool gl_Camera::ResetRotation()
{
	x_axis[0] = 1.0f;
	x_axis[1] = 0.0f;
	x_axis[2] = 0.0f;

	y_axis[0] = 0.0f;
	y_axis[1] = 1.0f;
	y_axis[2] = 0.0f;

	z_axis[0] = 0.0f;
	z_axis[1] = 0.0f;
	z_axis[2] = 1.0f;

	transform_mat[0] = 1.0f;
    transform_mat[1] = 0.0f;
    transform_mat[2] = 0.0f;
    transform_mat[3] = 0.0f;
    transform_mat[4] = 1.0f;
    transform_mat[5] = 0.0f;
    transform_mat[6] = 0.0f;
    transform_mat[7] = 0.0f;
    transform_mat[8] = 1.0f;

	update_look();

	return true;
}

/***********************************************
Set the Scene camera to look in the camera's
direction, using glulookat
***********************************************/
bool gl_Camera::SetLookAt()
{
	gluLookAt( position[0], position[1], position[2], look[0], look[1], look[2],
				y_axis[0], y_axis[1], y_axis[2]);

	return true;
}

/***********************************************
Retrieve the camera position
0 - 2 Position
3 - 5 Look
6 - 8 Up
***********************************************/
const GLfloat& gl_Camera::operator[]( int i )
{
    switch ( i )
    {
        case 0:
            return position[0];

        case 1:
            return position[1];

        case 2:
            return position[2];

        case 3:
            return look[0];

        case 4:
            return look[1];

        case 5:
            return look[2];

        case 6:
            return y_axis[0];

        case 7:
            return y_axis[1];

        case 8:
            return y_axis[2];

        default:
            return position[0];
    }
}


/******************************************************
Update the axis based on the rotation matrix supplied
******************************************************/
bool gl_Camera::update_axis( GLfloat *R )
{
    /////////////////////////////////////////////////
    //compute new x axis
    GLfloat new_x_axis_x = R[0]*x_axis[0] + R[1]*x_axis[1] + R[2]*x_axis[2];
    GLfloat new_x_axis_y = R[3]*x_axis[0] + R[4]*x_axis[1] + R[5]*x_axis[2];
    GLfloat new_x_axis_z = R[6]*x_axis[0] + R[7]*x_axis[1] + R[8]*x_axis[2];

    x_axis[0] = new_x_axis_x;
    x_axis[1] = new_x_axis_y;
    x_axis[2] = new_x_axis_z;

    //normalize
    GLfloat sqrt_v = sqrt( x_axis[0]*x_axis[0] + x_axis[1]*x_axis[1] + x_axis[2]*x_axis[2] );

    x_axis[0] /= sqrt_v;
    x_axis[1] /= sqrt_v;
    x_axis[2] /= sqrt_v;


    ///////////////////////////////////////////////
    //compute new y axis
    GLfloat new_y_axis_x = R[0]*y_axis[0] + R[1]*y_axis[1] + R[2]*y_axis[2];
    GLfloat new_y_axis_y = R[3]*y_axis[0] + R[4]*y_axis[1] + R[5]*y_axis[2];
    GLfloat new_y_axis_z = R[6]*y_axis[0] + R[7]*y_axis[1] + R[8]*y_axis[2];

    y_axis[0] = new_y_axis_x;
    y_axis[1] = new_y_axis_y;
    y_axis[2] = new_y_axis_z;

    //normalize
    sqrt_v = sqrt( y_axis[0]*y_axis[0] + y_axis[1]*y_axis[1] + y_axis[2]*y_axis[2] );

    y_axis[0] /= sqrt_v;
    y_axis[1] /= sqrt_v;
    y_axis[2] /= sqrt_v;


    //////////////////////////////////////////////////
    //compute new z axis
    GLfloat new_z_axis_x = R[0]*z_axis[0] + R[1]*z_axis[1] + R[2]*z_axis[2];
    GLfloat new_z_axis_y = R[3]*z_axis[0] + R[4]*z_axis[1] + R[5]*z_axis[2];
    GLfloat new_z_axis_z = R[6]*z_axis[0] + R[7]*z_axis[1] + R[8]*z_axis[2];

    z_axis[0] = new_z_axis_x;
    z_axis[1] = new_z_axis_y;
    z_axis[2] = new_z_axis_z;

    //normalize
    sqrt_v = sqrt( z_axis[0]*z_axis[0] + z_axis[1]*z_axis[1] + z_axis[2]*z_axis[2] );

    z_axis[0] /= sqrt_v;
    z_axis[1] /= sqrt_v;
    z_axis[2] /= sqrt_v;


    return true;
}


/*******************************************************
Update the transformation matrix with the new rotation
*******************************************************/
bool gl_Camera::mult_matrix( GLfloat *R )
{
    GLfloat new_trans[9];

    //Row 1 Colm 1
    new_trans[0] = R[0]*transform_mat[0] + R[1]*transform_mat[3] + R[2]*transform_mat[6];
    //Row 1 Colm 2
    new_trans[1] = R[0]*transform_mat[1] + R[1]*transform_mat[4] + R[2]*transform_mat[7];
    //Row 1 Colm 3
    new_trans[2] = R[0]*transform_mat[2] + R[1]*transform_mat[5] + R[2]*transform_mat[8];

    //Row 2 Colm 1
    new_trans[3] = R[3]*transform_mat[0] + R[4]*transform_mat[3] + R[5]*transform_mat[6];
    //Row 2 Colm 2
    new_trans[4] = R[3]*transform_mat[1] + R[4]*transform_mat[4] + R[5]*transform_mat[7];
    //Row 2 Colm 3
    new_trans[5] = R[3]*transform_mat[2] + R[4]*transform_mat[5] + R[5]*transform_mat[8];

    //Row 3 Colm 1
    new_trans[6] = R[6]*transform_mat[0] + R[7]*transform_mat[3] + R[8]*transform_mat[6];
    //Row 3 Colm 2
    new_trans[7] = R[6]*transform_mat[1] + R[7]*transform_mat[4] + R[8]*transform_mat[7];
    //Row 3 Colm 3
    new_trans[8] = R[6]*transform_mat[2] + R[7]*transform_mat[5] + R[8]*transform_mat[8];

    transform_mat[0] = new_trans[0];
    transform_mat[1] = new_trans[1];
    transform_mat[2] = new_trans[2];
    transform_mat[3] = new_trans[3];
    transform_mat[4] = new_trans[4];
    transform_mat[5] = new_trans[5];
    transform_mat[6] = new_trans[6];
    transform_mat[7] = new_trans[7];
    transform_mat[8] = new_trans[8];

    return true;
}

