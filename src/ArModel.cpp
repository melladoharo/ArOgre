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
#include "ArModel.h"

namespace ArOgre
{
ArModel::ArModel(Ogre::SceneManager *sManager, 
	    const Ogre::String& nameArMarker, 
	    const Ogre::String& nameArModel, 
	    const Ogre::String& nameMesh, 
	    int idMarker,
	    float markerSize)
  : mSceneMgr(sManager),
    mNodeMarker(0),
    mNameArMarker(nameArMarker),
    mEntModel(0),
    mNodeModel(0),
    mNameArModel(nameArModel),
    mIdMarker(idMarker),
    mMarkerSize(markerSize)
{
  // (marker position and orientation) sceneNode
  mNodeMarker=mSceneMgr->getRootSceneNode()->createChildSceneNode(mNameArMarker);
  
  // create entity and sceneNode model
  mEntModel=mSceneMgr->createEntity(nameMesh);
  mEntModel->setCastShadows(true);
  mNodeModel=mNodeMarker->createChildSceneNode(mNameArModel);
  mNodeModel->attachObject(mEntModel);
  
  // scale node to marker size
  mNodeModel->setScale(scaleToMarkerSize(mNodeModel, mMarkerSize));
}
//////////////////////////////////////////////////////////////////////////////////////////////
ArModel::~ArModel()
{
  // remove all scenenode and entity
  destroyAllAttachedMovableObjects(mNodeMarker);
  mNodeMarker->removeAndDestroyAllChildren();
  mSceneMgr->destroySceneNode(mNodeMarker);
  
  // show info
  Ogre::LogManager::getSingletonPtr()->logMessage(
    "[ArModel] remove model: \""+mNameArMarker+"\" Id: "+
    Ogre::StringConverter::toString(mIdMarker)+".");
}
//////////////////////////////////////////////////////////////////////////////////////////////
void ArModel::update ( bool visible, const Ogre::Vector3& newPos, const Ogre::Quaternion& newQua )
{
  mNodeMarker->setVisible(visible);	// makes visible or not
  mNodeMarker->setPosition(newPos);	// updates position
  mNodeMarker->setOrientation(newQua);	// updates orientation
}





//////////////////////////////////////////////////////////////////////////////////////////////
Ogre::Vector3 ArModel::scaleToMarkerSize ( Ogre::SceneNode* node, float sizeMarker )
{
  // needs to work properly that Box.X > 1.0f
  node->_update(true, true);
  Ogre::AxisAlignedBox aaBox(Ogre::AxisAlignedBox(node->_getWorldAABB()));

  // new scale
  Ogre::Vector3 scale(aaBox.getSize());
  
  // X new size box = sizeMarker
  if (aaBox.getSize().x>1.0f)	scale.x=sizeMarker/aaBox.getSize().x;
  else				scale.x=aaBox.getSize().x*0.01f;
  
  // calculate Y new size box
  if(aaBox.getSize().y>1.0f)
  {
    float tamBoxTarget=aaBox.getSize().x/sizeMarker;
    tamBoxTarget=aaBox.getSize().y/tamBoxTarget;
    scale.y=tamBoxTarget/aaBox.getSize().y;
  }
  
  // calculate Z new size box
  if(aaBox.getSize().z>1.0f)
  {
    float tamBoxTarget=aaBox.getSize().x/sizeMarker;
    tamBoxTarget=aaBox.getSize().z/tamBoxTarget;
    scale.z=tamBoxTarget/aaBox.getSize().z;
  }
  
  return scale;
}
//////////////////////////////////////////////////////////////////////////////////////////////
void ArModel::destroyAllAttachedMovableObjects ( Ogre::SceneNode* sNode )
{
  if(!sNode ) return;
  
  // Destroy all the attached objects
  Ogre::SceneNode::ObjectIterator itObject=sNode->getAttachedObjectIterator();
  while(itObject.hasMoreElements())
  {
    Ogre::MovableObject* pObject=static_cast<Ogre::MovableObject*>(itObject.getNext());
    sNode->getCreator()->destroyMovableObject( pObject );
  }

  // Recurse to child SceneNodes
  Ogre::SceneNode::ChildNodeIterator itChild=sNode->getChildIterator();
  while(itChild.hasMoreElements())
  {
    Ogre::SceneNode* pChildNode=static_cast<Ogre::SceneNode*>(itChild.getNext());
    destroyAllAttachedMovableObjects( pChildNode );
  }
}
} // namespace ArOgre

