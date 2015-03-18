#ifndef _TESTLIGHTS_H__
#define _TESTLIGHTS_H__

#include "PCH.h"
#include "ArBaseApplication.h"

class TestLights : public ArBaseApplication
{
public:
    TestLights(void);
    virtual ~TestLights(void);

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
    virtual void sliderMoved(OgreBites::Slider* slider);
    virtual void checkBoxToggled(OgreBites::CheckBox* box);
    
    // sdktray menu
    void createMainMenu();
    
    // setup lights
    void addLight(void);
    void removeLight(void);

private:
  ArOgre::ArModel* mModel;
  std::vector<ArOgre::ArModel*> vColum;
  
  // ground
  Ogre::Plane* mPlane;
  Ogre::Entity* mEntPlane;
  ArOgre::ArModel* mGround;
  
  // ogre lights
  std::vector<Ogre::Entity*> vEntLight;
  std::vector<Ogre::SceneNode*> vNodeLight;
  std::vector<Ogre::AnimationState*> vAnimLight;
};

#endif // #ifndef _TESTLIGHTS_H__
