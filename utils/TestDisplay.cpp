#include "PCH.h"
#include "TestDisplay.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR
//////////////////////////////////////////////////////////////////////////////////////////////
TestDisplay::TestDisplay()
  : mLight(0), mModel(0), mNextAction(0), mPreviousAction(0),
    mTimeActionNext(0), mTimeActionPrev(0), mTimeChange(0),
    mCurrentAcion(DROTATE), mCurrentColor(0), mCurrentModel(0)
{
}
//////////////////////////////////////////////////////////////////////////////////////////////
TestDisplay::~TestDisplay()
{
  if(mLight) delete mLight;
  if(mModel) delete mModel;
  if(mNextAction) delete mNextAction;
  if(mPreviousAction) delete mPreviousAction;
}




//////////////////////////////////////////////////////////////////////////////////////////////
// CREATE SCENE
//////////////////////////////////////////////////////////////////////////////////////////////
void TestDisplay::createScene(void)
{
 
}




//////////////////////////////////////////////////////////////////////////////////////////////
// FRAME LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
void TestDisplay::createFrameListener()
{
  ArBaseApplication::createFrameListener();
  mTrayMgr->showCursor();
  
  // create main menu
  if(!mShowWarning)
    createInfoMenu();
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestDisplay::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  bool ret=ArBaseApplication::frameRenderingQueued(evt);
  
  if(mModel)
  {
    bool detected;
    Ogre::Vector3 newPos; 
    Ogre::Quaternion newQua;
    
    detected=mDetector->detectBoard(newPos, newQua);
    mModel->update(detected, newPos, newQua);
    mLight->update(detected, newPos, newQua); 
    
    // Update action Up
    detected=mDetector->detectIdMarker(mNextAction->getIdMarker(), newPos, newQua);
    mNextAction->update(detected, newPos, newQua);
    if(detected)
    {
      mTimeActionNext=0;
    }
    else
    {
      mTimeActionNext+=evt.timeSinceLastFrame;
      if(mTimeActionNext>=1.5f)
      {
	mTimeActionPrev=0;
	performAction(true);
      }
    }
    // Update action Down
    detected=mDetector->detectIdMarker(mPreviousAction->getIdMarker(), newPos, newQua);
    mPreviousAction->update(detected, newPos, newQua);
    if(detected)
    {
      mTimeActionPrev=0;
    }
    else
    {
      mTimeActionPrev+=evt.timeSinceLastFrame;
      if(mTimeActionPrev>=1.5f)
      {
	mTimeActionNext=0;
	performAction(false);
      }
    }
    
    // change actions 
    unsigned int nAction=0;
    for(unsigned int i=0; i<vIdChange.size(); ++i)
    {
      detected=mDetector->detectIdMarker(vIdChange[i], newPos, newQua);
      if(!detected)
	nAction++;
    }
    if(nAction==4)
    {
      mTimeChange+=evt.timeSinceLastFrame;
      if(mTimeChange>=2.0f)
      {
	mTimeChange=0;
	mCurrentAcion=(mCurrentAcion!=DSELECT) ? mCurrentAcion+1 : 0;
	mTrayMgr->destroyAllWidgets();
	createMainMenu();
      }
    }
  }
  
  return ret;
}




//////////////////////////////////////////////////////////////////////////////////////////////
// KEY LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestDisplay::keyPressed ( const OIS::KeyEvent& arg )
{
  if(arg.key==OIS::KC_F1)
  {
    createInfoMenu();
  }
  else if(arg.key==OIS::KC_SPACE)
  {
    mCurrentAcion=(mCurrentAcion!=DSELECT) ? mCurrentAcion+1 : 0;
    mTrayMgr->destroyAllWidgets();
    createMainMenu();
  }
  return ArBaseApplication::keyPressed ( arg );
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestDisplay::keyReleased ( const OIS::KeyEvent& arg )
{
  return ArBaseApplication::keyReleased ( arg );
}




//////////////////////////////////////////////////////////////////////////////////////////////
// MOUSE LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestDisplay::mouseMoved(const OIS::MouseEvent &arg)
{
  return ArBaseApplication::mouseMoved(arg);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestDisplay::mousePressed(const OIS::MouseEvent &arg,OIS::MouseButtonID id)
{
return ArBaseApplication::mousePressed(arg, id);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestDisplay::mouseReleased(const OIS::MouseEvent &arg,OIS::MouseButtonID id)
{
return ArBaseApplication::mouseReleased(arg, id);
}




//////////////////////////////////////////////////////////////////////////////////////////////
// SDKTRAY LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
void TestDisplay::buttonHit ( OgreBites::Button* button )
{
  if(button->getName()=="bttWarning")
  {
    mTrayMgr->destroyAllWidgets();
    createInfoMenu();
  }
  else if(button->getName()=="bttInfoExit")
  {
    mTrayMgr->destroyAllWidgets();
    createMainMenu();
    createDisplay();
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void TestDisplay::createInfoMenu()
{
  OgreBites::TextBox* tb=mTrayMgr->createTextBox(
    OgreBites::TL_CENTER, "tbInfo", "Test Display", 500, 300);
  tb->setText("HIDE THE MARKERS\n\n\n"
    "_-> View all the models.\n"
    "_-> Hide the red and green buttons to perform the action.\n"
    "_-> To select another action, put your palm on the center of the board.\n"
    "_-> Press F1 to see this help again.\n"
  );
  mTrayMgr->createButton(OgreBites::TL_CENTER, "bttInfoExit", "Accept", 200);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void TestDisplay::createMainMenu()
{
  Ogre::String action;
  if(mCurrentAcion==DCOLOR) action="CHANGE COLOR";
  else if(mCurrentAcion==DROTATE) action="ROTATE";
  else if(mCurrentAcion==DSCALE) action="SCALE";
  else action="CHANGE MODEL";
  
  mTrayMgr->createLabel(OgreBites::TL_TOP, "lblAction", action, 200.0f);
}


void TestDisplay::createDisplay()
{
  Ogre::Real markerSize=mDetector->getMarkerSize();
  
  // action next
  int idMarker=mDetector->getBoardIdMarker(5,0);
  mNextAction=new ArOgre::ArModel(mSceneMgr, "arNext", "next", "mesh_button.mesh", idMarker, markerSize);
  mNextAction->getEntity()->setMaterialName("mat_button_red");
  
  // action previous
  idMarker=mDetector->getBoardIdMarker(0,0);
  mPreviousAction=new ArOgre::ArModel(mSceneMgr, "arPrev", "prev", "mesh_button.mesh", idMarker, markerSize);
  mPreviousAction->getEntity()->setMaterialName("mat_button_green");
  
  vIdChange.push_back(mDetector->getBoardIdMarker(2,1));
  vIdChange.push_back(mDetector->getBoardIdMarker(2,2));
  vIdChange.push_back(mDetector->getBoardIdMarker(3,1));
  vIdChange.push_back(mDetector->getBoardIdMarker(3,2));
  vColors.push_back(Ogre::ColourValue(0.8f, 0.2f, 0.18f, 1.0f));
  vColors.push_back(Ogre::ColourValue(0.2f, 0.7f, 0.2f, 1.0f));
  vColors.push_back(Ogre::ColourValue(0.2f, 0.3f, 0.8f, 1.0f));
  vColors.push_back(Ogre::ColourValue(0.88f, 0.85f, 0.89f, 1.0f));
  vColors.push_back(Ogre::ColourValue(0.1f, 0.1f, 0.1f, 1.0f));
  vColors.push_back(Ogre::ColourValue(0.8f, 0.85f, 0.3f, 1.0f));
  vNameModels.push_back("mesh_sofa_1.mesh");
  vNameModels.push_back("mesh_sofa_2.mesh");
  vNameModels.push_back("mesh_chair_1.mesh");
  
  // model display
  mModel=new ArOgre::ArModel(mSceneMgr, "arSofa1", "sofa1", "mesh_sofa_1.mesh", -1, markerSize);
  mModel->getModelNode()->scale(3,3,3);
  
  // armodel light
  mLight=new ArOgre::ArModel(mSceneMgr, "arLight", "Light", "mesh_sphere_2.mesh", -1, markerSize);
  mLight->getEntity()->setCastShadows(false);
  mLight->getModelNode()->translate(0,100,0);
  
  // setup scene light
  mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
  mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
  mSceneMgr->setShadowColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
  mBackground->enableShadows(mLight->getMarkerNode());
  
  // point light
  Ogre::Light* pointLight = mSceneMgr->createLight("pointLight");
  pointLight->setType(Ogre::Light::LT_POINT);
  pointLight->setPosition(Ogre::Vector3(0,2,0));
  pointLight->setDiffuseColour(Ogre::ColourValue(0.94f, 0.94f, 0.94f));
}



//////////////////////////////////////////////////////////////////////////////////////////////
// PERFORM ACTION
//////////////////////////////////////////////////////////////////////////////////////////////
void TestDisplay::performAction( bool next )
{
  if(mCurrentAcion==DCOLOR)
  {
    if(next) mCurrentColor=(mCurrentColor==vColors.size()-1) ? 0 : mCurrentColor+1;
    else mCurrentColor=(mCurrentColor==0) ? vColors.size()-1 : mCurrentColor-1;
    
    Ogre::MaterialPtr matPtr=mModel->getEntity()->getSubEntity(0)->getMaterial();
    matPtr->getTechnique(0)->getPass(0)->setAmbient(vColors[mCurrentColor]);
    matPtr->getTechnique(0)->getPass(0)->setDiffuse(vColors[mCurrentColor]);
    
    mTimeActionNext=mTimeActionPrev=0;
  }
  else if(mCurrentAcion==DROTATE)
  {
    Ogre::Real rot=(next) ? 1.0f : -1.0f;
    mModel->getModelNode()->yaw(Ogre::Degree(rot));
  }
  else if(mCurrentAcion==DSCALE)
  {
    Ogre::Real scl=(next) ? 1.01f : 0.99f;
    mModel->getModelNode()->scale(Ogre::Vector3(scl, scl, scl));
  }
  else if(mCurrentAcion==DSELECT)
  {
    if(mModel) delete mModel;
    
    if(next) mCurrentModel=(mCurrentModel==vNameModels.size()-1) ? 0 : mCurrentModel+1;
    else mCurrentModel=(mCurrentModel==0) ? vNameModels.size()-1 : mCurrentModel-1;
    
    mModel=new ArOgre::ArModel(
      mSceneMgr, "arNewModel", "model", vNameModels[mCurrentModel], -1, mDetector->getMarkerSize());
    mModel->getModelNode()->scale(3,3,3);
    
    mTimeActionNext=mTimeActionPrev=0;
  }
}



#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
  INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
  int main(int argc, char *argv[])
#endif
  {
      // Create application object
      TestDisplay app;

      try
      {
	  app.go();
      }
      catch( Ogre::Exception& e )
      {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	  MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
	  std::cerr << "An exception has occured: " <<
		    e.getFullDescription().c_str() << std::endl;
#endif
      }

      return 0;
  }

#ifdef __cplusplus
}
#endif

