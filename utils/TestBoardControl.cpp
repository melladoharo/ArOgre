#include "PCH.h"
#include "TestBoardControl.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR
//////////////////////////////////////////////////////////////////////////////////////////////
TestBoardControl::TestBoardControl()
  : mAirplane(0),
    mArOcean(0),
    mGameOver(false)
{
  srand(time(NULL));
}
//////////////////////////////////////////////////////////////////////////////////////////////
TestBoardControl::~TestBoardControl()
{
  if(mAirplane) delete mAirplane;
  if(mArOcean) delete mArOcean;
}




//////////////////////////////////////////////////////////////////////////////////////////////
// CREATE SCENE
//////////////////////////////////////////////////////////////////////////////////////////////
void TestBoardControl::createScene()
{
  // create plane, diferent plane for diferent model
  mPlane=new Ogre::Plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane("groundPlane",	
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
    (*mPlane),
    1, 1, 20, 20, true, 1, 1.0f, 1.0f, 
    Ogre::Vector3::UNIT_Z);
  
  // create entity plane 
  mGround=new ArOgre::ArModel(mSceneMgr, "ground", "ground1", "groundPlane", -1, 0.05);
  mGround->getModelNode()->scale(9,0,6.25f);
  mGround->getModelNode()->yaw(Ogre::Degree(180.0f));
  mGround->getEntity()->setMaterialName("mat_Start");
}




//////////////////////////////////////////////////////////////////////////////////////////////
// FRAME LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
void TestBoardControl::createFrameListener()
{
  ArBaseApplication::createFrameListener();
  mTrayMgr->showCursor();
  
  // Create main menu
  if(!mShowWarning)
    createInfoMenu();
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestBoardControl::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  bool ret=ArBaseApplication::frameRenderingQueued(evt);
  
  if(mAirplane)
  {
    bool detected;
    Ogre::Vector3 newPos; 
    Ogre::Quaternion newQua;
    
    if(!mGameOver)
    {
      detected=mDetector->detectBoard(newPos, newQua);
      mAirplane->update(detected, newPos, newQua);
      mPropeller->roll(Ogre::Degree(20));
      
      updateFuel();
      if(pbFuel->getProgress()<=0) // END GAME
      {
	for(unsigned int i=0; i<vFuel.size(); ++i)
	{
	  vFuel[i].node->setVisible(false);
	}
	mGameOver=true;
	mTrayMgr->destroyAllWidgets();
	createEndMenu();
      }
      else
      {
	pbFuel->setProgress(pbFuel->getProgress()-0.001f);
	pbFuel->setComment(Ogre::StringConverter::toString(int(pbFuel->getProgress()*100))+" %");
      } 
    }
  }
  else
  {
    bool groundDetected;
    Ogre::Vector3 newPosGround; 
    Ogre::Quaternion newQuaGround;
    
    groundDetected=mDetector->detectBoard(newPosGround, newQuaGround);
    mGround->update(groundDetected, newPosGround, newQuaGround);
  }
  
  return ret;
}




//////////////////////////////////////////////////////////////////////////////////////////////
// KEY LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestBoardControl::keyPressed ( const OIS::KeyEvent& arg )
{
  if(arg.key==OIS::KC_G)
  {
    mBackground->setVisible(false);
  }
  else if(arg.key==OIS::KC_W)
  {
    mAirplane->getModelNode()->translate(0,0.01,0);
  }
  else if(arg.key==OIS::KC_S)
  {
    mAirplane->getModelNode()->translate(0,-0.01,0);
  }
  else if(arg.key==OIS::KC_A)
  {
    mAirplane->getModelNode()->translate(-0.01,0,0);
  }
  else if(arg.key==OIS::KC_D)
  {
    mAirplane->getModelNode()->translate(0.01,0,0);
  }
  
  return ArBaseApplication::keyPressed ( arg );
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestBoardControl::keyReleased ( const OIS::KeyEvent& arg )
{
  return ArBaseApplication::keyReleased ( arg );
}




//////////////////////////////////////////////////////////////////////////////////////////////
// MOUSE LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestBoardControl::mouseMoved(const OIS::MouseEvent &arg)
{
  return ArBaseApplication::mouseMoved(arg);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestBoardControl::mousePressed(const OIS::MouseEvent &arg,OIS::MouseButtonID id)
{
  return ArBaseApplication::mousePressed(arg, id);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestBoardControl::mouseReleased(const OIS::MouseEvent &arg,OIS::MouseButtonID id)
{
  return ArBaseApplication::mouseReleased(arg, id);
}




//////////////////////////////////////////////////////////////////////////////////////////////
// SDKTRAY LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
void TestBoardControl::buttonHit ( OgreBites::Button* button )
{
  if(button->getName()=="bttWarning")
  {
    mTrayMgr->destroyAllWidgets();
    createMainMenu();
  }
  else if(button->getName()=="bttExit")
  {
    mShutDown=true;
  }
  else if(button->getName()=="bttInfoExit")
  {
    Ogre::Vector3 newPos; 
    Ogre::Quaternion newQua;
    if(mDetector->detectBoard(newPos, newQua))
    {
      mTrayMgr->destroyAllWidgets();
      createMainScene();
      createMainMenu();
      createFuel();
      mArOcean->update(true, newPos, newQua);
      mArOcean->getModelNode()->setVisible(false);
      mOceanSurfaceNode->setVisible(true);
      mGround->getModelNode()->setVisible(false);
    }
  }
  else if(button->getName()=="bttReset")
  {
    mTrayMgr->destroyAllWidgets();
    createMainMenu();
    mGameOver=false;
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void TestBoardControl::createInfoMenu()
{
  OgreBites::TextBox* tb=mTrayMgr->createTextBox(
    OgreBites::TL_CENTER, "tbInfo", "Test Board Control", 500, 300);
  tb->setText("CONTROL THE AIRPLANE\n\n\n"
    "_-> Before starts, move the board to see the message correctly..\n"
    "_-> Use the board to control the airplane.\n"
    "_-> Press OK to start the game and pick up the gas cans.\n"
    "_-> Do not let the fuel rod reaches zero.\n"
    "_-> During the game, it is forbidden to move the camera."
  );
  mTrayMgr->createButton(OgreBites::TL_CENTER, "bttInfoExit", "Accept", 200);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void TestBoardControl::createMainMenu()
{
  Ogre::Real tamWidget=200.0f;
  pbFuel=mTrayMgr->createProgressBar(OgreBites::TL_BOTTOM, "pbFuel", "Fuel", tamWidget*2, 60);
  pbFuel->setProgress(100);
  pbFuel->setComment(Ogre::StringConverter::toString(int(pbFuel->getProgress()*100))+" %");
}
//////////////////////////////////////////////////////////////////////////////////////////////
void TestBoardControl::createEndMenu()
{
  mTrayMgr->createLabel(OgreBites::TL_CENTER, "lblEnd", "GAME OVER", 180);
  mTrayMgr->createSeparator(OgreBites::TL_CENTER, "sptEnt", 180);
  mTrayMgr->createButton(OgreBites::TL_CENTER, "bttReset", "Restart", 180);
  mTrayMgr->createButton(OgreBites::TL_CENTER, "bttExit", "Exit", 180);
}






void TestBoardControl::createMainScene()
{
   // airplane: base + propeller
  Ogre::Real mSize=mConfig->getMarkerSize();
  mAirplane=new ArOgre::ArModel(mSceneMgr, "arPlane", "mPlane", "mesh_airplaneB.mesh", -1, mSize);
  mAirplane->getModelNode()->scale(2,2,2);
  mAirplane->getModelNode()->translate(0.0f, 0.025f, 0.0f);
  Ogre::Entity* entPro=mSceneMgr->createEntity("mesh_airplaneP.mesh");
  mPropeller=mAirplane->getModelNode()->createChildSceneNode();
  mPropeller->attachObject(entPro);
  mPropeller->translate(0.0f, 2.3f, -6.7f);
  
  // particle to airplane
  Ogre::ParticleSystem::setDefaultNonVisibleUpdateTimeout(5);  // set nonvisible timeout
  Ogre::ParticleSystem* ps;
  Ogre::SceneNode* nodePS;
  Ogre::Vector3 sclPS(0.05f, 0.05f, 0.05f);
  
  ps = mSceneMgr->createParticleSystem("Smoke", "Examples/JetEngine1");
  nodePS=mAirplane->getModelNode()->createChildSceneNode();
  nodePS->attachObject(ps);
  nodePS->scale(sclPS);
  nodePS->translate(0,0,5);
  nodePS->pitch(Ogre::Degree(90));
  
  // ocean
  mArOcean=new ArOgre::ArModel(mSceneMgr, "ArOcean", "ocean", "mesh_cube.mesh", -1, mSize);
  mArOcean->getModelNode()->scale(3,3,3);
  
  // define a plane mesh that will be used for the ocean surface
  Ogre::Plane oceanSurface;
  oceanSurface.normal = Ogre::Vector3::UNIT_Y;
  oceanSurface.d = 20;
  Ogre::MeshManager::getSingleton().createPlane("OceanSurface",
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    oceanSurface,
    2000, 2000, 50, 50, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);
  
  // ocean
  mOceanSurfaceEnt=mSceneMgr->createEntity("OceanSurface", "OceanSurface");
  mOceanSurfaceNode=mArOcean->getModelNode()->createChildSceneNode();
  mOceanSurfaceNode->attachObject(mOceanSurfaceEnt);
  mOceanSurfaceEnt->setMaterialName("Ocean2_HLSL_GLSL");
  mOceanSurfaceNode->scale(0.1f, 0.1f, 0.1f);
  mOceanSurfaceNode->translate(0.0f, -0.5f, 0.0f);
  
  // setup scene light
  mSceneMgr->setAmbientLight(Ogre::ColourValue(0.75f, 0.75f, 0.75f));
  mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
  mSceneMgr->setShadowColour(Ogre::ColourValue(0.87f, 0.87f, 0.87f));
  
  // point light
  Ogre::Light* pointLight = mSceneMgr->createLight("pointLight");
  pointLight->setType(Ogre::Light::LT_POINT);
  pointLight->setPosition(Ogre::Vector3(10.0f, 100.0f, 10.0f));
  pointLight->setDiffuseColour(Ogre::ColourValue(0.94f, 0.94f, 0.94f));
  Ogre::SceneNode *nodeLight=mArOcean->getMarkerNode()->createChildSceneNode();
  nodeLight->attachObject(pointLight);
  
  // fuel 
  //createFuel();
  
  // skybox
  mSceneMgr->setSkyBox(true, "Examples/EarlyMorningSkyBox", 10);
}


void TestBoardControl::createFuel()
{
  for(unsigned int i=0; i<5; ++i)
  {
    vFuel.push_back(sFuel());
    vFuel.back().entidad=mSceneMgr->createEntity("mesh_canister.mesh");
    vFuel.back().node=mArOcean->getModelNode()->createChildSceneNode();
    vFuel.back().node->attachObject(vFuel.back().entidad);
    vFuel.back().node->scale(0.03f, 0.03f, 0.03f);    
    
    randomFuel(vFuel.back());
    
    Ogre::ParticleSystem* ps;
    Ogre::SceneNode* nodePS;
    Ogre::Vector3 sclPS(0.05f, 0.05f, 0.05f);
  
    Ogre::String nFuel=Ogre::StringConverter::toString(i);
    ps = mSceneMgr->createParticleSystem("fuelAureola_"+nFuel, "Examples/Aureola");
    nodePS=vFuel.back().node->createChildSceneNode();
    nodePS->attachObject(ps);
    nodePS->scale(sclPS);
  }
}

void TestBoardControl::updateFuel()
{
  for(unsigned int i=0; i<vFuel.size(); ++i)
  {
    vFuel[i].node->setVisible(true);
    vFuel[i].node->translate(0,0,vFuel[i].speed);
    
    // Collision
    mAirplane->getModelNode()->_update(true, true);
    vFuel[i].node->_update(true, true);
    if(mAirplane->getModelNode()->_getWorldAABB().intersects(vFuel[i].node->_getWorldAABB()))
    {
      pbFuel->setProgress(pbFuel->getProgress()+0.15f);
      pbFuel->setComment(Ogre::StringConverter::toString(int(pbFuel->getProgress()*100))+" %");
      randomFuel(vFuel[i]);
    }
    
    // Reset position
    if(vFuel[i].node->getPosition().z>5)
    {
      randomFuel(vFuel[i]);
    }
  }
}


void TestBoardControl::randomFuel(sFuel& f)
{
  f.speed=Ogre::Math::RangeRandom(0.009f, 0.040f);
  
  Ogre::Real xPos=Ogre::Math::RangeRandom(-2.6f, 2.6f);
  Ogre::Real yPos=Ogre::Math::RangeRandom(0.5f, 2.5f);
  Ogre::Real zPos=Ogre::Math::RangeRandom(-6.0f, -10.0f);
  f.node->setPosition(0,0,0);
  f.node->translate(xPos, yPos, zPos);
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
        TestBoardControl app;

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

