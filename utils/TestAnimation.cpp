#include "PCH.h"
#include "TestAnimation.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR
//////////////////////////////////////////////////////////////////////////////////////////////
TestAnimation::TestAnimation()
  : mCrystal(0),
    mNumModels(0),
    mAnimChop(0)
{
}
//////////////////////////////////////////////////////////////////////////////////////////////
TestAnimation::~TestAnimation()
{
  if(mCrystal) delete mCrystal;
}




//////////////////////////////////////////////////////////////////////////////////////////////
// CREATE SCENE
//////////////////////////////////////////////////////////////////////////////////////////////
void TestAnimation::createScene()
{
  
}




//////////////////////////////////////////////////////////////////////////////////////////////
// FRAME LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
void TestAnimation::createFrameListener()
{
  ArBaseApplication::createFrameListener();
  
  // Create main menu
  if(!mShowWarning)
    createMainMenu();
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestAnimation::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  bool ret=ArBaseApplication::frameRenderingQueued(evt);

  bool detected;
  Ogre::Vector3 newPos; 
  Ogre::Quaternion newQua;
  
  if(mCrystal)
  {
    detected=mDetector->detectBoard(newPos, newQua);
    mCrystal->update(detected, newPos, newQua);
    mGround->update(mHideGround, newPos, newQua);
    
    
    for(unsigned int i=0; i<mNumModels; i++)
    {
      // update sneaking animation based on speed
      mAnimStates[i]->addTime(mAnimSpeeds[i] * evt.timeSinceLastFrame);
      mModelNodes[i]->setVisible(detected);

      if (mAnimStates[i]->getTimePosition() >= mAnimChop)   // when it's time to loop...
      {
	/* We need reposition the scene node origin, since the animation includes translation.
	Position is calculated from an offset to the end position, and rotation is calculated
	from how much the animation turns the character. */

	Ogre::Quaternion rot(Ogre::Degree(-60), Ogre::Vector3::UNIT_Y);   // how much the animation turns the character

	// find current end position and the offset
	Ogre::Vector3 currEnd = mModelNodes[i]->getOrientation() * mSneakEndPos + mModelNodes[i]->getPosition();
	Ogre::Vector3 offset = rot * mModelNodes[i]->getOrientation() * -mSneakStartPos;

	mModelNodes[i]->setPosition(currEnd + offset);
	mModelNodes[i]->rotate(rot);
	    
	mAnimStates[i]->setTimePosition(0);   // reset animation time
      }
    }
  }
  
  return ret;
}




//////////////////////////////////////////////////////////////////////////////////////////////
// KEY LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestAnimation::keyPressed ( const OIS::KeyEvent& arg )
{
  if(arg.key==OIS::KC_F1)
  {
    mTrayMgr->destroyAllWidgets();
    createHideMenu();
  }
  
  return ArBaseApplication::keyPressed ( arg );
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestAnimation::keyReleased ( const OIS::KeyEvent& arg )
{
  return ArBaseApplication::keyReleased ( arg );
}




//////////////////////////////////////////////////////////////////////////////////////////////
// MOUSE LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestAnimation::mouseMoved(const OIS::MouseEvent &arg)
{
   return ArBaseApplication::mouseMoved(arg);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestAnimation::mousePressed(const OIS::MouseEvent &arg,OIS::MouseButtonID id)
{
  return ArBaseApplication::mousePressed(arg, id);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestAnimation::mouseReleased(const OIS::MouseEvent &arg,OIS::MouseButtonID id)
{
  return ArBaseApplication::mouseReleased(arg, id);
}




//////////////////////////////////////////////////////////////////////////////////////////////
// SDKTRAY LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
void TestAnimation::buttonHit ( OgreBites::Button* button )
{
  if(button->getName()=="bttWarning")
  {
    mTrayMgr->destroyAllWidgets();
    createMainMenu();
  }
  else if(button->getName()=="bttHide")
  {
   mTrayMgr->destroyAllWidgets(); 
  }
  else if(button->getName()=="bttExitModel")
  {
    mTrayMgr->destroyAllWidgets();
    Ogre::Real markerSize=mDetector->getMarkerSize();
  
    // models scene
    mCrystal=new ArOgre::ArModel(mSceneMgr, "arCrystal", "arNodeCrystal", "mesh_crystal.mesh", -1, markerSize);
    mCrystal->getEntity()->setCastShadows(true);
    setupModels();
    
    // particle effect
    Ogre::ParticleSystem::setDefaultNonVisibleUpdateTimeout(5);  // set nonvisible timeout
    Ogre::ParticleSystem* ps;
    Ogre::SceneNode* nodePS;

    // create a blue nimbus around the crystal
    ps = mSceneMgr->createParticleSystem("Nimbus", "Examples/BlueNimbus");
    nodePS=mCrystal->getModelNode()->createChildSceneNode();
    nodePS->attachObject(ps);
    nodePS->scale(0.5f, 0.5f, 0.5f);
    
    // create plane, diferent plane for diferent model
    mPlane=new Ogre::Plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane("groundPlane",	
      Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
      (*mPlane), 1, 1, 20, 20, true, 1, 4, 2, Ogre::Vector3::UNIT_Z);
  
    // create entity plane 
    mGround=new ArOgre::ArModel(mSceneMgr, "ground", "ground1", "groundPlane", -1, 0.05);
    mGround->getModelNode()->scale(9,0,6.25f);
    mGround->getModelNode()->translate(0,0.0005f,0);
    mGround->getEntity()->setMaterialName("Examples/BumpyMetal");
  
    // setup scene light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
    mSceneMgr->setShadowColour(Ogre::ColourValue(0.75f, 0.75f, 0.75f));
    mBackground->enableShadows(mCrystal->getMarkerNode());
    
    // point light
    Ogre::Light* pointLight = mSceneMgr->createLight("pointLight");
    pointLight->setType(Ogre::Light::LT_POINT);
    pointLight->setPosition(Ogre::Vector3(0.0f, 0.085f, 0.0f));
    pointLight->setDiffuseColour(Ogre::ColourValue(0.3f, 0.3f, 0.9f));
    Ogre::SceneNode *nodeLight=mCrystal->getModelNode()->createChildSceneNode();
    nodeLight->attachObject(pointLight);
    
    // menu hide
    createHideMenu();
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void TestAnimation::sliderMoved ( OgreBites::Slider* slider )
{
  if(slider->getName()=="mSliderM")
    mNumModels=slider->getValue();
  else 
    mAnimChop=slider->getValue();
}
//////////////////////////////////////////////////////////////////////////////////////////////
void TestAnimation::checkBoxToggled ( OgreBites::CheckBox* box )
{
  mHideGround=!box->isChecked();
}
//////////////////////////////////////////////////////////////////////////////////////////////
void TestAnimation::createMainMenu()
{
  mTrayMgr->showCursor();
  
  OgreBites::TextBox* tb=
  mTrayMgr->createTextBox(OgreBites::TL_CENTER, "tb", "Test Animation", 320, 200);
  tb->setText("TEST ANIMATION\n\n\n"
    "_-> A demo of the skeletal animation feature, including spline animation.\n"
    "_-> Press F1 to show or hide the option menu."
  );
  mSldModel=mTrayMgr->createLongSlider(OgreBites::TL_CENTER, "mSliderM", "Num Models", 150, 50, 1, 20, 20);
  mSldChop=mTrayMgr->createLongSlider(OgreBites::TL_CENTER, "mSliderC", "Anim Chop  ", 150, 50, 5, 10, 6);
  mTrayMgr->createSeparator(OgreBites::TL_CENTER, "spt", 150);
  mTrayMgr->createButton(OgreBites::TL_CENTER, "bttExitModel", "Accept", 150);
  
  mSldModel->setValue(9);
  mSldChop->setValue(8);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void TestAnimation::createHideMenu()
{
  OgreBites::CheckBox* cbHide=
    mTrayMgr->createCheckBox(OgreBites::TL_BOTTOMLEFT, "cbHide", "Hide ground", 150.0f);  
  cbHide->setChecked(!mGround->getEntity()->isVisible());
}





//////////////////////////////////////////////////////////////////////////////////////////////
// SCENE MODELS
//////////////////////////////////////////////////////////////////////////////////////////////
void TestAnimation::setupModels()
{
  tweakSneakAnim();

  Ogre::SceneNode* sn = NULL;
  Ogre::Entity* ent = NULL;
  Ogre::AnimationState* as = NULL;

  for(unsigned int i=0; i<mNumModels; i++)
  {
    // create scene nodes for the models at regular angular intervals
    sn = mCrystal->getModelNode()->createChildSceneNode();
    sn->yaw(Ogre::Radian(Ogre::Math::TWO_PI * (float)i / (float)mNumModels));
    sn->translate(0, 0, -10, Ogre::Node::TS_LOCAL);
    mModelNodes.push_back(sn);

    // create and attach a jaiqua entity
    ent = mSceneMgr->createEntity("Jaiqua" + Ogre::StringConverter::toString(i + 1), "jaiqua.mesh");
    sn->attachObject(ent);
    ent->setCastShadows(true);
    
    // enable the entity's sneaking animation at a random speed and loop it manually since translation is involved
    as = ent->getAnimationState("Sneak");
    as->setEnabled(true);
    as->setLoop(false);
    mAnimSpeeds.push_back(Ogre::Math::RangeRandom(0.5, 1.5));
    mAnimStates.push_back(as);
  }

  // create name and value for skinning mode
  Ogre::StringVector names;
  names.push_back("Skinning");
  Ogre::String value = "Software";

  // change the value if hardware skinning is enabled
  Ogre::Pass* pass = ent->getSubEntity(0)->getMaterial()->getBestTechnique()->getPass(0);
  if (pass->hasVertexProgram() && pass->getVertexProgram()->isSkeletalAnimationIncluded()) value = "Hardware";
}

void TestAnimation::tweakSneakAnim()
{
  // get the skeleton, animation, and the node track iterator
  Ogre::SkeletonPtr skel = Ogre::SkeletonManager::getSingleton().load("jaiqua.skeleton",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
  Ogre::Animation* anim = skel->getAnimation("Sneak");
  Ogre::Animation::NodeTrackIterator tracks = anim->getNodeTrackIterator();
  
  while (tracks.hasMoreElements())   // for every node track...
  {
    Ogre::NodeAnimationTrack* track = tracks.getNext();
    // get the keyframe at the chopping point
    Ogre::TransformKeyFrame oldKf(0, 0);
    track->getInterpolatedKeyFrame(mAnimChop, &oldKf);

    // drop all keyframes after the chopping point
    while (track->getKeyFrame(track->getNumKeyFrames()-1)->getTime() >= mAnimChop - 0.3f)
      track->removeKeyFrame(track->getNumKeyFrames()-1);
    
    // create a new keyframe at chopping point, and get the first keyframe
    Ogre::TransformKeyFrame* newKf = track->createNodeKeyFrame(mAnimChop);
    Ogre::TransformKeyFrame* startKf = track->getNodeKeyFrame(0);

    Ogre::Bone* bone = skel->getBone(track->getHandle());
    if (bone->getName() == "Spineroot")   // adjust spine root relative to new location
    {
      mSneakStartPos = startKf->getTranslate() + bone->getInitialPosition();
      mSneakEndPos = oldKf.getTranslate() + bone->getInitialPosition();
      mSneakStartPos.y = mSneakEndPos.y;

      newKf->setTranslate(oldKf.getTranslate());
      newKf->setRotation(oldKf.getRotation());
      newKf->setScale(oldKf.getScale());
    }
    else   // make all other bones loop back
    {
      newKf->setTranslate(startKf->getTranslate());
      newKf->setRotation(startKf->getRotation());
      newKf->setScale(startKf->getScale());
    }
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
        TestAnimation app;

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

