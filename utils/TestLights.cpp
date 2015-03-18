#include "PCH.h"
#include "TestLights.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR
//////////////////////////////////////////////////////////////////////////////////////////////
TestLights::TestLights(void)
  : mModel(0)
{
  srand(time(NULL));
}
//////////////////////////////////////////////////////////////////////////////////////////////
TestLights::~TestLights(void)
{
  if(mModel) delete mModel; 
}




//////////////////////////////////////////////////////////////////////////////////////////////
// CREATE SCENE
//////////////////////////////////////////////////////////////////////////////////////////////
void TestLights::createScene(void)
{
  Ogre::Real markerSize=mDetector->getMarkerSize();
  
  // statue
  mModel=new ArOgre::ArModel(mSceneMgr, "arLight", "arModel", "mesh_statue.mesh", -1, markerSize);
  
  // colums
  for(unsigned int i=0; i<4; ++i)
  {
    Ogre::String numCol=Ogre::StringConverter::toString(i+1);
    vColum.push_back(new ArOgre::ArModel(mSceneMgr, "arCol"+numCol, "arMCol"+numCol, "mesh_column.mesh", -1, markerSize));
    vColum.back()->getModelNode()->scale(0.55f, 0.55f, 0.55f);
    vColum.back()->getModelNode()->translate(0,0.005f, 0);
  }
  vColum[0]->setIdMarker(mDetector->getBoardIdMarker(1,0));
  vColum[0]->getModelNode()->scale(1,0.45f,1);
  vColum[1]->setIdMarker(mDetector->getBoardIdMarker(1,3));
  vColum[1]->getModelNode()->scale(1,0.65f,1);
  vColum[2]->setIdMarker(mDetector->getBoardIdMarker(4,0));
  vColum[2]->getModelNode()->scale(1,0.85f,1);
  vColum[3]->setIdMarker(mDetector->getBoardIdMarker(4,3));
  
  // create ground
  mPlane=new Ogre::Plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane("groundPlane",	
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
    (*mPlane),
    1, 1, 20, 20, true, 1, 5, 4, 
    Ogre::Vector3::UNIT_Z);
  
  // create ground
  mGround=new ArOgre::ArModel(mSceneMgr, "ground", "ground1", "groundPlane", -1, 0.05);
  mGround->getModelNode()->scale(9,0,6.25f);
  mGround->getModelNode()->translate(0,0.0005f,0);
  mGround->getEntity()->setMaterialName("Examples/Rockwall");
}




//////////////////////////////////////////////////////////////////////////////////////////////
// FRAME LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
void TestLights::createFrameListener()
{
  ArBaseApplication::createFrameListener();

  // Create main menu
  if(!mShowWarning)
    createMainMenu();
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestLights::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  bool ret=ArBaseApplication::frameRenderingQueued(evt);
  
  bool detected;
  Ogre::Vector3 newPos; 
  Ogre::Quaternion newQua;
  
  detected=mDetector->detectBoard(newPos, newQua);
  mModel->update(detected, newPos, newQua);
  mGround->update(detected, newPos, newQua);
  
  for(unsigned int i=0; i<vColum.size(); ++i)
  {
    detected=mDetector->detectIdMarker(vColum[i]->getIdMarker(), newPos, newQua);
    vColum[i]->update(detected, newPos, newQua);
  }
  
  for(unsigned int i=0; i<vAnimLight.size(); ++i)
  {
    if(vAnimLight[i]->getEnabled())
    {
	  vNodeLight[i]->setVisible(detected);
      vAnimLight[i]->addTime(evt.timeSinceLastFrame);
    }
  }
    
  return ret;
}




//////////////////////////////////////////////////////////////////////////////////////////////
// KEY LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestLights::keyPressed ( const OIS::KeyEvent& arg )
{
  return ArBaseApplication::keyPressed ( arg );
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestLights::keyReleased ( const OIS::KeyEvent& arg )
{
    return ArBaseApplication::keyReleased ( arg );
}




//////////////////////////////////////////////////////////////////////////////////////////////
// MOUSE LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestLights::mouseMoved(const OIS::MouseEvent &arg)
{
   return ArBaseApplication::mouseMoved(arg);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestLights::mousePressed(const OIS::MouseEvent &arg,OIS::MouseButtonID id)
{
  return ArBaseApplication::mousePressed(arg, id);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestLights::mouseReleased(const OIS::MouseEvent &arg,OIS::MouseButtonID id)
{
  return ArBaseApplication::mouseReleased(arg, id);
}




//////////////////////////////////////////////////////////////////////////////////////////////
// SDKTRAY LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
void TestLights::buttonHit ( OgreBites::Button* button )
{
  if(button->getName()=="bttWarning")
  {
    mTrayMgr->destroyAllWidgets();
    createMainMenu();
  }
  else if(button->getName()=="bttAddLight")
  {
    addLight();
  }
  else if(button->getName()=="bttDelLight")
  {
    removeLight();
  }
  else
  {
    ArBaseApplication::buttonHit(button); 
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void TestLights::sliderMoved ( OgreBites::Slider* slider )
{
  if(slider->getName()=="sldAmbient")
  {
    Ogre::Real valAmbient=slider->getValue();
    Ogre::ColourValue newAmbientLight(valAmbient, valAmbient, valAmbient);
    mSceneMgr->setAmbientLight(newAmbientLight);
  }
}
void TestLights::checkBoxToggled ( OgreBites::CheckBox* box )
{
  if(box->getName()=="cbShadows")
  {
    if(box->isChecked())
    {
      mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
      mSceneMgr->setShadowColour(Ogre::ColourValue(0.7f, 0.76f, 0.7f));
      mBackground->enableShadows(mModel->getMarkerNode());
    }
    else
    {
      mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_NONE);
      mBackground->disableShadows();
    }
  }
  else if(box->getName()=="cbGlobalLight")
  {
    if(box->isChecked())
    {
      if(!mBackground->getShadowEnabled())
	mBackground->enableShadows(mModel->getMarkerNode());
      
      mBackground->enableGlobalIlumination(mCamera->getOgreCamera());
    }
    else
    {
      mBackground->disableGlobalIlumination();
    }
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void TestLights::createMainMenu()
{
  mTrayMgr->showCursor();
  
  mTrayMgr->createLabel(OgreBites::TL_BOTTOMLEFT, "lblLights", "Lights", 135.0f);
  mTrayMgr->createSeparator(OgreBites::TL_BOTTOMLEFT, "sptLight", 135.0f);
  mTrayMgr->createButton(OgreBites::TL_BOTTOMLEFT, "bttAddLight", "Add light", 135.0f);
  mTrayMgr->createButton(OgreBites::TL_BOTTOMLEFT, "bttDelLight", "Remove light", 135.0f);
  
  mTrayMgr->createLabel(OgreBites::TL_BOTTOMRIGHT, "lblConfig", "Shadows", 135.0f);
  mTrayMgr->createSeparator(OgreBites::TL_BOTTOMRIGHT, "sptConfig", 135.0f);
  mTrayMgr->createCheckBox(OgreBites::TL_BOTTOMRIGHT, "cbShadows", "Shadows", 135.0f);
  mTrayMgr->createCheckBox(OgreBites::TL_BOTTOMRIGHT, "cbGlobalLight", "Global light", 135.0f);
  
  OgreBites::Slider* sldAmbient=mTrayMgr->createLongSlider(
    OgreBites::TL_BOTTOM, "sldAmbient", "Ambient Light", 175, 50, 0, 1, 11);
  sldAmbient->setValue(1);
}




void TestLights::addLight ( void )
{
  // node light - sphere and ribbontrail
  vEntLight.push_back(mSceneMgr->createEntity("mesh_sphere_2.mesh"));
  vEntLight.back()->setCastShadows(false);
  vNodeLight.push_back(mModel->getModelNode()->createChildSceneNode(Ogre::Vector3(6, 10, 0)));
  vNodeLight.back()->attachObject(vEntLight.back());
  vNodeLight.back()->scale(0.05f, 0.05f, 0.05f);
  
  // material sphere and light
  Ogre::String nameLight=vEntLight.back()->getName();
  Ogre::ColourValue colorLigh(
    Ogre::Math::RangeRandom(0.5f, 1.0f), 
    Ogre::Math::RangeRandom(0.5f, 1.0f),
    Ogre::Math::RangeRandom(0.5f, 1.0f), 1.0f);
  Ogre::MaterialPtr matPtr=Ogre::MaterialManager::getSingleton().create(
    "matLuz_"+nameLight, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
  matPtr->setDiffuse(colorLigh);
  matPtr->setAmbient(colorLigh);
  matPtr->setSpecular(colorLigh);
  matPtr->setSelfIllumination(colorLigh);
  vEntLight.back()->setMaterial(matPtr);
  
  // create a ribbon trail that lights will leave behind
  Ogre::NameValuePairList params;
  params["numberOfChains"] = "2";
  params["maxElements"] = "80";
  Ogre::RibbonTrail* trail = (Ogre::RibbonTrail*)mSceneMgr->createMovableObject("RibbonTrail", &params);
  trail->setMaterialName("Examples/LightRibbonTrail");
  trail->setTrailLength(8);
  trail->setInitialColour(0, colorLigh);
  trail->setColourChange(0, 0.5f, 0.5f, 0.5f, 1.0f);
  trail->setInitialWidth(0, 0.5f);
  trail->addNode(vNodeLight.back());
  trail->setCastShadows(true);
  mModel->getModelNode()->attachObject(trail);
   
  // attach a light with the same colour to the light node
  Ogre::Light* light(mSceneMgr->createLight("Light_"+nameLight));
  light->setDiffuseColour(colorLigh);
  light->setSpecularColour(colorLigh);
  vNodeLight.back()->attachObject(light);
  
  // animation ligh
  Ogre::Animation* anim;
  Ogre::Real animTime=Ogre::Math::RangeRandom(8, 20);
  anim=mSceneMgr->createAnimation("AnimPath_"+nameLight, animTime);
  anim->setInterpolationMode(Ogre::Animation::IM_SPLINE);
 
  Ogre::NodeAnimationTrack* track=anim->createNodeTrack(1, vNodeLight.back());

  // enter keyframes for our track to define a path for the light to follow
  Ogre::Vector3 posIni(Ogre::Vector3(0, Ogre::Math::RangeRandom(5,20), -10));
  track->createNodeKeyFrame(0)->setTranslate(posIni);
  track->getNodeKeyFrame(0)->setScale(vNodeLight.back()->getScale());
  track->getNodeKeyFrame(0)->setRotation(vNodeLight.back()->getOrientation());
  Ogre::Real keyTime=animTime/4.0f;
  track->createNodeKeyFrame(keyTime)->setTranslate(Ogre::Vector3(
    Ogre::Math::RangeRandom(10, 20), 
    Ogre::Math::RangeRandom(5,20), 
    0));
  track->getNodeKeyFrame(1)->setScale(vNodeLight.back()->getScale());
  track->getNodeKeyFrame(1)->setRotation(vNodeLight.back()->getOrientation());
  keyTime+=animTime/4.0f;
  track->createNodeKeyFrame(keyTime)->setTranslate(Ogre::Vector3(
    0,
    Ogre::Math::RangeRandom(5,15), 
    Ogre::Math::RangeRandom(10,15)));
  track->getNodeKeyFrame(2)->setScale(vNodeLight.back()->getScale());
  track->getNodeKeyFrame(2)->setRotation(vNodeLight.back()->getOrientation());
  keyTime+=animTime/4.0f;
  track->createNodeKeyFrame(keyTime)->setTranslate(Ogre::Vector3(
    Ogre::Math::RangeRandom(-5,-25), 
    Ogre::Math::RangeRandom(10,15), 
    0));
  track->getNodeKeyFrame(3)->setScale(vNodeLight.back()->getScale());
  track->getNodeKeyFrame(3)->setRotation(vNodeLight.back()->getOrientation());
  keyTime+=animTime/4.0f;
  track->createNodeKeyFrame(keyTime)->setTranslate(posIni);
  track->getNodeKeyFrame(4)->setScale(vNodeLight.back()->getScale());
  track->getNodeKeyFrame(4)->setRotation(vNodeLight.back()->getOrientation());
  
  // create an animation state from the animation and enable it
  vAnimLight.push_back(mSceneMgr->createAnimationState("AnimPath_"+nameLight));
  vAnimLight.back()->setEnabled(true);
  vAnimLight.back()->setLoop(true);
}

void TestLights::removeLight ( void )
{
  if(!vNodeLight.empty())
  {
    Ogre::String nameLight(vEntLight.back()->getName());
    mSceneMgr->destroyEntity(vEntLight.back());
    mSceneMgr->destroySceneNode(vNodeLight.back());
    mSceneMgr->destroyAnimation("AnimPath_"+nameLight);
    mSceneMgr->destroyAnimationState("AnimPath_"+nameLight);
    mSceneMgr->destroyLight("Light_"+nameLight);
    
    vEntLight.pop_back();
    vNodeLight.pop_back();
    vAnimLight.pop_back();
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
        TestLights app;

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

