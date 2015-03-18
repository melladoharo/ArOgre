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
#include "ArBackground.h"

namespace ArOgre
{  
ArBackground::ArBackground( Ogre::SceneManager* sManager )
  : mSceneMgr(sManager),
    mRectBackground(0),
    mNodeBackground(0),
    mTextBackground(0),
    mTextName("text_2d_Background"), 
    mMatBackground(0),
    mMatName("mat_2d_Background"),
    mBuffer(0),
    mShadowPlane(0),
    mEntShadowPlane(0),
    mNodeShadowPlane(0),
    mMatShadowPlane(0),
    mMatShadowGlobalLight(0),
    mShowShadows(false)
{
  mMatBackground.setNull();
  mMatShadowPlane.setNull();
  mMatShadowGlobalLight.setNull();
  Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing ArBackground ***");
  _createRect();
}
//////////////////////////////////////////////////////////////////////////////////////////////
ArBackground::~ArBackground()
{
  // remove texture and materials
  Ogre::TextureManager::getSingleton().unload(mTextName);
  Ogre::TextureManager::getSingleton().remove(mTextName);
  Ogre::MaterialManager::getSingleton().unload(mMatName);
  Ogre::MaterialManager::getSingleton().remove(mMatName);
  
  if(mNodeBackground)
  {
    mNodeBackground->detachAllObjects();
    mSceneMgr->destroySceneNode(mNodeBackground);
  }

  Ogre::LogManager::getSingleton().logMessage("*-*-*  ArBackground Removed ");
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool ArBackground::setImage ( Mat cvImage )
{
  // create the new texture 
  unsigned int texWidth=cvImage.cols, texHeight=cvImage.rows;
  _createTexture(texWidth, texHeight);
  
  mBuffer=cvImage.ptr<unsigned char>(0);
  mPixelBox=Ogre::PixelBox(texWidth, texHeight, 1, Ogre::PF_R8G8B8, mBuffer);
  
  // create the new material
  _createMaterial();
  
  if(mRectBackground && mNodeBackground && !mTextBackground.isNull() && mBuffer)
  {
    Ogre::LogManager::getSingletonPtr()->logMessage("[ArBackground] background created succesfully.");
    return true;
  }
  
  Ogre::LogManager::getSingletonPtr()->logMessage("[ArBackground] error creating the background.");
  return false;
}
//////////////////////////////////////////////////////////////////////////////////////////////
void ArBackground::update()
{
  if (!mTextBackground.isNull())
  {
    // update the capture with the pixelbox
    Ogre::HardwarePixelBufferSharedPtr pixelBuffer=mTextBackground->getBuffer();
    pixelBuffer->blitFromMemory(mPixelBox);
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void ArBackground::enableShadows( Ogre::SceneNode* nodeMarker )
{
  if(!mShowShadows)
  {    
    // create plane, diferent plane for diferent model
    mShadowPlane=new Ogre::Plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane("shadowPlane",
      Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
      (*mShadowPlane),
      10, 10, 20, 20, true, 1, 1.0f, 1.0f, 
      Ogre::Vector3::UNIT_Z);
  
    // create entity plane 
    mEntShadowPlane=mSceneMgr->createEntity("shadowPlane");
    mEntShadowPlane->setCastShadows(false);
    mNodeShadowPlane=nodeMarker->createChildSceneNode();
    mNodeShadowPlane->attachObject(mEntShadowPlane);
    
    // create material shadow ("transparent material" (projective texture) to show shadows)
    mMatShadowPlane=Ogre::MaterialManager::getSingleton().getByName("mat_ARshadow");
    mEntShadowPlane->setMaterial(mMatShadowPlane);
  
    mShowShadows=true;
  }
}
void ArBackground::disableShadows()
{
  mShowShadows=false;
  if(mShadowPlane) delete mShadowPlane;
  mNodeShadowPlane->detachAllObjects();
  mSceneMgr->destroyEntity(mEntShadowPlane->getName());
  mSceneMgr->destroySceneNode(mNodeShadowPlane->getName());
}
//////////////////////////////////////////////////////////////////////////////////////////////
void ArBackground::enableGlobalIlumination ( Ogre::Camera* cam )
{
  // create material for "global ilumination"
  mMatShadowGlobalLight=Ogre::MaterialManager::getSingleton().create(
    "mat_ARShadowsGlobal", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
  mMatShadowGlobalLight->getTechnique(0)->getPass(0)->createTextureUnitState(mTextName);
  Ogre::TextureUnitState* tUnit=0;
  tUnit=mMatShadowGlobalLight->getTechnique(0)->getPass(0)->getTextureUnitState(0);
  tUnit->setProjectiveTexturing(true, cam);
  
  mEntShadowPlane->setMaterial(mMatShadowGlobalLight);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void ArBackground::disableGlobalIlumination()
{
  mEntShadowPlane->setMaterial(mMatShadowPlane);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void ArBackground::_createRect()
{
  // create background rectangle covering the whole screen
  mRectBackground=new Ogre::Rectangle2D(true);
  mRectBackground->setCorners(-1.0, 1.0, 1.0, -1.0);

  // render the background before everything else
  mRectBackground->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);

  // need to set the bounding box to something big
  mRectBackground->setBoundingBox(Ogre::AxisAlignedBox(
    -100000.0f * Ogre::Vector3::UNIT_SCALE,
    100000.0f * Ogre::Vector3::UNIT_SCALE));

  // attach background to the scene
  mNodeBackground=mSceneMgr->getRootSceneNode()->createChildSceneNode();
  mNodeBackground->attachObject(mRectBackground);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void ArBackground::_createTexture( unsigned int texWidth, unsigned int texHeight )
{
  // new capture texture (ogre texture)
  mTextBackground=Ogre::TextureManager::getSingleton().createManual(
    mTextName,
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    Ogre::TEX_TYPE_2D, 
    texWidth, texHeight, 
    0, 
    Ogre::PF_R8G8B8,
    Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void ArBackground::_createMaterial()
{
  // background material with new capture(textCapture -> dynamic texture)
  mMatBackground=Ogre::MaterialManager::getSingleton().create(mMatName,
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

  // background material properties
  mMatBackground->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
  mMatBackground->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
  mMatBackground->getTechnique(0)->getPass(0)->setLightingEnabled(false);
  mMatBackground->getTechnique(0)->getPass(0)->createTextureUnitState(mTextName);

  // apply the new material
  mRectBackground->setMaterial(mMatName);
}
} // namespace ArOgre

