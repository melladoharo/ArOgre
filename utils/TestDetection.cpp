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

#include "PCH.h"
#include "TestDetection.h" 

//////////////////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR
//////////////////////////////////////////////////////////////////////////////////////////////
TestDetection::TestDetection(void)
  : mBoardModel(0),
    mSelectMarker(false)
{
}
//////////////////////////////////////////////////////////////////////////////////////////////
TestDetection::~TestDetection(void)
{
  mTrayMgr->destroyAllWidgets();
  
  for(unsigned int i=0; i<mMarkerModels.size(); ++i)
    delete mMarkerModels[i];
   
  if(mBoardModel) delete mBoardModel;
}




//////////////////////////////////////////////////////////////////////////////////////////////
// CREATE SCENE
//////////////////////////////////////////////////////////////////////////////////////////////
void TestDetection::createScene(void)
{
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.8,0.8,0.8));
}




//////////////////////////////////////////////////////////////////////////////////////////////
// FRAME LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
void TestDetection::createFrameListener()
{
  ArBaseApplication::createFrameListener();
    
   // Create main menu
  if(!mShowWarning)
    createMainMenu();
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestDetection::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  bool ret=ArBaseApplication::frameRenderingQueued(evt);
  
  bool detected;
  Ogre::Vector3 newPos; 
  Ogre::Quaternion newQua;
  
  // update the markers models
  for(unsigned int i=0; i<mMarkerModels.size(); ++i)
  {
    detected=mDetector->detectIdMarker(mMarkerModels[i]->getIdMarker(), newPos, newQua);
    mMarkerModels[i]->update(detected, newPos, newQua);  
  }
  
  // update the board model
  if(mBoardModel)
  {
    detected=mDetector->detectBoard(newPos, newQua);
    mBoardModel->update(detected, newPos, newQua);
  }
  
  return ret;
}




//////////////////////////////////////////////////////////////////////////////////////////////
// KEY LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestDetection::keyPressed ( const OIS::KeyEvent& arg )
{
  if(arg.key==OIS::KC_F1)
  {
    mTrayMgr->destroyAllWidgets();
    createMainMenu();
  }
  return ArBaseApplication::keyPressed ( arg );
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestDetection::keyReleased ( const OIS::KeyEvent& arg )
{
    return ArBaseApplication::keyReleased ( arg );
}




//////////////////////////////////////////////////////////////////////////////////////////////
// MOUSE LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestDetection::mouseMoved(const OIS::MouseEvent &arg)
{
  return ArBaseApplication::mouseMoved(arg);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestDetection::mousePressed(const OIS::MouseEvent &arg,OIS::MouseButtonID id)
{
  return ArBaseApplication::mousePressed(arg, id);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool TestDetection::mouseReleased(const OIS::MouseEvent &arg,OIS::MouseButtonID id)
{
  return ArBaseApplication::mouseReleased(arg, id);
}




//////////////////////////////////////////////////////////////////////////////////////////////
// SDKTRAY LISTENER
//////////////////////////////////////////////////////////////////////////////////////////////
void TestDetection::buttonHit ( OgreBites::Button* button )
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
  else if(button->getName()=="bttMdlMarker")
  {
    mChangeModel=false;
    mSelectMarker=true;
    mTrayMgr->destroyAllWidgets();
    createSelectMenu();
  }
  else if(button->getName()=="bttMdlBoard")
  {
    mSelectMarker=false;
    mTrayMgr->destroyAllWidgets();
    createSelectMenu();
  }
  else if(button->getName()=="bttRemove")
  {
    for(unsigned int i=0; i<mMarkerModels.size(); ++i)
    delete mMarkerModels[i];
    mMarkerModels.clear();
  
    if(mBoardModel) 
      delete mBoardModel;
    mBoardModel=0;
  }
  else if(button->getName()=="bttAcceptSM")
  {
    mTrayMgr->destroyAllWidgets();
    if(mSelectMarker)
      createMarkerMenu();
    else
      createMainMenu();
  }
  else if(button->getName()=="bttAccept")
  {
    mTrayMgr->destroyAllWidgets();
    createMainMenu();
  }
  else if(button->getName()=="bttInfo")
  {
    mTrayMgr->destroyAllWidgets();
    createInfoMenu();
  }
  else if(button->getName()=="bttExitInfo")
  {
    mTrayMgr->destroyAllWidgets();
    createMainMenu();
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void TestDetection::sliderMoved ( OgreBites::Slider* slider )
{
  if(slider->getName()=="sldRow" || slider->getName()=="sldCol")
  {
    int newId=mDetector->getBoardIdMarker(mSldRow->getValue()-1, mSldCol->getValue()-1);
    mMarkerModels.back()->setIdMarker(newId);  
  }
}
void TestDetection::itemSelected ( OgreBites::SelectMenu* menu )
{
  Ogre::String nameMesh=menu->getSelectedItem();
  int idMarker=mDetector->getBoardIdMarker(0,0);
  float markerSize=mDetector->getMarkerSize();
  
  if(menu->getSelectionIndex()>0)
  {
    if(mSelectMarker)
    {
      if(mChangeModel)
      {
	delete mMarkerModels.back();
	mMarkerModels.pop_back();
      }
    
      ArOgre::ArModel* newModel=0;
      Ogre::String nameMarker="marker_"+Ogre::StringConverter::toString(mMarkerModels.size()+1);
      Ogre::String nameModel="model_"+Ogre::StringConverter::toString(mMarkerModels.size()+1);  
      newModel=new ArOgre::ArModel(mSceneMgr, nameMarker, nameModel, nameMesh, idMarker, markerSize);
      mMarkerModels.push_back(newModel);
      mChangeModel=true;
    }
    else
    {
      if(mBoardModel)
	delete mBoardModel;
    
      mBoardModel=new ArOgre::ArModel(mSceneMgr, "board_1", "board_Model_1", nameMesh,-1,markerSize);
      //mBoardModel->getModelNode()->scale(1.25f, 1.25f, 1.25f);
    }
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void TestDetection::createMainMenu()
{
  mTrayMgr->showCursor();
  mTrayMgr->createLabel(OgreBites::TL_BOTTOMLEFT, "lbltd", "Test Detection", 150.0f);
  mTrayMgr->createButton(OgreBites::TL_BOTTOMLEFT, "bttMdlMarker", "Model Marker", 150.0f);
  mTrayMgr->createButton(OgreBites::TL_BOTTOMLEFT, "bttMdlBoard", "Model Board", 150.0f);
  mTrayMgr->createButton(OgreBites::TL_BOTTOMLEFT, "bttRemove", "Remove Models", 150.0f);
  mTrayMgr->createSeparator(OgreBites::TL_BOTTOMLEFT, "mmSpt", 150.0f);
  mTrayMgr->createButton(OgreBites::TL_BOTTOMLEFT, "bttExit", "Exit", 150.0f);
}
void TestDetection::createInfoMenu()
{
  OgreBites::TextBox* tb=mTrayMgr->createTextBox(
    OgreBites::TL_CENTER, "tbInfo", "Test Detection", 500, 300);
  tb->setText("TEST DETECTION\n\n\n"
    "_-> Press F1 to display the main menu.");
  
  mTrayMgr->createButton(OgreBites::TL_CENTER, "bttExitInfo", "Back", 180.0f);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void TestDetection::createSelectMenu()
{
  mTrayMgr->showCursor();
  mTrayMgr->createLabel(OgreBites::TL_BOTTOM, "lbltd", "Select 3d model", 270.0f);
  OgreBites::SelectMenu* smModels=0;
  smModels=mTrayMgr->createLongSelectMenu(OgreBites::TL_BOTTOM, "smModels", "Models", 200.0f, 10);
  mTrayMgr->createButton(OgreBites::TL_BOTTOM, "bttAcceptSM", "Accept", 270.0f);
  smModels->addItem("Select a model");
  
  Ogre::StringVectorPtr pList;
  pList=Ogre::ResourceGroupManager::getSingleton().findResourceNames("General", "*.mesh");
  for(unsigned int i = 0;i < pList->size();i++)
  {
    Ogre::String addstr = (*pList)[i];
    smModels->addItem(addstr.c_str());
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void TestDetection::createMarkerMenu()
{
  int maxRows=mDetector->getBoardHeight();
  int maxCols=mDetector->getBoardWidth();
  mTrayMgr->createLabel(OgreBites::TL_BOTTOM, "lbltd", "Select marker", 300.0f);
  mSldRow=mTrayMgr->createLongSlider(OgreBites::TL_BOTTOM, "sldRow", "ROW", 200, 50, 1, maxRows, maxRows);
  mSldCol=mTrayMgr->createLongSlider(OgreBites::TL_BOTTOM, "sldCol", "COL", 200, 50, 1, maxCols, maxCols);
  mTrayMgr->createButton(OgreBites::TL_BOTTOM, "bttAccept", "Accept", 300.0f);
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
        TestDetection app;

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

