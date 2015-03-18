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

#ifndef _ARCAPTURER_H__
#define _ARCAPTURER_H__

#include "PCH.h"

namespace ArOgre
{
/** Load a input stream video and capture images from it.
 * 
 *  The grabber loading a input stream, and get pictures of it. Later 
 *  these images are displayed by the ArBackground. Must be updated every 
 *  frame to capture new images.
 * 
 *  The grabber can get images from two different input sources: from a 
 *  video camera or webcam, or from a video file.
*/
class ArCapturer
{
public:
  /** Standard constructor.
   *  @param
   * 	camParams The aruco::CameraParameters from ArCamera.
  */
  ArCapturer( aruco::CameraParameters camParams );
  
  /** Standard destructor.
  */
  ~ArCapturer();

  /** Load input stream format.
   *  @param
   * 	pathCapture the input stream. If the video source is a camera, the 
   * 	value of this string should be "LIVE". In contrast, if the video 
   * 	source is a file, the value of this string should be the path to 
   * 	that file.
   *  @param
   * 	idDevice is the number of the used camera. If you have more than one 
   * 	camera connected, you must indicate the number here.
  */
  bool loadStream( const Ogre::String& pathCapture, unsigned int idDevice,
    bool captureSize, unsigned int captureHeight, unsigned int captureWidth,
    bool loop);
  
  /** Capture a new picture from stream. 
   *  Returns true if the capture was successful.
  */
  bool capture();
  
  /** Returns the input captured image (cv::Mat).
  */
  cv::Mat getCapturedImage() 
  { 
    return mInputImageUnd; 
  }
  
  /** Returns the width captured image.
  */
  unsigned int getCaptureWidth() 
  {
    return mInputImage.cols;
  }
  
  /** Returns the height captured image.
  */
  unsigned int getCaptureHeight()
  {
    return mInputImage.rows;
  }
  
  /** Sets the ArUco camera parameters.
  */
  void setCameraParams( aruco::CameraParameters camParams )
  {
    mCameraParams=camParams;
  }
  
  
private:
  /// ArUco camera parameters
  aruco::CameraParameters mCameraParams;
  bool mLoopVideo;
  
  /// OpenCV video capturer
  cv::VideoCapture mVideoCapturer;
  
  /// OpenCV image and final image (undistort)
  cv::Mat mInputImage, mInputImageUnd;
};
} // namespace ArOgre

#endif // define _ARCAPTURER_H__
