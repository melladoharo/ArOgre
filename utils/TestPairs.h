#ifndef _TESTPAIRS_H__
#define _TESTPAIRS_H__

#include "PCH.h"
#include "ArBaseApplication.h"

class TestPairs : public ArBaseApplication
{
public:
    TestPairs();
    virtual ~TestPairs();

protected:
    // Frame Listener
    virtual void createScene();
    virtual void createFrameListener();
    virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);

    // Key Listener
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    
    // Mouse Listener
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg,OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg,OIS::MouseButtonID id);
    
    // Sdktray Listener
    virtual void buttonHit(OgreBites::Button* button);
    virtual void checkBoxToggled(OgreBites::CheckBox* box);
    
    // sdktray menu
    void createMainMenu();
    
    // pairs
    void createPairs(unsigned int numPairs);
    //void createAnimation(Ogre::SceneNode* node, Ogre::SceneNode* node2=NULL);
    Ogre::AnimationState* createAnimation(Ogre::SceneNode* node, bool reverse);
    
private:
  struct sPair
  {
    ArOgre::ArModel* model;		// arogre model
    bool paired;			// paired or not
    bool selected;			// selected or not
    Ogre::AnimationState* animFront;	// anim selection
    Ogre::AnimationState* animBack;	// anim deselection
  };
  
  std::vector<sPair> vPairs;
  Ogre::RaySceneQuery *mRaySceneQuery;		// The ray scene query pointer
  std::vector<unsigned int> vModelSelected;	// models selected
  Ogre::Real mTimeReturn;			// time to check if there are pairs
  bool mGameStarted;				// start the game
  
  // menu sdktray
  OgreBites::CheckBox* cbEasy, *cbNormal, *cbHard;
};

#endif // #ifndef _TESTPAIRS_H__
