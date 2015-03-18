#ifndef _TESTBOARDCONTROL_H__
#define _TESTBOARDCONTROL_H__

#include "PCH.h"
#include "ArBaseApplication.h"

// Fuel
struct sFuel
{
  Ogre::Entity* entidad;
  Ogre::SceneNode* node;
  Ogre::Real speed;
};
  
class TestBoardControl : public ArBaseApplication
{
public: 
  TestBoardControl();
  virtual ~TestBoardControl();

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
  void createInfoMenu();
  void createMainMenu();
  void createEndMenu();

  void createMainScene();
  void createFuel();
  void updateFuel();
  void randomFuel(sFuel& f);
  
private:
  // Airplane
  ArOgre::ArModel* mAirplane;
  ArOgre::ArModel* mArOcean;
  Ogre::SceneNode* mPropeller;
  Ogre::Entity* mOceanSurfaceEnt;
  Ogre::SceneNode* mOceanSurfaceNode;
  
  // ground message
  Ogre::Plane* mPlane;
  Ogre::Entity* mEntPlane;
  ArOgre::ArModel* mGround;
  
  // Fuel
  std::vector<sFuel> vFuel;
  
  // sdktray
  OgreBites::ProgressBar* pbFuel;
  bool mGameOver;
};

#endif // #ifndef _TESTBOARDCONTROL_H__
