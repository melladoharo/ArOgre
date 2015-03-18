#include "PCH.h"
#include "TestSolarSystem.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR
//////////////////////////////////////////////////////////////////////////////////////////////
TestSolarSystem::TestSolarSystem(void)
  : mSun(0),
    mCamNode(0),
    mBttReset(0), mBttExit(0), mBttHide(0), mBttInfo(0),
    mBttPause(0), mBttSpeedUp(0), mBttSpeedDown(0), mBttExitInfo(0),
    mPauseRot(false)
{ 
  mSpeedRot=1;
}
//////////////////////////////////////////////////////////////////////////////////////////////
TestSolarSystem::~TestSolarSystem(void)
{
  mTrayMgr->destroyAllWidgets();
  
  mBackground->disableShadows();
  if(mCamNode) mCamNode->detachObject(mCamera->getName());
  if(mSun) delete mSun;
}




//////////////////////////////////////////////////////////////////////////////////////////////
// CREATE SCENE
//////////////////////////////////////////////////////////////////////////////////////////////
void TestSolarSystem::createScene(void)
{
  // create solar system
  createSolarSystem();
  
  // create particles effects
  createParticles();
  
  // setup lights scene
  createLights();
  
  // show background shadows
  mBackground->enableShadows(mSun->getMarkerNode());
}




//////////////////////////////////////////////////////////////////////////////////////////////
// FRAME LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
void TestSolarSystem::createFrameListener()
{
  ArBaseApplication::createFrameListener();
  mTrayMgr->hideFrameStats();
  mTrayMgr->showCursor();
 
  // create the main menu
  if(!mShowWarning)
    createMainMenu();
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestSolarSystem::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  bool ret=ArBaseApplication::frameRenderingQueued(evt);
  
  // update AR Sun
  bool detected;
  Ogre::Vector3 newPos; 
  Ogre::Quaternion newQua;
  
  detected=mDetector->detectBoard(newPos, newQua);
  mSun->update(detected, newPos, newQua);
  
  // rotation planets around the sun
  if(!mPauseRot)
  {
    // "effects" particles 
    mSun->getModelNode()->yaw(Ogre::Degree(0.05f)*mSpeedRot);
    
    mMercury.rot->yaw(Ogre::Degree(1)*mMercury.speed*mSpeedRot);
    mMercury.node->yaw(Ogre::Degree(40));
    mVenus.rot->yaw(Ogre::Degree(1)*mVenus.speed*mSpeedRot);
    mVenus.node->yaw(Ogre::Degree(40));
    mEarth.rot->yaw(Ogre::Degree(1)*mEarth.speed*mSpeedRot);
    mEarth.node->yaw(Ogre::Degree(20));
    mMars.rot->yaw(Ogre::Degree(1)*mMars.speed*mSpeedRot);
    mMars.node->yaw(Ogre::Degree(20));
    mJupiter.rot->yaw(Ogre::Degree(1)*mJupiter.speed*mSpeedRot);
    mJupiter.node->yaw(Ogre::Degree(18));
    mSaturn.rot->yaw(Ogre::Degree(1)*mSaturn.speed*mSpeedRot);
    mSaturn.node->yaw(Ogre::Degree(17));
    mUranus.rot->yaw(Ogre::Degree(1)*mUranus.speed*mSpeedRot);
    mUranus.node->yaw(Ogre::Degree(22));
    mNeptune.rot->yaw(Ogre::Degree(1)*mNeptune.speed*mSpeedRot);
    mNeptune.node->yaw(Ogre::Degree(20));
  }
  
  return ret;
}




//////////////////////////////////////////////////////////////////////////////////////////////
// KEY LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestSolarSystem::keyPressed ( const OIS::KeyEvent& arg )
{
  if(arg.key==OIS::KC_F1)
  {
    mTrayMgr->destroyAllWidgets();
    createMainMenu();
  }
  else if(arg.key==OIS::KC_UP)
  {
    mSun->getModelNode()->translate(0,0,0.1f);
  }
  else if(arg.key==OIS::KC_DOWN)
  {
    mSun->getModelNode()->translate(0,0,-0.1f);
  }
  else if(arg.key==OIS::KC_LEFT)
  {
    mSun->getModelNode()->translate(-0.1f,0,0);
  }
  else if(arg.key==OIS::KC_RIGHT)
  {
    mSun->getModelNode()->translate(0.1f,0,0);
  }
  return ArBaseApplication::keyPressed ( arg );
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestSolarSystem::keyReleased ( const OIS::KeyEvent& arg )
{
  return ArBaseApplication::keyReleased ( arg );
}




//////////////////////////////////////////////////////////////////////////////////////////////
// MOUSE LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestSolarSystem::mouseMoved(const OIS::MouseEvent &arg)
{
  return ArBaseApplication::mouseMoved(arg);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestSolarSystem::mousePressed(const OIS::MouseEvent &arg,OIS::MouseButtonID id)
{
  return ArBaseApplication::mousePressed(arg, id);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestSolarSystem::mouseReleased(const OIS::MouseEvent &arg,OIS::MouseButtonID id)
{
  return ArBaseApplication::mouseReleased(arg, id);
}




//////////////////////////////////////////////////////////////////////////////////////////////
// SDKTRAY LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
void TestSolarSystem::buttonHit ( OgreBites::Button* button )
{
  if(button->getName()=="bttWarning")
  {
    mTrayMgr->destroyAllWidgets();
    createInfoMenu();
  }
  else if(button->getName()=="bttExit") 
  {
    mShutDown=true;
  }
  else if(button->getName()=="bttReset")
  {
    mSun->getModelNode()->setOrientation(Ogre::Quaternion::IDENTITY);
    mMercury.rot->setOrientation(Ogre::Quaternion::IDENTITY);
    mVenus.rot->setOrientation(Ogre::Quaternion::IDENTITY);
    mEarth.rot->setOrientation(Ogre::Quaternion::IDENTITY);
    mMars.rot->setOrientation(Ogre::Quaternion::IDENTITY);
    mJupiter.rot->setOrientation(Ogre::Quaternion::IDENTITY);
    mSaturn.rot->setOrientation(Ogre::Quaternion::IDENTITY);
    mUranus.rot->setOrientation(Ogre::Quaternion::IDENTITY);
    mNeptune.rot->setOrientation(Ogre::Quaternion::IDENTITY);
  }
  else if(button->getName()=="bttHide")
  {
    mTrayMgr->destroyAllWidgets();
    mTrayMgr->hideCursor();
  }
  else if(button->getName()=="bttInfo")
  {
    createInfoMenu();
  }
  else if(button->getName()=="bttExitInfo")
  {
    mTrayMgr->destroyAllWidgets();
    createMainMenu();
  }
  else if(button->getName()=="bttPause")
  {
    mPauseRot=!mPauseRot;
  }
  else if(button->getName()=="bttSpdUp")
  {
    mSpeedRot=(mSpeedRot<64) ? mSpeedRot*2 : mSpeedRot;
  }
  else if(button->getName()=="bttSpdDown")
  {
    mSpeedRot=(mSpeedRot>1) ? mSpeedRot/2 : mSpeedRot;
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void TestSolarSystem::createMainMenu()
{
  mTrayMgr->showCursor();
  mTrayMgr->createLabel(OgreBites::TL_CENTER, "lblss", "Solar System", 180.0f);
  mBttReset=mTrayMgr->createButton(OgreBites::TL_CENTER, "bttReset", "Reset", 180.0f);
  mBttHide=mTrayMgr->createButton(OgreBites::TL_CENTER, "bttHide", "Hide", 180.0f);
  mBttInfo=mTrayMgr->createButton(OgreBites::TL_CENTER, "bttInfo", "Info", 180.0f);
  mTrayMgr->createSeparator(OgreBites::TL_CENTER, "mmSpt", 180.0f);
  mBttExit=mTrayMgr->createButton(OgreBites::TL_CENTER, "bttExit", "Exit", 180.0f);
  mBttPause=mTrayMgr->createButton(OgreBites::TL_BOTTOM, "bttPause", "Pause", 150.0f);
  mBttSpeedUp=mTrayMgr->createButton(OgreBites::TL_BOTTOMRIGHT, "bttSpdUp", "Speed Up x2", 180.0f);
  mBttSpeedDown=mTrayMgr->createButton(OgreBites::TL_BOTTOMLEFT, "bttSpdDown", "Speed Down x2", 150.0f);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void TestSolarSystem::createNotDetectedMenu()
{

}
//////////////////////////////////////////////////////////////////////////////////////////////
void TestSolarSystem::createInfoMenu()
{
  mTrayMgr->destroyAllWidgets();
  
  OgreBites::TextBox* tb=mTrayMgr->createTextBox(OgreBites::TL_CENTER, "tbInfo", "Solar System", 500, 300);
  tb->setText("SOLAR SYSTEM\n\n\n"
    "_-> Press F1 to display the main menu.\n"
    "_-> Use the widgets to control the speed.\n"
    "_-> Use the arrows to move the planets.\n" 
  );
  mBttExitInfo=mTrayMgr->createButton(OgreBites::TL_CENTER, "bttExitInfo", "Back", 180.0f);
}





//////////////////////////////////////////////////////////////////////////////////////////////
// CREATE THE SCENE LIGHTS
//////////////////////////////////////////////////////////////////////////////////////////////
void TestSolarSystem::createLights()
{
  // scene ambient light
  mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
  mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
  //mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
  //mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE);
  mSceneMgr->setShadowTextureSize(1024);
  mSceneMgr->setShadowColour(Ogre::ColourValue(0.7,0.7,0.7));
  
  // scene light
  mLightGeneral=mSun->getModelNode()->createChildSceneNode();
  Ogre::ColourValue colorLigh(1,1,1);
  Ogre::Light* light(mSceneMgr->createLight("sunLight"));
  light->setDiffuseColour(colorLigh);
  light->setSpecularColour(colorLigh);
  light->setCastShadows(true);
  mLightGeneral->attachObject(light);
  mLightGeneral->translate(0,100,0);
}




//////////////////////////////////////////////////////////////////////////////////////////////
// CREATE THE PARTICLES EFFECTS SCENE
//////////////////////////////////////////////////////////////////////////////////////////////
void TestSolarSystem::createParticles()
{
  // cam setup to see particles 
  mCamNode=mSun->getMarkerNode()->createChildSceneNode();
  mCamNode->attachObject(mCamera->getOgreCamera());
  mCamNode->yaw(Ogre::Degree(90));
  mCamNode->pitch(Ogre::Degree(90));
  mCamNode->roll(Ogre::Degree(90));
  
  Ogre::ParticleSystem::setDefaultNonVisibleUpdateTimeout(5);  // set nonvisible timeout
  Ogre::ParticleSystem* ps;
  Ogre::SceneNode* nodePS;
  Ogre::Vector3 sclPS(0.5f,0.5f,0.5f);
  
  // first particle, stars
  ps = mSceneMgr->createParticleSystem("Stars", "Space/Dust");
  nodePS=mSun->getModelNode()->createChildSceneNode();
  nodePS->attachObject(ps);
  nodePS->scale(sclPS);
  nodePS->translate(0, 1.2, 0);
  
  // second particles, colorfull galaxys
  ps = mSceneMgr->createParticleSystem("Galaxy", "Space/Galaxy");
  nodePS=mSun->getModelNode()->createChildSceneNode();
  nodePS->attachObject(ps);
  nodePS->scale(sclPS);
  nodePS->translate(0, 1.2, 0);
}



//////////////////////////////////////////////////////////////////////////////////////////////
// CREATE THE SOLAR SYSTEM SCENE
//////////////////////////////////////////////////////////////////////////////////////////////
void TestSolarSystem::createSolarSystem()
{
  // sun, parent node for solar system
  float markerSize=mDetector->getMarkerSize();
  mSun=new ArOgre::ArModel(mSceneMgr, "ArSun", "ArNodeSun", "mesh_sphere.mesh", -1, markerSize);
  mSun->getEntity()->setMaterialName("solarSystem_Sun");
  mSun->getModelNode()->translate(0,0.05,0);
  //mSun->getModelNode()->scale(2,2,2);
  mSun->getEntity()->getSubEntity(0)->getMaterial()->setLightingEnabled(false);
  mSun->getEntity()->getSubEntity(0)->getMaterial()->setReceiveShadows(false);
  
  // planets - mercury
  mMercury.entity=mSceneMgr->createEntity("mesh_sphere.mesh");
  mMercury.rot=mSun->getModelNode()->createChildSceneNode();
  mMercury.node=mMercury.rot->createChildSceneNode();
  mMercury.node->attachObject(mMercury.entity);
  mMercury.node->translate(6, 0, 0);
  mMercury.node->scale(0.1,0.1,0.1);
  mMercury.entity->setMaterialName("solarSystem_Mercury");
  mMercury.speed=3;
  mMercury.rot->yaw(Ogre::Degree(Ogre::Math::RangeRandom(0, 365)));
  
  // planets - venus
  mVenus.entity=mSceneMgr->createEntity("mesh_sphere.mesh");
  mVenus.rot=mSun->getModelNode()->createChildSceneNode();
  mVenus.node=mVenus.rot->createChildSceneNode();
  mVenus.node->attachObject(mVenus.entity);
  mVenus.node->translate(8, 0, 0);
  mVenus.node->scale(0.17,0.17,0.17);
  mVenus.entity->setMaterialName("solarSystem_Venus");
  mVenus.speed=1.8;
  mVenus.rot->yaw(Ogre::Degree(Ogre::Math::RangeRandom(0, 365)));
  
  // planets - earth
  mEarth.entity=mSceneMgr->createEntity("mesh_sphere.mesh");
  mEarth.rot=mSun->getModelNode()->createChildSceneNode();
  mEarth.node=mEarth.rot->createChildSceneNode();
  mEarth.node->attachObject(mEarth.entity);
  mEarth.node->translate(11, 0, 0);
  mEarth.node->scale(0.175,0.175,0.175);
  mEarth.entity->setMaterialName("solarSystem_Earth");
  mEarth.speed=1;
  mEarth.rot->yaw(Ogre::Degree(Ogre::Math::RangeRandom(0, 365)));
  
  // planets - mars
  mMars.entity=mSceneMgr->createEntity("mesh_sphere.mesh");
  mMars.rot=mSun->getModelNode()->createChildSceneNode();
  mMars.node=mMars.rot->createChildSceneNode();
  mMars.node->attachObject(mMars.entity);
  mMars.node->translate(14, 0, 0);
  mMars.node->scale(0.11,0.11,0.11);
  mMars.entity->setMaterialName("solarSystem_Mars");
  mMars.speed=0.5;
  mMars.rot->yaw(Ogre::Degree(Ogre::Math::RangeRandom(0, 365)));
  
  // planets - jupiter
  mJupiter.entity=mSceneMgr->createEntity("mesh_sphere.mesh");
  mJupiter.rot=mSun->getModelNode()->createChildSceneNode();
  mJupiter.node=mJupiter.rot->createChildSceneNode();
  mJupiter.node->attachObject(mJupiter.entity);
  mJupiter.node->translate(18, 0, 0);
  mJupiter.node->scale(0.5,0.5,0.5);
  mJupiter.entity->setMaterialName("solarSystem_Jupiter");
  mJupiter.speed=0.320f;
  mJupiter.rot->yaw(Ogre::Degree(Ogre::Math::RangeRandom(0, 365)));
  
  // planets - saturn
  mSaturn.entity=mSceneMgr->createEntity("mesh_sphere.mesh");
  mSaturn.rot=mSun->getModelNode()->createChildSceneNode();
  mSaturn.node=mSaturn.rot->createChildSceneNode();
  mSaturn.node->attachObject(mSaturn.entity);
  mSaturn.node->translate(26, 0, 0);
  mSaturn.node->scale(0.4,0.4,0.4); 
  mSaturn.entity->setMaterialName("solarSystem_Saturn");
  mSaturn.speed=0.290f;
  mSaturn.rot->yaw(Ogre::Degree(Ogre::Math::RangeRandom(0, 365)));
  
  // planets - saturns rings
  Ogre::Entity* entRings=mSceneMgr->createEntity("mesh_rings.mesh");
  entRings->setMaterialName("solarSystem_SaturnRings");
  Ogre::SceneNode* nodeRings=mSaturn.node->createChildSceneNode();
  nodeRings->attachObject(entRings);
  mSaturn.node->pitch(Ogre::Degree(30));
  
  // planets - uranus
  mUranus.entity=mSceneMgr->createEntity("mesh_sphere.mesh");
  mUranus.rot=mSun->getModelNode()->createChildSceneNode();
  mUranus.node=mUranus.rot->createChildSceneNode();
  mUranus.node->attachObject(mUranus.entity);
  mUranus.node->translate(33, 0, 0);
  mUranus.node->scale(0.24,0.24,0.24); 
  mUranus.entity->setMaterialName("solarSystem_Uranus");
  mUranus.speed=0.184f;
  mUranus.rot->yaw(Ogre::Degree(Ogre::Math::RangeRandom(0, 365)));
  
  // planets - nepture
  mNeptune.entity=mSceneMgr->createEntity("mesh_sphere.mesh");
  mNeptune.rot=mSun->getModelNode()->createChildSceneNode();
  mNeptune.node=mNeptune.rot->createChildSceneNode();
  mNeptune.node->attachObject(mNeptune.entity);
  mNeptune.node->translate(37, 0, 0);
  mNeptune.node->scale(0.195,0.195,0.195); 
  mNeptune.entity->setMaterialName("solarSystem_Neptune");
  mNeptune.speed=0.164f;
  mNeptune.rot->yaw(Ogre::Degree(Ogre::Math::RangeRandom(0, 365)));
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
        TestSolarSystem app;

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

