#include "PCH.h"
#include "TestGame.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR
//////////////////////////////////////////////////////////////////////////////////////////////
TestGame::TestGame()
  : mSinbad(0),
    mGrass(0),
    mSword1(0),
    mSword2(0),
    mAnimBase(0),
    mAnimTop(0),
    mAnimAtack(0)
{
  srand(time(NULL));
}
//////////////////////////////////////////////////////////////////////////////////////////////
TestGame::~TestGame()
{
  if(mSinbad) delete mSinbad;
  if(mGrass) delete mGrass;
}




//////////////////////////////////////////////////////////////////////////////////////////////
// CREATE SCENE
//////////////////////////////////////////////////////////////////////////////////////////////
void TestGame::createScene()
{
  mSceneMgr->setAmbientLight(Ogre::ColourValue(1,1,1));
  
  // grass
  Ogre::Real markerSize=mDetector->getMarkerSize();
  mGrass=new ArOgre::ArModel(mSceneMgr, "arGrass", "arGrassModel", "mesh_grass.mesh", -1, markerSize);
  mGrass->getModelNode()->scale(8,7,8);
  mGrass->getEntity()->setCastShadows(false);
  
  // sinbad
  mSinbad=new ArOgre::ArModel(mSceneMgr, "arSinbad", "arSinbadModel", "Sinbad.mesh", -1, markerSize);
  mSinbad->getModelNode()->scale(1.5f, 1.5f, 1.5f); 
  mSinbad->getModelNode()->translate(0.0f, 0.05f, 0.0f);
  mSinbad->getEntity()->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);
  // swords
  mSword1=mSceneMgr->createEntity("SinbadSword1", "Sword.mesh");
  mSword2=mSceneMgr->createEntity("SinbadSword2", "Sword.mesh");
  mSinbad->getEntity()->attachObjectToBone("Handle.L", mSword1);
  mSinbad->getEntity()->attachObjectToBone("Handle.R", mSword2);
  
  // enemies
  Ogre::SceneNode* nodeEnemie=mGrass->getModelNode();
  for(unsigned int i=0; i<Ogre::Math::RangeRandom(20,30); ++i)
  {
    vEntEnemies.push_back(mSceneMgr->createEntity("ogrehead.mesh"));
    vNodeEnemies.push_back(nodeEnemie->createChildSceneNode());
    vNodeEnemies.back()->attachObject(vEntEnemies.back());
    Ogre::Real sclEnem=Ogre::Math::RangeRandom(10, 25)*0.001f;
    vNodeEnemies.back()->setScale(sclEnem, sclEnem, sclEnem);
    vNodeEnemies.back()->translate(Ogre::Vector3(
      Ogre::Math::RangeRandom(-8,8),
      1, 
      Ogre::Math::RangeRandom(-4,4)));
  }
  
  // setup scene light
  mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
  mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
  mSceneMgr->setShadowColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
  mBackground->enableShadows(mGrass->getMarkerNode());
  
  // point light
  Ogre::Light* pointLight = mSceneMgr->createLight("pointLight");
  pointLight->setType(Ogre::Light::LT_POINT);
  pointLight->setPosition(Ogre::Vector3(0.0f, 2.0f, 0.0f));
  pointLight->setDiffuseColour(Ogre::ColourValue(0.94f, 0.94f, 0.94f));
  Ogre::SceneNode *nodeLight=mGrass->getModelNode()->createChildSceneNode();
  nodeLight->attachObject(pointLight);
}




//////////////////////////////////////////////////////////////////////////////////////////////
// FRAME LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
void TestGame::createFrameListener()
{
  ArBaseApplication::createFrameListener();
  mTrayMgr->showCursor();

  // Create main menu
  if(!mShowWarning)
    createWelcomeMenu();

  // create default anims
  mAnimBase=mSinbad->getEntity()->getAnimationState("IdleBase");
  mAnimTop=mSinbad->getEntity()->getAnimationState("IdleTop");
  mAnimAtack=mSinbad->getEntity()->getAnimationState("SliceVertical");
  
  mAnimBase->setLoop(true); 	mAnimBase->setEnabled(true);
  mAnimTop->setLoop(true);	mAnimTop->setEnabled(true);
  mAnimAtack->setLoop(true);	mAnimAtack->setEnabled(false);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestGame::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  bool ret=ArBaseApplication::frameRenderingQueued(evt);
  
  bool detected;
  Ogre::Vector3 newPos; 
  Ogre::Quaternion newQua;
  
  // update arModels
  detected=mDetector->detectBoard(newPos, newQua);
  mGrass->update(detected, newPos, newQua);
  mSinbad->update(detected, newPos, newQua);
  
  // update animations
  if(mAnimTop)	mAnimTop->addTime(evt.timeSinceLastFrame);
  if(mAnimBase)	mAnimBase->addTime(evt.timeSinceLastFrame);
  if(mAnimAtack) mAnimAtack->addTime(evt.timeSinceLastFrame);
  for(unsigned int i=0; i<vNodeEnemies.size(); ++i)
    vNodeEnemies[i]->yaw(Ogre::Degree(evt.timeSinceLastFrame*100.0f));
    
  // attack - eliminate enemies
  if(mAnimAtack->getEnabled() && vNodeEnemies.size()>0)
  {
    mSinbad->getModelNode()->_update(true, true);
    for(unsigned int i=0; i<vNodeEnemies.size(); ++i)
    {
      vNodeEnemies[i]->_update(true, true);
      if(mSword1->getWorldBoundingBox().intersects(vEntEnemies[i]->getWorldBoundingBox()))
      {
	mSceneMgr->destroyEntity(vEntEnemies[i]);
	mSceneMgr->destroySceneNode(vNodeEnemies[i]);
	vEntEnemies.erase(vEntEnemies.begin()+i);
	vNodeEnemies.erase(vNodeEnemies.begin()+i);
      }
    }
    
    // checks end game
    if(vNodeEnemies.size()>0)
    {
      mLblInfo->setCaption("Enemies left: "+Ogre::StringConverter::toString(vNodeEnemies.size()));
    }
    else
    {
      mLblInfo->setCaption("You win!!!");
      
      mAnimBase->setEnabled(false);
      mAnimTop->setEnabled(false);
      mAnimAtack->setEnabled(false);
      
      mAnimBase=mSinbad->getEntity()->getAnimationState("Dance");
      mAnimBase->setLoop(true);
      mAnimBase->setEnabled(true);
      mSword1->setVisible(false);
      mSword2->setVisible(false);
      mTrayMgr->destroyAllWidgets();
      createGameMenu();
    }
  }
  
  if(vEntEnemies.size()>0)
    processUnbufferedInput(evt);
  
  return ret;
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestGame::processUnbufferedInput ( const Ogre::FrameEvent& evt )
{
  // animation "IDLE"
  mAnimBase->setEnabled(false);
  mAnimTop->setEnabled(false);
  mAnimAtack->setEnabled(false);
  
  mAnimBase=mSinbad->getEntity()->getAnimationState("IdleBase");
  mAnimTop=mSinbad->getEntity()->getAnimationState("IdleTop");

  mAnimBase->setLoop(true);	mAnimBase->setEnabled(true);	
  mAnimTop->setLoop(true);	mAnimTop->setEnabled(true);

  // animation "RUN"
  if(mKeyboard->isKeyDown(OIS::KC_LEFT) || mKeyboard->isKeyDown(OIS::KC_RIGHT) || 
     mKeyboard->isKeyDown(OIS::KC_UP) || mKeyboard->isKeyDown(OIS::KC_DOWN))
  {
    mAnimBase->setEnabled(false);
    mAnimTop->setEnabled(false);

    mAnimBase=mSinbad->getEntity()->getAnimationState("RunBase");
    mAnimTop=mSinbad->getEntity()->getAnimationState("RunTop");
 
    mAnimBase->setLoop(true); mAnimBase->setEnabled(true);
    mAnimTop->setLoop(true); mAnimTop->setEnabled(true);
  }
  if(mKeyboard->isKeyDown(OIS::KC_SPACE))
  {
    mAnimTop->setEnabled(false);
    
    mAnimAtack->setEnabled(true);
  }
  
  Ogre::Vector3 transVector=Ogre::Vector3::ZERO;
  Ogre::Real rot=0.0f;
  static Ogre::Real mRotate=7.0f;	// The rotate constant
  static Ogre::Real mMove=0.15f;	// The movement constant

  // model controller
  if (mKeyboard->isKeyDown(OIS::KC_UP)) // Forward
  {
    transVector.z+=mMove;
  }
  if (mKeyboard->isKeyDown(OIS::KC_DOWN)) // Backward
  {
    transVector.z-=mMove;
  }
  if (mKeyboard->isKeyDown(OIS::KC_LEFT)) // Left - yaw
  {
    rot+=mRotate;
  }
  if (mKeyboard->isKeyDown(OIS::KC_RIGHT)) // Right - yaw
  {
    rot-=mRotate;
  }

  // aply transformations
  mSinbad->getModelNode()->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
  mSinbad->getModelNode()->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(rot));
  
  return true;
}





//////////////////////////////////////////////////////////////////////////////////////////////
// KEY LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestGame::keyPressed ( const OIS::KeyEvent& arg )
{
  return ArBaseApplication::keyPressed ( arg );
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestGame::keyReleased ( const OIS::KeyEvent& arg )
{
  return ArBaseApplication::keyReleased ( arg );
}




//////////////////////////////////////////////////////////////////////////////////////////////
// MOUSE LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestGame::mouseMoved(const OIS::MouseEvent &arg)
{
   return ArBaseApplication::mouseMoved(arg);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestGame::mousePressed(const OIS::MouseEvent &arg,OIS::MouseButtonID id)
{
  return ArBaseApplication::mousePressed(arg, id);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestGame::mouseReleased(const OIS::MouseEvent &arg,OIS::MouseButtonID id)
{
  return ArBaseApplication::mouseReleased(arg, id);
}




//////////////////////////////////////////////////////////////////////////////////////////////
// SDKTRAY LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
void TestGame::buttonHit ( OgreBites::Button* button )
{
  if(button->getName()=="bttWarning")
  {
    mTrayMgr->destroyAllWidgets();
    createWelcomeMenu();
  }
  else if(button->getName()=="bttExit")
  {
    mShutDown=true;
  }
  else if(button->getName()=="bttExitInfo")
  {
    mTrayMgr->destroyAllWidgets();
    mLblInfo=mTrayMgr->createLabel(OgreBites::TL_TOP, "lblEnem", "Enemies", 200.0f);
    mLblInfo->setCaption("Enemies left: "+Ogre::StringConverter::toString(vNodeEnemies.size()));
  }
  else if(button->getName()=="bttReset")
  {
    for(unsigned int i=0; i<vNodeEnemies.size(); ++i)
    {
      mSceneMgr->destroyEntity(vEntEnemies.back());
      mSceneMgr->destroySceneNode(vNodeEnemies.back());
      vEntEnemies.pop_back();
      vNodeEnemies.pop_back();
    }
    // enemies
    Ogre::SceneNode* nodeEnemie=mGrass->getModelNode();
    for(unsigned int i=0; i<Ogre::Math::RangeRandom(20,30); ++i)
    {
      vEntEnemies.push_back(mSceneMgr->createEntity("ogrehead.mesh"));
      vNodeEnemies.push_back(nodeEnemie->createChildSceneNode());
      vNodeEnemies.back()->attachObject(vEntEnemies.back());
      Ogre::Real sclEnem=Ogre::Math::RangeRandom(10, 25)*0.001f;
      vNodeEnemies.back()->setScale(sclEnem, sclEnem, sclEnem);
      vNodeEnemies.back()->translate(Ogre::Vector3(
	Ogre::Math::RangeRandom(-8,8),
	1, 
	Ogre::Math::RangeRandom(-4,4)));
    }
    
    mSword1->setVisible(true);
    mSword2->setVisible(true);
    mTrayMgr->destroyAllWidgets();
    mLblInfo=mTrayMgr->createLabel(OgreBites::TL_TOP, "lblEnem", "Enemies", 200.0f);
    mLblInfo->setCaption("Enemies left: "+Ogre::StringConverter::toString(vNodeEnemies.size()));
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void TestGame::createWelcomeMenu()
{
  mTrayMgr->showCursor();
  
  OgreBites::TextBox* tb=mTrayMgr->createTextBox(OgreBites::TL_CENTER, "tbInfo", "ArOgre Game", 500, 300);
  tb->setText("FIND HEADS\n\n\n"
    "_-> Find and destroy all the heads that are hidden in the grass.\n"
    "_-> To move character use the arrows.\n" 
    "_-> To atack press space.\n");
  mTrayMgr->createButton(OgreBites::TL_CENTER, "bttExitInfo", "Accept", 180.0f);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void TestGame::createGameMenu()
{
  mTrayMgr->showCursor();
  mTrayMgr->createLabel(OgreBites::TL_CENTER, "lblgg", "Heads Game", 180.0f);
  mTrayMgr->createSeparator(OgreBites::TL_CENTER, "mmSpt", 180.0f);
  mTrayMgr->createButton(OgreBites::TL_CENTER, "bttReset", "Reset", 180.0f);
  mTrayMgr->createButton(OgreBites::TL_CENTER, "bttExit", "Exit", 180.0f);
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
        TestGame app;

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

