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

#ifndef _ARBASEAPPLICATION_H__
#define _ARBASEAPPLICATION_H__

#include "PCH.h"
#include "ArCamera.h"
#include "ArCapturer.h"
#include "ArConfig.h"
#include "ArBackground.h"
#include "ArDetector.h"
#include "ArModel.h"

/** Base class for developing applications with Ogre and Ogre.
*/
class ArBaseApplication : public Ogre::FrameListener, public Ogre::WindowEventListener,
  public OIS::KeyListener, public OIS::MouseListener, OgreBites::SdkTrayListener
{
public:
  ArBaseApplication();
  virtual ~ArBaseApplication();

  virtual void go();
  

protected:
  /// Ogre setup
  virtual bool setup();
  virtual bool configure();
  virtual void chooseSceneManager();
  virtual void createCamera();
  virtual void createFrameListener();
  virtual void createArContent();
  virtual void createScene()=0; // Override me!
  virtual void destroyScene();
  virtual void createViewports();
  virtual void setupResources();
  virtual void createResourceListener();
  virtual void loadResources();

  /// Ogre::FrameListener
  virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

  /// OIS::KeyListener
  virtual bool keyPressed(const OIS::KeyEvent &arg);
  virtual bool keyReleased(const OIS::KeyEvent &arg);
    
  /// OIS::MouseListener
  virtual bool mouseMoved(const OIS::MouseEvent &arg);
  virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
  virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

  /// Ogre::WindowEventListener
  virtual void windowResized(Ogre::RenderWindow* rw);
  virtual void windowClosed(Ogre::RenderWindow* rw);

  /// Sdktray Listener
  virtual void buttonHit(OgreBites::Button* button);
  
  /// ArOgre
  virtual void windowsSizeWarning();

  /// Ogre
  Ogre::Root *mRoot;
  Ogre::SceneManager* mSceneMgr;
  Ogre::RenderWindow* mWindow;
  Ogre::String mResourcesCfg;
  Ogre::String mPluginsCfg;
  bool mShutDown;

  /// OgreBites
  OgreBites::SdkTrayManager* mTrayMgr;
  bool mShowWarning;

  /// OIS Input devices
  OIS::InputManager* mInputManager;
  OIS::Mouse* mMouse;
  OIS::Keyboard* mKeyboard;

  /// ArOgre
  ArOgre::ArCamera* mCamera;
  ArOgre::ArCapturer* mCapturer;
  ArOgre::ArConfig* mConfig;
  ArOgre::ArBackground* mBackground;
  ArOgre::ArDetector* mDetector;
};

#endif // #ifndef _ARBASEAPPLICATION_H__
