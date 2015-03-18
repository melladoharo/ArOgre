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
#include "ArBaseApplication.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR & DESTRUCTOR
//////////////////////////////////////////////////////////////////////////////////////////////
ArBaseApplication::ArBaseApplication()
  : mRoot(0),
    mSceneMgr(0),
    mWindow(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mShutDown(false),
    mTrayMgr(0),
    mShowWarning(false),
    mInputManager(0),
    mMouse(0),
    mKeyboard(0),
    mCamera(0),
    mCapturer(0),
    mConfig(0),
    mBackground(0),
    mDetector(0)
{
}
//////////////////////////////////////////////////////////////////////////////////////////////
ArBaseApplication::~ArBaseApplication()
{
  // remove ArOgre content
  if(mCamera) delete mCamera;
  if(mCapturer) delete mCapturer;
  if(mConfig) delete mConfig;
  if(mBackground) delete mBackground;
  if(mDetector) delete mDetector;
  if(mTrayMgr) delete mTrayMgr;

  // remove ourself as a Window listener
  Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
  windowClosed(mWindow);
  delete mRoot;
}


//////////////////////////////////////////////////////////////////////////////////////////////
// CONFIGURE
//////////////////////////////////////////////////////////////////////////////////////////////
bool ArBaseApplication::configure()
{
  // Show the configuration dialog and initialise the system
  // You can skip this and use root.restoreConfig() to load configuration
  // settings if you were sure there are valid ones saved in ogre.cfg
  //if(mRoot->restoreConfig())
  if(mRoot->showConfigDialog())
  { 
    // If returned true, user clicked OK so initialise
    mWindow = mRoot->initialise(true, "ArOgre Application");
    
    // Loads arogre configuration file
    mConfig=new ArOgre::ArConfig;
    if(mConfig->loadConfiguration())
      return true;
    
    return false;
  }
  else
  {
    return false;
  }
} 


//////////////////////////////////////////////////////////////////////////////////////////////
// CHOOSE SCENE MANAGER
//////////////////////////////////////////////////////////////////////////////////////////////
void ArBaseApplication::chooseSceneManager()
{
  // get the SceneManager, in this case a generic one
  mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
}


//////////////////////////////////////////////////////////////////////////////////////////////
// CREATE CAMERA
//////////////////////////////////////////////////////////////////////////////////////////////
void ArBaseApplication::createCamera()
{
  // create the new ArCamera
  mCamera=new ArOgre::ArCamera(mSceneMgr, "ArNewCam");
  if(!mCamera->loadConfig(mConfig->getPathCamera()))
  {
    mShutDown=true;
  }
} 


//////////////////////////////////////////////////////////////////////////////////////////////
// CREATE FRAMELISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
void ArBaseApplication::createFrameListener()
{
  Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
  OIS::ParamList pl;
  size_t windowHnd = 0;
  std::ostringstream windowHndStr;

  mWindow->getCustomAttribute("WINDOW", &windowHnd);
  windowHndStr << windowHnd;
  pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

  mInputManager = OIS::InputManager::createInputSystem( pl );

  mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
  mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

  mMouse->setEventCallback(this);
  mKeyboard->setEventCallback(this);

  // set initial mouse position (windows center)
  OIS::MouseState &mutableMouseState = const_cast<OIS::MouseState &>(mMouse->getMouseState());
  mutableMouseState.X.abs = (int)(mWindow->getWidth()/2);
  mutableMouseState.Y.abs = (int)(mWindow->getHeight()/2);

  // set initial mouse clipping size
  windowResized(mWindow);

  // register as a Window listener
  Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

  // set skdtrays options
  mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mMouse, this);

  // checks the screen resolution
  windowsSizeWarning();
  
  // add default framelistener
  mRoot->addFrameListener(this);
}


//////////////////////////////////////////////////////////////////////////////////////////////
// CREATE AROGRE CONTENT
//////////////////////////////////////////////////////////////////////////////////////////////
void ArBaseApplication::createArContent()
{
  // create the new ArCapturer 
  mCapturer=new ArOgre::ArCapturer(mCamera->getCameraParameters());
  if(mCapturer->loadStream(mConfig->getPathVideo(), mConfig->getIdDevice(),
    mConfig->getCaptureSize(), mConfig->getCaptureHeight(), mConfig->getCaptureWidth(),
    mConfig->getLoopVideo())) // load video stream
  {  
    // first, capture an image used as background texture 
    if(mCapturer->capture())
    {
      mBackground=new ArOgre::ArBackground(mSceneMgr);
      mBackground->setImage(mCapturer->getCapturedImage()); 
      mBackground->update();
      
      // create the new ArDetector
      mDetector=new ArOgre::ArDetector(mCamera->getCameraParameters());
      mDetector->setMarkerSize(mConfig->getMarkerSize());
      if(mDetector->loadBoardConfig(mConfig->getPathBoard(), mConfig->getBoardProbability()))
      {
	// set manual projection and view matrix
	mCamera->setProjectionMatrix(mDetector->getProjectionMatrix(mCapturer->getCapturedImage(), mCamera->getCameraParameters()));
	mCamera->setViewMatrix(Ogre::Matrix4::IDENTITY);
      }
      else
      {
	mShutDown=true;
      }
      }
  }
  else
  {
      mShutDown=true;
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////
// DESTROY SCENE
//////////////////////////////////////////////////////////////////////////////////////////////
void ArBaseApplication::destroyScene()
{
}


//////////////////////////////////////////////////////////////////////////////////////////////
// CREATE VIEWPORTS
//////////////////////////////////////////////////////////////////////////////////////////////
void ArBaseApplication::createViewports()
{
  // create one viewport, entire window
  Ogre::Viewport* vp = mWindow->addViewport(mCamera->getOgreCamera());
  vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

  // alter the camera aspect ratio to match the viewport
  mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}


//////////////////////////////////////////////////////////////////////////////////////////////
// SETUP RESOURCES
//////////////////////////////////////////////////////////////////////////////////////////////
void ArBaseApplication::setupResources()
{
  // load resource paths from config file
  Ogre::ConfigFile cf;
  cf.load(mResourcesCfg);

  // go through all sections & settings in the file
  Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

  Ogre::String secName, typeName, archName;
  while (seci.hasMoreElements())
  {
    secName = seci.peekNextKey();
    Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
    Ogre::ConfigFile::SettingsMultiMap::iterator i;
    for (i = settings->begin(); i != settings->end(); ++i)
    {
	typeName = i->first;
	archName = i->second;
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
    }
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////
// CREATE RESOURCELISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
void ArBaseApplication::createResourceListener()
{

}


//////////////////////////////////////////////////////////////////////////////////////////////
// LOAD RESOURCES
//////////////////////////////////////////////////////////////////////////////////////////////
void ArBaseApplication::loadResources()
{
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
  
  // manual load sdktray fonts (Ogre bug?)
  Ogre::FontManager::getSingleton().getByName("SdkTrays/Value")->load();
  Ogre::FontManager::getSingleton().getByName("SdkTrays/Caption")->load();
}


//////////////////////////////////////////////////////////////////////////////////////////////
// GO
//////////////////////////////////////////////////////////////////////////////////////////////
void ArBaseApplication::go()
{
#ifdef _DEBUG
  mResourcesCfg = "resources_d.cfg";
  mPluginsCfg = "plugins_d.cfg";
#else
  mResourcesCfg = "resources.cfg";
  mPluginsCfg = "plugins.cfg";
#endif

  if (!setup())
    return;

  mRoot->startRendering();

  // clean up
  destroyScene();
}


//////////////////////////////////////////////////////////////////////////////////////////////
// setup
//////////////////////////////////////////////////////////////////////////////////////////////
bool ArBaseApplication::setup()
{
  mRoot = new Ogre::Root(mPluginsCfg);

  setupResources();

  bool carryOn = configure();
  if (!carryOn) return false;

  chooseSceneManager();
  createCamera();
  createViewports();
  
  // set default mipmap level (NB some APIs ignore this)
  Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

  // create any resource listeners (for loading screens)
  createResourceListener();
  
  // load resources
  loadResources();
  
  // create the new ArOgre content
  createArContent();
  
  // create the scene
  createScene();

  // create the framelistener
  createFrameListener();
  
  return true;
};


//////////////////////////////////////////////////////////////////////////////////////////////
// FRAME RENDERING QUEUED
//////////////////////////////////////////////////////////////////////////////////////////////
bool ArBaseApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  if(mWindow->isClosed())
    return false;

  if(mShutDown)
    return false;

  // need to capture/update each device
  mKeyboard->capture(); 
  mMouse->capture();

  // update the traymanager
  mTrayMgr->frameRenderingQueued(evt);

  // update ar content (capture image, update background and detect markers)
  if(mCapturer->capture())
  { 
    mBackground->update();
    mDetector->detect(mCapturer->getCapturedImage());  
  }
  
  return true; 
}


//////////////////////////////////////////////////////////////////////////////////////////////
// KEY LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
bool ArBaseApplication::keyPressed( const OIS::KeyEvent &arg )
{
  if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up

  if(arg.key == OIS::KC_F5)   // refresh all textures
  {
    Ogre::TextureManager::getSingleton().reloadAll();
  }
  //else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
  else if (arg.key == OIS::KC_F10)   // take a screenshot
  {
      mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
  }
  else if (arg.key == OIS::KC_ESCAPE)
  {
      mShutDown = true;
  }

  return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool ArBaseApplication::keyReleased( const OIS::KeyEvent &arg )
{
  return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////
// MOUSE LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
bool ArBaseApplication::mouseMoved( const OIS::MouseEvent &arg )
{
  mTrayMgr->injectMouseMove(arg);
  return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool ArBaseApplication::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
  mTrayMgr->injectMouseDown(arg, id);
  return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool ArBaseApplication::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
  mTrayMgr->injectMouseUp(arg, id);
  return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////
// SKDTRAY LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
void ArBaseApplication::buttonHit ( OgreBites::Button* button )
{
  if(button->getName()=="bttWarning")
  {
    mShowWarning=false;
    
    mTrayMgr->destroyWidget(mTrayMgr->getWidget("tbWarning"));
    mTrayMgr->destroyWidget(mTrayMgr->getWidget("bttWarning"));
    mTrayMgr->showCursor();
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////
// WINDOWS RESIZED
//////////////////////////////////////////////////////////////////////////////////////////////
void ArBaseApplication::windowResized(Ogre::RenderWindow* rw)
{
  unsigned int width, height, depth;
  int left, top;
  rw->getMetrics(width, height, depth, left, top);

  const OIS::MouseState &ms = mMouse->getMouseState();
  ms.width = width;
  ms.height = height;
}


//////////////////////////////////////////////////////////////////////////////////////////////
// WINDOWS CLOSED
//////////////////////////////////////////////////////////////////////////////////////////////
void ArBaseApplication::windowClosed(Ogre::RenderWindow* rw)
{
  // only close for window that created OIS (the main window in these demos)
  if(rw == mWindow)
  {
    if(mInputManager)
    {
      mInputManager->destroyInputObject(mMouse);
      mInputManager->destroyInputObject(mKeyboard);

      OIS::InputManager::destroyInputSystem(mInputManager);
      mInputManager = 0;
    }
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////
// WINDOWS SIZE WARNING
//////////////////////////////////////////////////////////////////////////////////////////////
void ArBaseApplication::windowsSizeWarning()
{
  unsigned int windowsWidth, windowsHeight;
  windowsWidth=mRoot->getSingleton().getAutoCreatedWindow()->getWidth();
  windowsHeight=mRoot->getSingleton().getAutoCreatedWindow()->getHeight();
  Ogre::Real arWin=Ogre::Real(windowsWidth)/Ogre::Real(windowsHeight);
  
  unsigned int captureWidth, captureHeight;
  captureWidth=mCapturer->getCaptureWidth();
  captureHeight=mCapturer->getCaptureHeight();
  Ogre::Real arCapture=Ogre::Real(captureWidth)/Ogre::Real(captureHeight);
  
  if(arWin!=arCapture)
  {
    mShowWarning=true;
    
    OgreBites::TextBox* tb=0;
    Ogre::String capWidth=Ogre::StringConverter::toString(captureWidth);
    Ogre::String capHeight=Ogre::StringConverter::toString(captureHeight);
    tb=mTrayMgr->createTextBox(OgreBites::TL_CENTER, "tbWarning", "Warning", 500, 300);
    tb->setText(
      " - _ WARNING _ -\n\n\n"
      "_-> The screen resolution you have chosen can cause visual errors."
      " The input stream has a size of "+capWidth+" wide by "+capHeight+" high.\n\n"
      "_-> Select the screen resolution that is closest.");
    mTrayMgr->createButton(OgreBites::TL_CENTER, "bttWarning", "Accept", 180.0f);
  }
}

