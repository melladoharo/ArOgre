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

#ifndef _ARBACKGROUND_H__
#define _ARBACKGROUND_H__

#include "PCH.h"

namespace ArOgre
{
/** A 2d background where the scene will rendered.
 * 
 *  The 2d background is used to represent the catch taken by the grabber. The
 *  background must always be drawn. Must be updated every frame to update the
 *  texture drawn.
 *  It is also responsible for drawing the shadows, when they are required. It 
 *  is recommended to associate the shadows with a single node, the node board.
*/
class ArBackground
{
public:
  /** Standard constructor.
   *  @param
   * 	sManager scene manager responsible for the scene.
  */
  ArBackground( Ogre::SceneManager* sManager );
  
  /** Standard destructor.
  */
  ~ArBackground();

  /** Sets the new image that background draws.
   *  @param
   * 	cvImage Captured cv::Mat image that will use as texture 2d background.
  */
  bool setImage(cv::Mat cvImage);
  
  /** Updates the background texture.
   *  @remarks
   * 	Must be called in each frame.
  */
  void update();
  
  /** Creates a plane under the sceneNode where draws the shadow.
   *  @remarks
   * 	It is only possible to create shadows once. It is recommended 
   * 	that it be associated with the detected board scene node.
   *  @param
   * 	nodeMarker Ogre::SceneNode associated with the detected marker.
  */
  void enableShadows(Ogre::SceneNode* nodeMarker);
  
  /** Disables the shadows
  */
  void disableShadows();
  
  /** Returns if shadows are enabled.
  */
  bool getShadowEnabled() 
  {
    return mShowShadows;
  }
  
  /** Enables global ilumination
  */
  void enableGlobalIlumination(Ogre::Camera* cam);
  
  /** Disables the "global ilumination"
  */
  void disableGlobalIlumination();
  
  /** Returns the texture used in background
  */
  Ogre::TexturePtr getTexture()
  {
    return mTextBackground;
  }
  
  /** Sets the background visible or not
  */
  void setVisible(bool visible)
  {
    mNodeBackground->setVisible(visible);
  }
  
private:
  /** Creates a 2d rectangle entity covering the whole screen.
  */
  void _createRect();
  
  /** Conversion between the cv::Image to Ogre::Texture.
  */
  void _createTexture(unsigned int texWidth, unsigned int texHeight);
  
  /** New material with the new texture aply to 2d rectangle.
  */
  void _createMaterial();
    
  /// Scene manager responsible for the scene.
  Ogre::SceneManager* mSceneMgr;
  
  /// OGRE: 2d Background: (texture, node, rect, pixelbox and buffer).
  Ogre::Rectangle2D* mRectBackground;
  Ogre::SceneNode* mNodeBackground;
  Ogre::TexturePtr mTextBackground;
  Ogre::String mTextName;
  Ogre::MaterialPtr mMatBackground;
  Ogre::String mMatName;
  Ogre::PixelBox mPixelBox;
  unsigned char* mBuffer;
  
  /// OGRE: Shadows: 3d plane[plane, entity, node] and material).
  Ogre::Plane* mShadowPlane;
  Ogre::Entity* mEntShadowPlane;
  Ogre::SceneNode* mNodeShadowPlane;
  Ogre::MaterialPtr mMatShadowPlane;
  Ogre::MaterialPtr mMatShadowGlobalLight;
  bool mShowShadows;
};
} // namespace ArOgre

#endif // define ARBACKGROUND_H

