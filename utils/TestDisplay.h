#ifndef _TESTDISPLAY_H__
#define _TESTDISPLAY_H__

#include "PCH.h"
#include "ArBaseApplication.h"

enum eAction{DCOLOR, DROTATE, DSCALE, DSELECT};

class TestDisplay : public ArBaseApplication
{
public:
  TestDisplay();
  virtual ~TestDisplay();

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
  
  void createDisplay();
  void performAction(bool next);

private:
  ArOgre::ArModel* mLight, * mModel, * mNextAction, * mPreviousAction;
  Ogre::Real mTimeActionNext, mTimeActionPrev, mTimeChange;
  
  std::vector<unsigned int> vIdChange;
  std::vector<Ogre::ColourValue> vColors;
  std::vector<Ogre::String> vNameModels;
  
  unsigned int mCurrentAcion, mCurrentColor, mCurrentModel;
};

#endif // #ifndef _TESTDISPLAY_H__
