#ifndef _TESTANIMATION_H__
#define _TESTANIMATION_H__

#include "PCH.h"
#include "ArBaseApplication.h"

class TestAnimation : public ArBaseApplication
{
public:
    TestAnimation();
    virtual ~TestAnimation();

protected:
    // Frame Listener
    virtual void createScene();
    virtual void createFrameListener();
    virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);

    // Key Listener
    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);
    
    // Mouse Listener
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg,OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg,OIS::MouseButtonID id);
    
    // Sdktray Listener
    virtual void buttonHit(OgreBites::Button* button);
    virtual void sliderMoved( OgreBites::Slider* slider);
    virtual void checkBoxToggled(OgreBites::CheckBox* box);
    
    // sdktray menu
    void createMainMenu();
    void createHideMenu();

    // scene models config
    void setupModels();
    void tweakSneakAnim();
    
private:
  // AR
  ArOgre::ArModel* board;
  ArOgre::ArModel* mCrystal;
  std::vector<Ogre::SceneNode*> mModelNodes;
  std::vector<Ogre::AnimationState*> mAnimStates;
  std::vector<Ogre::Real> mAnimSpeeds;
  
  // ground
  Ogre::Plane* mPlane;
  Ogre::Entity* mEntPlane;
  ArOgre::ArModel* mGround;
  
  // Anim
  Ogre::Vector3 mSneakStartPos;
  Ogre::Vector3 mSneakEndPos;
  
  // Menu
  unsigned int mNumModels, mAnimChop;
  OgreBites::Slider* mSldModel, *mSldChop;
  bool mHideGround;
};

#endif // #ifndef _TESTANIMATION_H__
