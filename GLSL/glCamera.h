/***************************************************************
Name: Kenny Moser	krm104@gmail.com
Course: ECE8990
File:   glCamera.h

This source file contains the prototypes of the gl_camera
class. As well as helper/friend functions.

Use all you like, but if you break it, don't reference me >.>

***************************************************************/


//duplicate include lock
#ifndef __GLSL_CAMERA_H__
#define __GLSL_CAMERA_H__


/****************************
Required Includes
****************************/
#include "glsl.h"

#include <cmath>


/***************************
Additional Includes
***************************/


/*******************************************
Camera Object
Contains the data members and methods for
manipulating a camera
*******************************************/
class gl_Camera
{
	/******************************
	Public Methods
	******************************/
    public:
        //Camera object default constructor
        gl_Camera( );
    
        //Camera object parameter constructor
        gl_Camera( GLfloat pos_x, GLfloat pos_y, GLfloat pos_z,
            GLfloat look_x, GLfloat look_y, GLfloat look_z,
            GLfloat up_x, GLfloat up_y, GLfloat up_z );

        //Camera object default destructor
        ~gl_Camera( );

        //Translate the Camera coordinates to a new location
        bool Movef( GLfloat pos_x, GLfloat pos_y, GLfloat pos_z );

        //Translate the Camera coordinates along the specified axis
        //0 = camera_x; 1 = camera_y; 2 = camera_z; 3 = world_x;
        //4 = world_y; 5 = world_z
        bool Translatef( GLfloat inc, GLshort axis );

        //Rotate the Camera about its x axis to look in a different direction
        bool RotateCamerax( GLfloat ang );

        //Rotate the Camera about its y axis to look in a different direction
        bool RotateCameray( GLfloat ang );
    
        //Rotate the Camera about its z axis to look in a different direction
        bool RotateCameraz( GLfloat ang );
    
        //Rotate the Camera about some other axis to look in a different direction
        bool RotateArbitrary( GLfloat ang, GLfloat axis_x, GLfloat axis_y, GLfloat axis_z );

		//Reset the rotation matrix to Identity
		bool ResetRotation();

		//Set the Scene Camera to look in the camera's direction
		bool SetLookAt();

        /*Retrieve the camera position values
		0 - 2 Position
		3 - 5 Look
		5 - 8 Up*/
        const GLfloat& operator[]( int i );

	/**********************
	Public Members
	**********************/
		GLfloat transform_mat[9];

	/************************
	Private Methods
	************************/
    private:
        //Update look at point coordinates
        bool update_look( );

        //Recalculate the Axis based on the rotation matrix supplied
        bool update_axis( GLfloat *R );

        //Update the transformation matrix with the new rotation
        bool mult_matrix( GLfloat *R );

	/***********************
	Private Members
	***********************/
	public:
        //Data Members of the Camera Object
        GLfloat position[3];
        GLfloat look[3];
        GLfloat x_axis[3];
        GLfloat y_axis[3];
        GLfloat z_axis[3];
};


#endif  //__GLSL_CAMERA_H__

