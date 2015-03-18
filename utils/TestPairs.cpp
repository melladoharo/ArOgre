#include "PCH.h"
#include "TestPairs.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR
//////////////////////////////////////////////////////////////////////////////////////////////
TestPairs::TestPairs()
  : mRaySceneQuery(0),
    mTimeReturn(0.0f),
    mGameStarted(false),
    cbEasy(0), cbNormal(0), cbHard(0)
{
  srand(time(NULL));
}
//////////////////////////////////////////////////////////////////////////////////////////////
TestPairs::~TestPairs()
{
  for(unsigned int i=0; i<vPairs.size(); ++i) delete vPairs[i].model;
  if(mRaySceneQuery) mSceneMgr->destroyQuery(mRaySceneQuery);
}




//////////////////////////////////////////////////////////////////////////////////////////////
// CREATE SCENE
//////////////////////////////////////////////////////////////////////////////////////////////
void TestPairs::createScene()
{
  mSceneMgr->setAmbientLight(Ogre::ColourValue(0.9f, 0.95f, 0.95f));
}




//////////////////////////////////////////////////////////////////////////////////////////////
// FRAME LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
void TestPairs::createFrameListener()
{
  ArBaseApplication::createFrameListener();
  mTrayMgr->showCursor();

  // create main menu
  if(!mShowWarning)
    createMainMenu();

  // create raySceneQuery
  mRaySceneQuery=mSceneMgr->createRayQuery(Ogre::Ray());
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestPairs::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
 bool ret=ArBaseApplication::frameRenderingQueued(evt);

  bool detected;
  Ogre::Vector3 newPos; 
  Ogre::Quaternion newQua;
  
  // update models
  for(unsigned int i=0; i<vPairs.size(); ++i)
  {
    if(mGameStarted)
    {
      // update anim front
      if(vPairs[i].animFront->getEnabled())
      {
	vPairs[i].animFront->addTime(evt.timeSinceLastFrame);
	if(vPairs[i].animFront->hasEnded())
	{
	  vPairs[i].animFront->setEnabled(false);
	}
      }
      // update anim back
      if(vPairs[i].animBack->getEnabled())
      {
	vPairs[i].animBack->addTime(evt.timeSinceLastFrame);
	if(vPairs[i].animBack->hasEnded())
	{
	  vPairs[i].animBack->setEnabled(false);
	}
      } 
    }
    else
    {
      detected=mDetector->detectIdMarker(vPairs[i].model->getIdMarker(), newPos, newQua);
      vPairs[i].model->update(detected, newPos, newQua);
    }
  }
  
  if(mGameStarted)
  {
    // checks if two selected items are paired
    if(mTimeReturn>1.0f && vModelSelected.size()>1)
    {
      Ogre::MaterialPtr matPtrPareja1=Ogre::MaterialManager::getSingleton().getByName(
	Ogre::String(vPairs[vModelSelected[0]].model->getEntity()->getSubEntity(0)->getMaterialName()));
      Ogre::MaterialPtr matPtrPareja2=Ogre::MaterialManager::getSingleton().getByName(
	Ogre::String(vPairs[vModelSelected[1]].model->getEntity()->getSubEntity(0)->getMaterialName()));
      
      // the selected models have the same texture, then are paired
      if(matPtrPareja1->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName() == 
	matPtrPareja2->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName())
      {
	vPairs[vModelSelected[0]].paired=true;
	vPairs[vModelSelected[1]].paired=true;
	
	// all paired?
	bool endGame=true;
	for(unsigned int i=0; i<vPairs.size(); ++i)
	{
	  if(vPairs[i].paired==false)
	  {
	    endGame=false;
	  }
	}
	if(endGame)
	{
	  for(unsigned int i=0; i<vPairs.size(); ++i) delete vPairs[i].model;
	  mSceneMgr->destroyAllAnimations();
	  mSceneMgr->destroyAllAnimationStates();
	  vPairs.clear();
	  createMainMenu();
	}
      }
      else // not paired, deselect
      {
	vPairs[vModelSelected[0]].animBack->setTimePosition(0);
	vPairs[vModelSelected[0]].animBack->setEnabled(true);
	vPairs[vModelSelected[1]].animBack->setTimePosition(0);
	vPairs[vModelSelected[1]].animBack->setEnabled(true);
      }
      vPairs[vModelSelected[0]].selected=false;
      vPairs[vModelSelected[1]].selected=false;
      vModelSelected.clear();
    }
  }
  
  mTimeReturn+=evt.timeSinceLastFrame;
  
  return ret;
}




//////////////////////////////////////////////////////////////////////////////////////////////
// KEY LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestPairs::keyPressed ( const OIS::KeyEvent& arg )
{
  return ArBaseApplication::keyPressed ( arg );
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestPairs::keyReleased ( const OIS::KeyEvent& arg )
{
  return ArBaseApplication::keyReleased ( arg );
}




//////////////////////////////////////////////////////////////////////////////////////////////
// MOUSE LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestPairs::mouseMoved(const OIS::MouseEvent &arg)
{
  return ArBaseApplication::mouseMoved(arg);
}

bool TestPairs::mousePressed ( const OIS::MouseEvent& arg, OIS::MouseButtonID id )
{
  if(id==OIS::MB_Left) // left mouse button down
  { 
    Ogre::Ray mouseRay = mTrayMgr->getCursorRay(mCamera->getOgreCamera());
    mRaySceneQuery->setRay(mouseRay);

    // execute query
    Ogre::RaySceneQueryResult &result=mRaySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator iter=result.begin();

    // select single object
    for(;iter!=result.end(); iter++) 
    {
      // if clicked on a ARmodel
      if(iter->movable->getParentSceneNode()->getName().find("selector_Item")==Ogre::String::npos &&
	 iter->movable->getParentSceneNode()->getName().find("node_GS_")!=Ogre::String::npos)
      {
	// find the armodel clicked
	for(unsigned int i=0; i<vPairs.size(); ++i)
	{
	  if(vPairs[i].model->getModelNode()->getName()==iter->movable->getParentSceneNode()->getName())
	  {
	    // not permited select more than two pairs
	    if(vModelSelected.size()<2 && !vPairs[i].selected && !vPairs[i].paired)
	    {
	      if(!vPairs[i].animBack->getEnabled())
	      {
		vModelSelected.push_back(i);
		vPairs[i].animFront->setTimePosition(0);
		vPairs[i].animFront->setEnabled(true);
		vPairs[i].selected=true;
		mTimeReturn=0;
	      }  
	    }
	  }
	}
      }
    }
  }
  
  return ArBaseApplication::mousePressed ( arg, id );
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestPairs::mouseReleased(const OIS::MouseEvent &arg,OIS::MouseButtonID id)
{
  return ArBaseApplication::mouseReleased(arg, id);
}




//////////////////////////////////////////////////////////////////////////////////////////////
// SDKTRAY LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
void TestPairs::buttonHit ( OgreBites::Button* button )
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
  else if(button->getName()=="bttStart")
  {
    mTrayMgr->destroyAllWidgets();
    mGameStarted=true;
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void TestPairs::checkBoxToggled ( OgreBites::CheckBox* box )
{
  if(box->getName()=="cbEasy" && box->isChecked())
  {
    createPairs(3);
    cbNormal->setChecked(false, false);
    cbHard->setChecked(false, false);
  }
  else if(box->getName()=="cbNormal" && box->isChecked())
  {
    createPairs(6);
    cbEasy->setChecked(false, false);
    cbHard->setChecked(false, false);
  }
  else if(box->getName()=="cbHard" && box->isChecked())
  {
    createPairs(12);
    cbEasy->setChecked(false, false);
    cbNormal->setChecked(false, false);
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void TestPairs::createMainMenu()
{
  mTrayMgr->showCursor();
  mTrayMgr->createLabel(OgreBites::TL_CENTER, "lblpairs", "Find the pair", 180.0f);
  mTrayMgr->createSeparator(OgreBites::TL_CENTER, "sptpairs", 160.0f);
  cbEasy=mTrayMgr->createCheckBox(OgreBites::TL_CENTER, "cbEasy", "Easy", 160.0f);
  cbNormal=mTrayMgr->createCheckBox(OgreBites::TL_CENTER, "cbNormal", "Normal", 160.0f);
  cbHard=mTrayMgr->createCheckBox(OgreBites::TL_CENTER, "cbHard", "Hard", 160.0f);
  mTrayMgr->createButton(OgreBites::TL_CENTER, "bttStart", "Start", 160.0f);
  //mTrayMgr->createButton(OgreBites::TL_CENTER, "bttEasy", "Easy", 180.0f);
  //mTrayMgr->createButton(OgreBites::TL_CENTER, "bttNormal", "Normal", 180.0f);
  //mTrayMgr->createButton(OgreBites::TL_CENTER, "bttHard", "Hard", 180.0f);
  //mTrayMgr->createSeparator(OgreBites::TL_CENTER, "mmSpt", 180.0f);
  mTrayMgr->createButton(OgreBites::TL_CENTER, "bttExit", "Exit", 160.0f);
}




//////////////////////////////////////////////////////////////////////////////////////////////
// CREATE PAIRS
//////////////////////////////////////////////////////////////////////////////////////////////
void TestPairs::createPairs ( unsigned int numPairs )
{
  // remove the previous pairs
  for(unsigned int i=0; i<vPairs.size(); ++i) delete vPairs[i].model;
  mSceneMgr->destroyAllAnimations();
  mSceneMgr->destroyAllAnimationStates();
  vPairs.clear();
  
  // texture pair name: text_pair_animal_1.jpg, text_pair_animal_2.jpg, ...
  std::vector<Ogre::String> vTextName;
  for(unsigned int i=1; i<=12; ++i)
  {
    vTextName.push_back("text_pair_animal_" + Ogre::StringConverter::toString(i) + ".jpg");
  }
  
  // ramdon animal texture
  unsigned int selectName=rand()%vTextName.size();
  
  // marker size
  Ogre::Real markerSize=mDetector->getMarkerSize();
  
  // all id markers available
  std::vector<int> vId=mDetector->getBoardIdMarker();
  std::random_shuffle(vId.begin(), vId.end());
  
  // create pairs
  for(unsigned int i=1; i<=numPairs*2; ++i)
  {
    Ogre::String nMdl=Ogre::StringConverter::toString(i);
    vPairs.push_back(sPair());
    vPairs.back().paired=false;
    vPairs.back().selected=false;
    vPairs.back().model=new ArOgre::ArModel(mSceneMgr, "AR_GS_"+nMdl, "node_GS_"+nMdl, "mesh_pairs.mesh", vId[i-1], markerSize);
    Ogre::MaterialPtr matPtr=Ogre::MaterialManager::getSingleton().create("mat_Sel_"+nMdl, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    matPtr->getTechnique(0)->getPass(0)->createTextureUnitState(vTextName[selectName]);
    vPairs.back().model->getEntity()->getSubEntity(0)->setMaterial(matPtr); 
    vPairs.back().animFront=createAnimation(vPairs.back().model->getModelNode(), false);
    vPairs.back().animBack=createAnimation(vPairs.back().model->getModelNode(), true);
    
    // ramdon id marker
    if(i%2==0)
      selectName=(selectName==vTextName.size()-1) ? 0 : selectName+1;
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////
Ogre::AnimationState* TestPairs::createAnimation ( Ogre::SceneNode* node, bool reverse )
{
  Ogre::Real animTime=0.3f;
  Ogre::String nameAnim; 
  if(reverse) nameAnim="animation_AR_back_"+node->getName();
  else nameAnim="animation_AR_front_"+node->getName();
  Ogre::Animation* animation=mSceneMgr->createAnimation(nameAnim, animTime);
  animation->setInterpolationMode(Ogre::Animation::IM_SPLINE);
  
  Ogre::NodeAnimationTrack* animTrack=animation->createNodeTrack(0, node);
  animTrack->createNodeKeyFrame(0);
  animTrack->getNodeKeyFrame(0)->setTranslate(node->getPosition());
  if(reverse)  animTrack->getNodeKeyFrame(0)->setRotation(Ogre::Quaternion(Ogre::Degree(179.99f), Ogre::Vector3::UNIT_Z));
  else animTrack->getNodeKeyFrame(0)->setRotation(Ogre::Quaternion(Ogre::Degree(0), Ogre::Vector3::UNIT_Z));
  animTrack->getNodeKeyFrame(0)->setScale(node->getScale());
  animTrack->createNodeKeyFrame(animTime-0.05f);
  animTrack->getNodeKeyFrame(1)->setTranslate(node->getPosition());
  if(reverse) animTrack->getNodeKeyFrame(1)->setRotation(Ogre::Quaternion(Ogre::Degree(0.0f), Ogre::Vector3::UNIT_Z));
  else animTrack->getNodeKeyFrame(1)->setRotation(Ogre::Quaternion(Ogre::Degree(179.99f), Ogre::Vector3::UNIT_Z));
  animTrack->getNodeKeyFrame(1)->setScale(node->getScale());
  
  Ogre::AnimationState* animState=mSceneMgr->createAnimationState(nameAnim);
  animState->setLoop(false);
  animState->setEnabled(false);
  return animState;
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
        TestPairs app;

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

