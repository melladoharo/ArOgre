#ifndef _TESTPARTICLES_H__
#define _TESTPARTICLES_H__

#include "PCH.h"
#include "ArBaseApplication.h"

class TestParticles : public ArBaseApplication
{
public:
    TestParticles(void);
    virtual ~TestParticles(void);

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
    
    // sdktray menu
    void createMainMenu();
    
    void setupParticles();

private:
  // models
  ArOgre::ArModel* mModel;
  
  // particles
  Ogre::SceneNode* mJetEnginePivot;
  Ogre::SceneNode* mCamNode;
  OgreBites::CheckBox* cbFireworks;
  OgreBites::CheckBox* cbJetEngine1;
  OgreBites::CheckBox* cbJetEngine2;
  OgreBites::CheckBox* cbSwarm;
  OgreBites::CheckBox* cbAureola;
  OgreBites::CheckBox* cbNimbus;
  OgreBites::CheckBox* cbSmoke;
  OgreBites::CheckBox* cbSnow;
};

#endif // #ifndef _TESTPARTICLES_H__
