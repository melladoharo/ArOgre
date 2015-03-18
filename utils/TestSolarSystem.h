#ifndef _TESTSOLARSYSTEM_H__
#define _TESTSOLARSYSTEM_H__

#include "PCH.h"
#include "ArBaseApplication.h"

struct sPlanet
{
  Ogre::SceneNode* rot;
  Ogre::SceneNode* node;
  Ogre::Entity* entity;
  float speed;
};
  
class TestSolarSystem : public ArBaseApplication
{
public:
    TestSolarSystem(void);
    virtual ~TestSolarSystem(void);

protected:
    // Frame Listener
    virtual void createScene(void);
    virtual void createFrameListener(void);
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
    
    // create solarsystem scene
    void createSolarSystem();
    void createParticles();
    void createLights();
    
    // sdktray menu
    void createMainMenu();
    void createInfoMenu();
    void createNotDetectedMenu();
    
private:
  ArOgre::ArModel* mSun;
  Ogre::SceneNode* mLightGeneral;
 
  sPlanet mMercury, mVenus, mEarth, mMars, mJupiter, mSaturn, mUranus, mNeptune;
  Ogre::SceneNode* mCamNode;
  
  // sdktrays
  OgreBites::Button* mBttReset, *mBttExit, *mBttHide, *mBttInfo,
		    *mBttPause, *mBttSpeedUp, *mBttSpeedDown, *mBttExitInfo;
  bool mPauseRot;
  int mSpeedRot;
};

#endif // #ifndef _TESTSOLARSYSTEM_H__
