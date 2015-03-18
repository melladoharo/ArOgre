#ifndef _TESTGAME_H__
#define _TESTGAME_H__

#include "PCH.h"
#include "ArBaseApplication.h"

class TestGame : public ArBaseApplication
{
public:
    TestGame();
    virtual ~TestGame();

protected:
    // Frame Listener
    virtual void createScene();
    virtual void createFrameListener();
    virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);
    bool processUnbufferedInput(const Ogre::FrameEvent& evt);
    
    // Key Listener
    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);
    
    // Mouse Listener
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg,OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg,OIS::MouseButtonID id);
    
    // Sdktray Listener
    virtual void buttonHit(OgreBites::Button* button);
    
    // sdktray menu
    void createWelcomeMenu();
    void createGameMenu();

private:
  // models
  ArOgre::ArModel* mSinbad,* mGrass;
  Ogre::Entity* mSword1,* mSword2;
  
  // enemies
  std::vector<Ogre::Entity*> vEntEnemies;
  std::vector<Ogre::SceneNode*> vNodeEnemies;
  
  // animation
  Ogre::AnimationState* mAnimBase;
  Ogre::AnimationState* mAnimTop;
  Ogre::AnimationState* mAnimAtack;
  
  // sdktray
  OgreBites::Label* mLblInfo;
};

#endif // #ifndef _TESTGAME_H__
