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
#include "ArConfig.h"

namespace ArOgre
{
  
ArConfig::ArConfig()
  : mPathVideo(Ogre::StringUtil::BLANK),
    mIdDevice(0),
    mCaptureSize(false),
    mCaptureHeight(0),
    mCaptureWidth(0),
    mLoopVideo(false),
    mPathBoardParam(Ogre::StringUtil::BLANK),
    mPathCameraParam(Ogre::StringUtil::BLANK),
    mBoardProbability(0),
    mMarkerSize(0)
{
  Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing ArConfig ***");
}
//////////////////////////////////////////////////////////////////////////////////////////////
ArConfig::~ArConfig()
{
  Ogre::LogManager::getSingleton().logMessage("*-*-*  ArConfig Removed");
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool ArConfig::loadConfiguration()
{
  ifstream fileCfg("arogre.cfg");
  
  if(fileCfg.good()) // exist the config file?
  {
    if(fileCfg.is_open()) // is open
    {
      Ogre::String line;
      
      while(getline (fileCfg, line)) // read line by line
      {
	if(line[0]=='/' && line[1]=='/') // coment line, 
	{
	}
	else if(line.find("InputVideo")!=Ogre::String::npos) // InputVideo
	{
	  mPathVideo=line.substr(11, line.size());
	}
	else if(line.find("IdDevice")!=Ogre::String::npos) // IdDevice
	{
	  mIdDevice=Ogre::StringConverter::parseInt(line.substr(9, line.size()));
	}
	else if(line.find("SetCaptureSize")!=Ogre::String::npos) // SetCaptureSize
	{
	  if(line.substr(15, line.size())=="On")
	    mCaptureSize=true;
	  else
	    mCaptureSize=false;
	}
	else if(line.find("CaptureHeight")!=Ogre::String::npos) // CaptureHeight
	{
	  mCaptureHeight=Ogre::StringConverter::parseInt(line.substr(14, line.size()));
	}
	else if(line.find("CaptureWidth")!=Ogre::String::npos) // CaptureWidth
	{
	  mCaptureWidth=Ogre::StringConverter::parseInt(line.substr(13, line.size()));
	}
	else if(line.find("LoopVideo")!=Ogre::String::npos) // LoopVideo
	{
	  if(line.substr(10, line.size())=="On")
	    mLoopVideo=true;
	  else
	    mLoopVideo=false;
	}
	else if(line.find("CameraCalibration")!=Ogre::String::npos) // CameraCalibration
	{
	  mPathCameraParam=line.substr(18, line.size());
	}
	else if(line.find("BoardInfo")!=Ogre::String::npos) // BoardInfo
	{
	  mPathBoardParam=line.substr(10, line.size());
	}
	else if(line.find("MarkerSize")!=Ogre::String::npos) // MarkerSize
	{
	  mMarkerSize=Ogre::StringConverter::parseReal(line.substr(11, line.size()));
	}
      }
      Ogre::LogManager::getSingletonPtr()->logMessage("[ArConfig] Configuration file loaded correctly.");
      fileCfg.close();
      return true;
    }
    else
    {
      Ogre::LogManager::getSingletonPtr()->logMessage("[ArConfig] Error loading the file arogre.cfg");
      return false;
    }
  }
  else
  {
    Ogre::LogManager::getSingletonPtr()->logMessage("[ArConfig] Error arogre.cfg does not exist. "
      "Creating a new config file, please configure it.");
    createDefaultConfig();
    return false;
  }
  
  return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool ArConfig::createDefaultConfig()
{
  ofstream file("arogre.cfg");
  
  if(file.is_open())
  {
    file<<"// ArOgre configuration file. All options must be configured.\n"
	  "// Available options:\n"
	  
	  "\t// \"InputVideo\"\n"
	  "\t// Specifies a input video file. To capture images from camera, pass \"LIVE\" option.\n"
	  "\t// Eg: InputVideo=LIVE (images from camera)\n"
	  "\t// Eg: InputVideo=pathToVideo.video (images from video)\n\n"
	  
	  "\t// \"IdDevice\"\n"
	  "\t// Is the id of video device. Set to 0 if you have only one camera.\n"
	  "\t// Eg: IdDevice=0\n\n"
	  
	  "\t// \"SetCaptureSize\"\n"
	  "\t// Set the captures dimensions manually on or off (captureHeight and captureWidth).\n"
	  "\t// Eg: SetCaptureSize=Off\n\n"
	  
	  "\t// \"CaptureHeight\"\n"
	  "\t// Set the captures height.\n"
	  "\t// Eg: CaptureHeight=480\n\n"
	  
	  "\t// \"CaptureWidth\"\n"
	  "\t// Set the captures width.\n"
	  "\t// Eg: CaptureWidth=640\n\n"
	  
	  "\t// \"LoopVideo\"\n"
	  "\t// Set the video repeat on or off.\n"
	  "\t// Eg: LoopVideo=On\n\n" 
	  
	  "\t// \"CameraCalibration\"\n"
	  "\t// ArUco's camera calibration file. Supported format: \".int\" and \".yml\".\n"
	  "\t// Eg: CameraCalibration=pathToFile.yml\n\n"
	  
	  "\t// \"BoardInfo\"\n"
	  "\t// File with the ArUco's board configuration.\n"
	  "\t// Eg: Board=pathToFile.abc\n\n"
	  
	  "\t// \"BoardProbability\"\n"
	  "\t// Accuracy to detect board.\n"
	  "\t// Eg: BoardProbability=0.3\n\n"
	  
	  "\t// \"MarkerSize\"\n"
	  "\t// Size of the marker's sides (expressed in meters).\n"
	  "\t// Eg: MarkerSize=0.05\n\n\n"

	  "[AROGRE CONFIGURATION]\n"
	  "InputVideo=LIVE\n"
	  "IdDevice=0\n"
	  "SetCaptureSize=On\n"
	  "CaptureHeight=480\n"
	  "CaptureWidth=640\n"
	  "LoopVideo=On\n"
	  "CameraCalibration=./media/aruco/camera.yml\n"
	  "BoardInfo=./media/aruco/boardinfo.abc\n"
	  "BoardProbability=0.3\n"
	  "MarkerSize=0.05\n";
    file.close();
  }
  else
  {
    Ogre::LogManager::getSingletonPtr()->logMessage("[ArConfig] Error creating the file arogre.cfg");
    return false;
  }
  
  return true;
} 
} // namespace ArOgre