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

#include "PCH.h"
#include "ArCapturer.h"

namespace ArOgre
{
ArCapturer::ArCapturer( aruco::CameraParameters camParams )
  : mCameraParams(camParams),
    mLoopVideo(false)
{
  Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing ArCapturer ***"); 
}
//////////////////////////////////////////////////////////////////////////////////////////////
ArCapturer::~ArCapturer()
{
  Ogre::LogManager::getSingleton().logMessage("*-*-*  ArCapturer Removed ");
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool ArCapturer::loadStream ( const Ogre::String& pathCapture, unsigned int idDevice,
  bool captureSize, unsigned int captureHeight, unsigned int captureWidth, bool loop)
{
  // input stream type, Live camera input
  if(pathCapture=="LIVE")
  {
    mVideoCapturer.open(idDevice);
    
    // set capture height and width manually
    if(captureSize)
    { 
      mVideoCapturer.set(CV_CAP_PROP_FRAME_HEIGHT, captureHeight);
      mVideoCapturer.set(CV_CAP_PROP_FRAME_WIDTH, captureWidth);
    }
  }
  else // video input
  {
    mVideoCapturer.open(pathCapture);
    mLoopVideo=loop;
  }

  // error?
  if(mVideoCapturer.isOpened())
  {
    Ogre::LogManager::getSingletonPtr()->logMessage("[ArCapturer] Stream loaded succesfully.");
    return true;
  }
  
  Ogre::LogManager::getSingletonPtr()->logMessage("[ArCapturer] Error! Could not open the input stream.");
  return false;
} 
//////////////////////////////////////////////////////////////////////////////////////////////
bool ArCapturer::capture()
{
  // capture and convert the image
  if (mVideoCapturer.grab() && mVideoCapturer.retrieve(mInputImage))
  {
    // adjust the paramaters
    mCameraParams.resize(mInputImage.size());
    
    // remove distorion in final image [mInputImageUnd]
    cv::undistort(
      mInputImage, 
      mInputImageUnd, 
      mCameraParams.CameraMatrix, 
      mCameraParams.Distorsion);
      
    return true;
  }
  
  // loop video?
  if(mVideoCapturer.get(CV_CAP_PROP_POS_AVI_RATIO==1) && mLoopVideo)
    mVideoCapturer.set(CV_CAP_PROP_POS_AVI_RATIO, 0);
  
  return false;
}

} // namespace ArOgre


