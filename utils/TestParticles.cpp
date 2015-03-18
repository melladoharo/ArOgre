#include "PCH.h"
#include "TestParticles.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR & DESTRUCTOR
//////////////////////////////////////////////////////////////////////////////////////////////
TestParticles::TestParticles()
  : mModel(0),
    mJetEnginePivot(0),
    mCamNode(0)
{
}
//////////////////////////////////////////////////////////////////////////////////////////////
TestParticles::~TestParticles()
{
  if(mModel) delete mModel;
}




//////////////////////////////////////////////////////////////////////////////////////////////
// CREATE SCENE
//////////////////////////////////////////////////////////////////////////////////////////////
void TestParticles::createScene()
{
  // models
  Ogre::Real markerSize=mDetector->getMarkerSize();
  mModel=new ArOgre::ArModel(mSceneMgr, "arPar", "arModel", "mesh_barrel.mesh", -1, markerSize);

  mCamNode=mModel->getMarkerNode()->createChildSceneNode();
  mCamNode->attachObject(mCamera->getOgreCamera());
  mCamNode->yaw(Ogre::Degree(90));
  mCamNode->pitch(Ogre::Degree(90));
  mCamNode->roll(Ogre::Degree(90));
  
  // create particles
  setupParticles();
  
  // lights
  mSceneMgr->setAmbientLight(Ogre::ColourValue(0.97f, 0.95f, 0.96f));
  mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
  mSceneMgr->setShadowColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
  mBackground->enableShadows(mModel->getMarkerNode());
  
  // point light
  Ogre::Light* pointLight = mSceneMgr->createLight("pointLight");
  pointLight->setType(Ogre::Light::LT_POINT);
  pointLight->setPosition(Ogre::Vector3(0.0f, 0.25f, 0.0f));
  pointLight->setDiffuseColour(Ogre::ColourValue(0.0f, 0.0f, 0.5f));
}




//////////////////////////////////////////////////////////////////////////////////////////////
// FRAME LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
void TestParticles::createFrameListener()
{
  ArBaseApplication::createFrameListener();
  //mTrayMgr->destroyAllWidgets();

  // create main menu
  createMainMenu();
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestParticles::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  bool ret=ArBaseApplication::frameRenderingQueued(evt);

  bool detected;
  Ogre::Vector3 newPos; 
  Ogre::Quaternion newQua;
  
  // update the board model
  detected=mDetector->detectBoard(newPos, newQua);
  mModel->update(detected, newPos, newQua);
  
  // visibility particles
  mJetEnginePivot->yaw(Ogre::Degree(evt.timeSinceLastFrame*30.0f));
  mSceneMgr->getParticleSystem(cbFireworks->getName())->setVisible(cbFireworks->isChecked());
  mSceneMgr->getParticleSystem(cbJetEngine1->getName())->setVisible(cbJetEngine1->isChecked());
  mSceneMgr->getParticleSystem(cbJetEngine2->getName())->setVisible(cbJetEngine2->isChecked());
  mSceneMgr->getParticleSystem(cbSwarm->getName())->setVisible(cbSwarm->isChecked());
  mSceneMgr->getParticleSystem(cbAureola->getName())->setVisible(cbAureola->isChecked());
  mSceneMgr->getParticleSystem(cbNimbus->getName())->setVisible(cbNimbus->isChecked());
  mSceneMgr->getParticleSystem(cbSmoke->getName())->setVisible(cbSmoke->isChecked());
  mSceneMgr->getParticleSystem(cbSnow->getName())->setVisible(cbSnow->isChecked());
    
  return ret;
}




//////////////////////////////////////////////////////////////////////////////////////////////
// KEY LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestParticles::keyPressed ( const OIS::KeyEvent& arg )
{
  if(arg.key==OIS::KC_UP)
  {
    mCamNode->pitch(Ogre::Degree(5));
    //mModel->getModelNode()->pitch(Ogre::Degree(5));
  }
    return ArBaseApplication::keyPressed ( arg );
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestParticles::keyReleased ( const OIS::KeyEvent& arg )
{
    return ArBaseApplication::keyReleased ( arg );
}




//////////////////////////////////////////////////////////////////////////////////////////////
// MOUSE LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestParticles::mouseMoved(const OIS::MouseEvent &arg)
{
   return ArBaseApplication::mouseMoved(arg);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestParticles::mousePressed(const OIS::MouseEvent &arg,OIS::MouseButtonID id)
{
  return ArBaseApplication::mousePressed(arg, id);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestParticles::mouseReleased(const OIS::MouseEvent &arg,OIS::MouseButtonID id)
{
  return ArBaseApplication::mouseReleased(arg, id);
}




//////////////////////////////////////////////////////////////////////////////////////////////
// SDKTRAY LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
void TestParticles::buttonHit ( OgreBites::Button* button )
{
  ArBaseApplication::buttonHit(button);
}

//////////////////////////////////////////////////////////////////////////////////////////////
void TestParticles::createMainMenu()
{
  mTrayMgr->createLabel(OgreBites::TL_TOPLEFT, "VisLabel", "Particles");
  cbFireworks=mTrayMgr->createCheckBox(OgreBites::TL_TOPLEFT, "Fireworks", "Fireworks", 130);
  cbJetEngine1=mTrayMgr->createCheckBox(OgreBites::TL_TOPLEFT, "JetEngine1", "JetEngine A", 130);
  cbJetEngine2=mTrayMgr->createCheckBox(OgreBites::TL_TOPLEFT, "JetEngine2", "JetEngine B", 130);
  cbSwarm=mTrayMgr->createCheckBox(OgreBites::TL_TOPLEFT, "Swarm", "Swarm", 130);
  cbAureola=mTrayMgr->createCheckBox(OgreBites::TL_TOPLEFT, "Aureola", "Aureola", 130);
  cbNimbus=mTrayMgr->createCheckBox(OgreBites::TL_TOPLEFT, "Nimbus", "Nimbus", 130);
  cbSmoke=mTrayMgr->createCheckBox(OgreBites::TL_TOPLEFT, "Smoke", "Smoke", 130);
  cbSnow=mTrayMgr->createCheckBox(OgreBites::TL_TOPLEFT, "Snow", "Snow", 130);
  cbFireworks->setChecked(false);
  cbJetEngine1->setChecked(false);
  cbJetEngine2->setChecked(false);
  cbSwarm->setChecked(false);
  cbAureola->setChecked(false);
  cbNimbus->setChecked(false);
  cbSmoke->setChecked(false);
  cbSnow->setChecked(false);
}




//////////////////////////////////////////////////////////////////////////////////////////////
// SETUP PARTICLES
//////////////////////////////////////////////////////////////////////////////////////////////
void TestParticles::setupParticles()
{
  Ogre::ParticleSystem::setDefaultNonVisibleUpdateTimeout(5);  // set nonvisible timeout
  Ogre::ParticleSystem* ps;
  Ogre::SceneNode* nodePS;
  Ogre::Vector3 sclPS(0.0612f, 0.0612f, 0.0612f);
  

  
  // create some nice fireworks and place it at the origin
  ps = mSceneMgr->createParticleSystem("Fireworks", "Examples/Fireworks");
  ps->setVisible(false);
  nodePS=mModel->getModelNode()->createChildSceneNode();
  nodePS->attachObject(ps);
  nodePS->scale(sclPS);
  
  // create a green nimbus around the ogre head
  ps = mSceneMgr->createParticleSystem("Nimbus", "Examples/GreenyNimbus");
  ps->setVisible(false);
  nodePS=mModel->getModelNode()->createChildSceneNode();
  nodePS->attachObject(ps);
  nodePS->scale(sclPS);

  // create aureola perpendicular to the ground
  ps = mSceneMgr->createParticleSystem("Aureola", "Examples/Aureola");
  ps->setVisible(false);
  nodePS=mModel->getModelNode()->createChildSceneNode();
  nodePS->attachObject(ps);
  nodePS->scale(sclPS);
  
  // swarm
  ps = mSceneMgr->createParticleSystem("Swarm", "Examples/Swarm");
  ps->setVisible(false);
  nodePS=mModel->getModelNode()->createChildSceneNode();
  nodePS->attachObject(ps);
  nodePS->scale(sclPS);
  
  // smoke
  ps = mSceneMgr->createParticleSystem("Smoke", "Examples/Smoke");
  ps->setVisible(false);
  nodePS=mModel->getModelNode()->createChildSceneNode();
  nodePS->attachObject(ps);
  nodePS->scale(sclPS);
  
  // snow
  ps = mSceneMgr->createParticleSystem("Snow", "Examples/Snow");
  ps->setVisible(false);
  nodePS=mModel->getModelNode()->createChildSceneNode();
  nodePS->attachObject(ps);
  nodePS->scale(sclPS);
  
  // create shared pivot node for spinning the fountains
  mJetEnginePivot = nodePS=mModel->getModelNode()->createChildSceneNode();
  Ogre::Vector3 posJetEngine(5.0f, 0.0f, 0.0f);
  
  // attach the fountain to a child node of the pivot at a distance and angle
  ps = mSceneMgr->createParticleSystem("JetEngine1", "Examples/JetEngine1");
  ps->setVisible(false);
  nodePS=mJetEnginePivot->createChildSceneNode();
  nodePS->attachObject(ps);
  nodePS->scale(sclPS);
  nodePS->translate(posJetEngine);
  
  ps = mSceneMgr->createParticleSystem("JetEngine2", "Examples/JetEngine2");
  ps->setVisible(false);
  nodePS=mJetEnginePivot->createChildSceneNode();
  nodePS->attachObject(ps);
  nodePS->scale(sclPS);
  nodePS->translate(-posJetEngine);
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
        TestParticles app;

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

