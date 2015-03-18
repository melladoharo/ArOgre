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

#ifndef _ARMODEL_H__
#define _ARMODEL_H__

#include "PCH.h"

namespace ArOgre
{
/** Reprensets models with ArOgre.
 * 
 *  To represent a model it is necessary to use two nodes. 
 *  A parent node associated with the marker position, orientation 
 *  and scale. The second node is a child of the first (the node marker)
 *  and represents the entity (mesh).
*/
class ArModel
{
public:
  /** Standard constructor.
   *  @param
   * 	sManager scene manager responsible for the scene.
   *  @param
   * 	nameArModel the name of ArModel. Is the name of the marker node.
   *  @param
   * 	nameNode is the name of the child node.
   *  @param
   * 	nameMesh is the name of entity.
   *  @param
   * 	idMarker the identificator of marker detected.
   * 	By defaut is -1. Indicates that no marker assigned.
   *  @param 
   * 	markerSize is the size of markers used in the scene.
  */
  ArModel(Ogre::SceneManager *sManager,
	  const Ogre::String& nameArMarker,
	  const Ogre::String& nameArModel,
	  const Ogre::String& nameMesh,
	  int idMarker,
	  float markerSize);
    
  /** Standard destructor.
  */
  virtual ~ArModel();
 
  /** Updates the position and orientation model.
   *  @param
   * 	sets visible or not the model.
   *  @param
   * 	newPos the new position Ogre::Vector3.
   *  @param
   * 	newQua the new orientation Ogre::Quaternion.
  */
  void update(bool visible, const Ogre::Vector3& newPos, const Ogre::Quaternion& newQua);

  /** Returns the id marker model.
  */
  int getIdMarker() 
  {
    return mIdMarker;
  }
  
  /** Sets the new id marker to model
  */
  void setIdMarker(int id) 
  { 
    mIdMarker=id;
  }
  
  /** Returns the parent node. 
   *  It is the node with the position, orientation and scale of the detected marker.
  */
  Ogre::SceneNode* getMarkerNode() 
  {
    return mNodeMarker;
  }
  
  /** Returns the child node. 
   *  It is the child node of marker node.
  */
  Ogre::SceneNode* getModelNode() 
  {
    return mNodeModel;
  }
  
  /** Returns the entity of the child node.
  */
  Ogre::Entity* getEntity() 
  {
    return mEntModel;
  }
  
  /** Sets de visibility of the parent node. 
   *  All the attached objects change visibility too.
  */
  void setVisible(bool visible) 
  {
    mNodeMarker->setVisible(visible);
  }
  
  /** Returns the scale vector to adjust the size of the model to the size of the marker.
  */
  static Ogre::Vector3 scaleToMarkerSize(Ogre::SceneNode* node, float sizeMarker);
  
  /** Destroy all the childs attached to the ArModel.
  */
  static void destroyAllAttachedMovableObjects(Ogre::SceneNode* sNode);
  
private:
  /// Scene manager responsible for the scene.
  Ogre::SceneManager* mSceneMgr;
  
  /// Ogre scene node marker (position and rotation marker)
  Ogre::SceneNode* mNodeMarker;
  Ogre::String mNameArMarker;
  
  /// Ogre scene node model (mesh entity)
  Ogre::Entity* mEntModel;
  Ogre::SceneNode* mNodeModel;
  Ogre::String mNameArModel;
  
  /// ArUco markers properties
  int mIdMarker;
  Ogre::Real mMarkerSize; 
};
} // namespace ArOgre

#endif // ifndef _ARMODEL_H__
