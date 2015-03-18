/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2014  <copyright holder> <email>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _ARCAMERA_H__
#define _ARCAMERA_H__

#include "PCH.h"

namespace ArOgre
{
/** A viewpoint from which the scene will be rendered.
*/
class ArCamera
{
public:
  /** Standard constructor.
  */
  ArCamera ( Ogre::SceneManager* sManager, const Ogre::String& nameCamera );

  /** Standard destructor.
  */
  virtual ~ArCamera();

  /** Load configuration file to config the new Ogre::camera.
  */
  bool loadConfig ( const Ogre::String& pathConfig );
  
  /** Sets the aspect ratio camera.
  */
  void setAspectRatio ( Ogre::Real ar )
  {
    mCamera->setAspectRatio ( ar );
  }
  
  /** Sets the projection matrix.
  */
  void setProjectionMatrix(Ogre::Matrix4 matProj)
  {
    mCamera->setCustomProjectionMatrix(true, matProj); 
  }
    
  /** Sets the view matrix.
  */
  void setViewMatrix(Ogre::Matrix4 matView)
  {
    mCamera->setCustomViewMatrix(true, matView);
  }
  
  /** Returns the Ogres camera.
  */
  Ogre::Camera* getOgreCamera() 
  { 
    return mCamera;
  }

  /** Returns the Ogres camera name.
  */
  const String getName() 
  {
    return mCamName;
  }

  /** Returns the camera parameters from aruco.
  */
  aruco::CameraParameters& getCameraParameters() 
  {
    return mCameraParams;
  } 

  
private:
  /// Scene manager responsible for the scene.
  Ogre::SceneManager* mSceneMgr;
  
  /// New Ogre camera parameters.
  Ogre::Camera* mCamera;			// Ogre camera
  Ogre::Vector3 mPosition;			// Camera position - default (0,0,0)
  Ogre::Quaternion mOrientation;		// Camera orientation, quaternion style
  Ogre::Vector3 mLookAt;			// Objetive
  Ogre::String mCamName;			// Name camera

  /// ArUco camera parameters
  aruco::CameraParameters mCameraParams;	// ArUco cameraParamaters
};
} // namespace ArOgre

#endif // define _ARCAMERA_H__
