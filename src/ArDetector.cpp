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
#include "ArDetector.h"

namespace ArOgre
{ 
ArDetector::ArDetector( aruco::CameraParameters camParams )
  : mCameraParams(camParams),
    mBoardProbability(0),
    mMaxProbability(0.3f),
    mMarkerSize(0.05f)
{
  Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing ArDetector ***");
}
//////////////////////////////////////////////////////////////////////////////////////////////
ArDetector::~ArDetector()
{
  Ogre::LogManager::getSingleton().logMessage("*-*-*  ArDetector Removed ");
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool ArDetector::loadBoardConfig ( const Ogre::String& pathBoardConfig, Ogre::Real bProbability )
{
  mMaxProbability=bProbability;
  
  try
  {
    mBoardConfig.readFromFile(pathBoardConfig);
    Ogre::LogManager::getSingleton().logMessage("[ArDetector] config file loaded succesfully.");
    return true;
  }
  catch (std::exception &ex)
  {
    Ogre::LogManager::getSingletonPtr()->logMessage("[ArDetector] "+Ogre::String(ex.what()));
    return false;
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void ArDetector::detect(cv::Mat inputImageUnd, bool detectBoard)
{
  // Detect markers
  mDetector.detect(inputImageUnd, mMarkers, mCameraParams.CameraMatrix, cv::Mat(), mMarkerSize);
  
  // Detect board
  if(detectBoard)
  {
    mBoardProbability=mBoardDetector.detect(
      mMarkers, mBoardConfig, mBoardDetected, mCameraParams, mMarkerSize);
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool ArDetector::detectIdMarker ( int idMarker, Ogre::Vector3& newPos, Ogre::Quaternion& newQua )
{
  // find marker detected and update position rotation
  for (unsigned int i=0;i<mMarkers.size();i++)
  {
    if (mMarkers[i].id==idMarker)
    {
      cv::Mat Tvec=mMarkers[i].Tvec;
      cv::Mat Rvec=mMarkers[i].Rvec;
      
      // new position
      newPos=Ogre::Vector3(-Tvec.at<float>(0, 0), -Tvec.at<float>(1, 0), Tvec.at<float>(2, 0));
      
      // new rotation
      cv::Mat rot(3, 3, CV_32FC1);
      cv::Rodrigues(Rvec, rot);
      Ogre::Vector3 oa_x(-rot.at<float>(0,0), -rot.at<float>(1,0), rot.at<float>(2,0));
      Ogre::Vector3 oa_y(-rot.at<float>(0,1), -rot.at<float>(1,1), rot.at<float>(2,1));
      Ogre::Vector3 oa_z(-rot.at<float>(0,2), -rot.at<float>(1,2), rot.at<float>(2,2));
 
      newQua=Ogre::Quaternion(oa_x, oa_y, oa_z);
      
      return true;
    }
  }
  return false; // no detected id marker
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool ArDetector::detectIdMarker ( int idMarker )
{
  // find if marker is detected
  for (unsigned int i=0;i<mMarkers.size();i++)
    if (mMarkers[i].id==idMarker)
      return true;

  return false;
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool ArDetector::detectBoard ( Ogre::Vector3& newPos, Ogre::Quaternion& newQua )
{
  if(mBoardProbability>mMaxProbability)
  {
    cv::Mat Tvec=mBoardDetected.Tvec;
    cv::Mat Rvec=mBoardDetected.Rvec;
    
    // new position
    newPos=Ogre::Vector3(-Tvec.at<float>(0, 0), -Tvec.at<float>(1, 0), Tvec.at<float>(2, 0));
    
    // new rotation
    cv::Mat rot(3, 3, CV_32FC1);
    cv::Rodrigues(Rvec, rot);
    Ogre::Vector3 oa_x(-rot.at<float>(0,0), -rot.at<float>(1,0), rot.at<float>(2,0));
    Ogre::Vector3 oa_y(-rot.at<float>(0,1), -rot.at<float>(1,1), rot.at<float>(2,1));
    Ogre::Vector3 oa_z(-rot.at<float>(0,2), -rot.at<float>(1,2), rot.at<float>(2,2));

    newQua=Ogre::Quaternion(oa_x, oa_y, oa_z);
    
    return true;
  }
  return false;
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool ArDetector::detectBoard()
{
  if(mBoardProbability>mMaxProbability)
  {
    return true;
  }
  return false;
}
//////////////////////////////////////////////////////////////////////////////////////////////
int ArDetector::getBoardIdMarker ( int row, int col )
{
  if(row>=0 && row<getBoardHeight())
    if(col>=0 && col<getBoardWidth())
      return mBoardConfig._markersId.at<int>(row, col);
    
  Ogre::LogManager::getSingleton().logMessage("[ArDetector] error! The col or row are out of bounds.");
  return -1;
}
//////////////////////////////////////////////////////////////////////////////////////////////
std::vector<int> ArDetector::getBoardIdMarker( )
{
  std::vector<int> idMarkers;
  
  for(int i=0; i<getBoardHeight(); ++i)
    for(int j=0; j<getBoardWidth(); ++j)
      idMarkers.push_back(mBoardConfig._markersId.at<int>(i,j));
    
  return idMarkers;
}
//////////////////////////////////////////////////////////////////////////////////////////////
Ogre::Matrix4 ArDetector::getProjectionMatrix( cv::Mat inputImageUnd, aruco::CameraParameters camParams )
{
  // set the new projection matrix 
  double projMatrix[16];
  cv::Size windowsSize(inputImageUnd.cols, inputImageUnd.rows);
  
  mDetector.glGetProjectionMatrix(camParams, 
				   inputImageUnd.size(), 
				   windowsSize, 
				   projMatrix, 0.05f, 10.0f, false);

  Ogre::Matrix4 matProjection(projMatrix[0], projMatrix[4], projMatrix[8],  projMatrix[12],
			      projMatrix[1], projMatrix[5], projMatrix[9],  projMatrix[13],
			      projMatrix[2], projMatrix[6], projMatrix[10], projMatrix[14],
			      projMatrix[3], projMatrix[7], projMatrix[11], projMatrix[15]);
  
  return matProjection * Ogre::Matrix4::getScale(-1, -1, -1);
}

} // namespace ArOgre

