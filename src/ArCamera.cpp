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
#include "ArCamera.h"

namespace ArOgre
{
ArCamera::ArCamera ( Ogre::SceneManager* sManager, const Ogre::String& nameCamera )
  : mSceneMgr(sManager),
    mCamera(0),
    mPosition(Ogre::Vector3(0,0,0)),
    mOrientation(Ogre::Quaternion(1,0,0,0)),
    mLookAt(Ogre::Vector3(0,0,-1)),
    mCamName(nameCamera)
{
  Ogre::LogManager::getSingleton().logMessage("*** Initializing ArCamera ***");
  
  // Create the new Ogre::camera
  mCamera=mSceneMgr->createCamera ( mCamName );
  mCamera->setPosition(mPosition);
  mCamera->lookAt(mLookAt);
  mCamera->setOrientation ( mOrientation );
  mCamera->setNearClipDistance(0.01f);
  mCamera->setFarClipDistance(10.0f);
}
//////////////////////////////////////////////////////////////////////////////////////////////
ArCamera::~ArCamera( )
{
  if(mSceneMgr->getCamera(mCamName))
    mSceneMgr->destroyCamera(mCamera);
  Ogre::LogManager::getSingleton().logMessage("*-*-*  ArCamera Removed ");
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool ArCamera::loadConfig ( const Ogre::String& pathConfig )
{
  // load info camera
  try
  {
    if (pathConfig.substr(pathConfig.size()-4)==".int")
    {
      mCameraParams.readFromFile(pathConfig);
      Ogre::LogManager::getSingleton().logMessage("[ArCamera] config file loaded succesfully.");
      return true;
    }
    else if (pathConfig.substr(pathConfig.size()-4)==".yml")
    {
      Ogre::LogManager::getSingleton().logMessage("[ArCamera] config file loaded succesfully.");
      mCameraParams.readFromXMLFile(pathConfig);
      return true;
    }
    return false;
  }
  catch (std::exception &ex)
  {
    Ogre::LogManager::getSingletonPtr()->logMessage("[ArCamera] "+Ogre::String(ex.what()));
    return false;
  }
}

}


