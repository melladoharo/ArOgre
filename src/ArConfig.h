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

#ifndef _ARCONFIG_H__
#define _ARCONFIG_H__

#include "PCH.h"

namespace ArOgre
{
class ArConfig
{
public:
  ArConfig();
  ~ArConfig();
  
  /** Loads the configuration file (arogre.cfg).
   *  Returns true if the parameters are corrects. False in other case.
  */
  bool loadConfiguration();
  
  /** Returns the input video path.
  */
  Ogre::String getPathVideo() 
  {
    return mPathVideo;
  }
  
  /** Returns the camera file path.
  */
  Ogre::String getPathCamera() 
  {
    return mPathCameraParam;
  }
  
  /** Returns the id device
  */
  unsigned int getIdDevice()
  {
    return mIdDevice;
  }
  
  /** Returns the capture size set manually
  */
  bool getCaptureSize()
  {
    return mCaptureSize;
  }
  
  /** Returns the capture height set manually
  */
  unsigned int getCaptureHeight()
  {
    return mCaptureHeight;
  }
  
  /** Returns the capture width set manually
  */
  unsigned int getCaptureWidth()
  {
    return mCaptureWidth;
  }
  
  bool getLoopVideo()
  {
    return mLoopVideo;
  }

  
  /** Returns the board file path.
  */
  Ogre::String getPathBoard() 
  {
    return mPathBoardParam;
  }
  
  /** Returns the board probablity
  */
  Ogre::Real getBoardProbability()
  {
    return mBoardProbability;
  }
  
  /** Returns the marker size.
  */
  Ogre::Real getMarkerSize() 
  {
    return mMarkerSize;
  }
  
  
private:
  /** Creates the arogre.cfg file with the default parameters.
   *  Returns true if the file has been created correctly. Returns false otherwise.
  */
  bool createDefaultConfig();
  
  Ogre::String mPathVideo;
  unsigned int mIdDevice;
  bool mCaptureSize;
  unsigned int mCaptureHeight;
  unsigned int mCaptureWidth;
  bool mLoopVideo;
  Ogre::String mPathBoardParam;
  Ogre::String mPathCameraParam;
  Ogre::Real mBoardProbability;
  Ogre::Real mMarkerSize;

};
} // namespace ArOgre

#endif // _ARCONFIG_H__