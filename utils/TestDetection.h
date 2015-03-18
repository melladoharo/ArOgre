/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2014  <copyright holder> <email>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _TESTDETECTION_H__
#define _TESTDETECTION_H__

#include "PCH.h"
#include "ArBaseApplication.h"

class TestDetection : public ArBaseApplication
{
public:
    TestDetection(void);
    virtual ~TestDetection(void);

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
    virtual void sliderMoved( OgreBites::Slider* slider);
    virtual void itemSelected( OgreBites::SelectMenu* menu);
    
    // sdktray menus
    void createMainMenu();
    void createInfoMenu();
    void createSelectMenu();
    void createMarkerMenu();

private:
  // sdktray
  OgreBites::Slider* mSldRow, * mSldCol;
  
  std::vector<ArOgre::ArModel*> mMarkerModels;
  ArOgre::ArModel* mBoardModel;
  bool mSelectMarker; // true=marker, false=board
  bool mChangeModel; 
};

#endif // #ifndef _TESTDETECTION_H__
