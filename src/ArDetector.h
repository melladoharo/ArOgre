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

#ifndef _ARDETECTOR_H__
#define _ARDETECTOR_H__

#include "PCH.h"

namespace ArOgre
{
/** Detect markers and boards in an input image.
 * 
 *  
*/
class ArDetector
{
public:
  /** Standard constructor.
   *  @param
   * 	camParams the aruco::CameraParameters.
  */
  ArDetector(aruco::CameraParameters camParams);
  
  /** Standard destructor.
  */
  ~ArDetector();

  /** Loads the configuration file board.
   *  @param
   * 	pathBoardConfig tha path of the board config file.
   * 	bProbability the max probability to detect board.
  */
  bool loadBoardConfig(const Ogre::String& pathBoardConfig, Ogre::Real bProbability);
  
  /** Detects markers in the passed image.
   *  @param
   * 	inputImageUnd the final image where detects markers and board.
   * 	detectBoard indicates whether the board should be detected.
   *  @remarks
   * 	The detected markers are returned in a vector. 
   * 	The board detection returns a probability of detection.
  */
  void detect(cv::Mat inputImageUnd, bool detectBoard=true);
  
  /** Checks if the marker (id Marker) has been detected.
   *  @param
   * 	idMarker the identifier of the marker to be detected.
   *  @param
   * 	newPos the new position of the detected marker (idMarker).
   *  @param
   * 	newQua the new orientation of the detected marker (idMarker).
  */ 
  bool detectIdMarker(int idMarker, Ogre::Vector3& newPos, Ogre::Quaternion& newQua);
  
  /** Checks if the marker (id Marker) has been detected.
   *  @param
   * 	idMarker the identifier of the marker to be detected.
  */
  bool detectIdMarker(int idMarker);
  
  /** Checks if the board has been detected.
   *  @param
   * 	newPos the new position of the detected marker (idMarker).
   *  @param
   * 	newQua the new orientation of the detected marker (idMarker).
  */
  bool detectBoard(Ogre::Vector3& newPos, Ogre::Quaternion& newQua);
  
  /** Checks if the board has been detected.
  */
  bool detectBoard();
  
  /** Returns the identifier of the marker in the position passed.
   *  @remarks
   * 	row=0 and col=0 is the first marker.
   */
  int getBoardIdMarker( int row, int col );
  
  /** Returns all the identifiers markers of the board.
   */
  std::vector<int> getBoardIdMarker();
  
  /** Returns the projection matrix.
  */
  Ogre::Matrix4 getProjectionMatrix(cv::Mat inputImageUnd, aruco::CameraParameters camParams);
  
  /** Set the marker size to detector.
  */
  void setMarkerSize(float size)
  {
    mMarkerSize=size;
  }
  
  /** Returns the markers size.
  */
  float getMarkerSize()
  {
    return mMarkerSize;
  }
  
  /** Returns the numbers of markers detected
  */
  unsigned int getNumDetectedMarkers() 
  {
    return mMarkers.size();
  }
  
  /** Returns the number of markers that board haves vertically.
  */
  int getBoardHeight()
  {
    return mBoardConfig._markersId.rows;
  }
  
  /** Returns the number of markers that board haves horizontally.
  */
  int getBoardWidth()
  {
    return mBoardConfig._markersId.cols;
  }

  
private:
  /// ArUco camera parameters
  aruco::CameraParameters mCameraParams;
  
  /// ArUco detector: 
  aruco::MarkerDetector mDetector;
  
  /// ArUco board, boardDetector and boardConfiguration
  aruco::BoardConfiguration mBoardConfig;
  aruco::BoardDetector mBoardDetector;
  aruco::Board mBoardDetected;
  float mBoardProbability, mMaxProbability;
  
  /// ArUco markers: markerDetector, markers and markers size
  std::vector<aruco::Marker> mMarkers;	// detected markers
  float mMarkerSize;			// size marker
};
} // namespace ArOgre

#endif // _ARDETECTOR_H__
