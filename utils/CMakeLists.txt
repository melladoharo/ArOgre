#Todos los archivos del proyecto
INCLUDE_DIRECTORIES(.)
FILE(GLOB hdrs "*.h*")
FILE(GLOB srcs "*.cpp")

SET(hdrs ${hdrs})
SET(srcs ${srcs})


#Linking con el proyecto global
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src ${GNULIBS_INCLUDE_DIR})
LINK_LIBRARIES(${PROJECT_NAME} ${REQUIRED_LIBRARIES})


#---Configuracion de OGRE---

#Modulos CMAKE ogre3D
#Packages OGRE3D
# find_package(OGRE REQUIRED)
# find_package(OIS REQUIRED)
#Includes Ogre3D
include_directories ("/usr/share/OGRE/Samples/Common/include/") 
include_directories (${OGRE_INCLUDE_DIRS})
include_directories (${OIS_INCLUDE_DIRS})
include_directories (${aruco_INCLUDE_DIRS})

add_executable (TestAnimation TestAnimation.cpp)
add_executable (TestBoardControl TestBoardControl.cpp)
add_executable (TestDetection TestDetection.cpp)
add_executable (TestDisplay TestDisplay.cpp)
add_executable (TestGame TestGame.cpp)
add_executable (TestLights TestLights.cpp)
add_executable (TestPairs TestPairs.cpp)
add_executable (TestParticles TestParticles.cpp)
add_executable (TestSolarSystem TestSolarSystem.cpp)

target_link_libraries(TestAnimation ${OGRE_LIBRARY} ${OIS_LIBRARY} ${REQUIRED_LIBRARIES})
target_link_libraries(TestBoardControl ${OGRE_LIBRARY} ${OIS_LIBRARY} ${REQUIRED_LIBRARIES})
target_link_libraries(TestDetection ${OGRE_LIBRARY} ${OIS_LIBRARY} ${REQUIRED_LIBRARIES})
target_link_libraries(TestDisplay ${OGRE_LIBRARY} ${OIS_LIBRARY} ${REQUIRED_LIBRARIES})
target_link_libraries(TestGame ${OGRE_LIBRARY} ${OIS_LIBRARY} ${REQUIRED_LIBRARIES})
target_link_libraries(TestLights ${OGRE_LIBRARY} ${OIS_LIBRARY} ${REQUIRED_LIBRARIES})
target_link_libraries(TestPairs ${OGRE_LIBRARY} ${OIS_LIBRARY} ${REQUIRED_LIBRARIES})
target_link_libraries(TestParticles ${OGRE_LIBRARY} ${OIS_LIBRARY} ${REQUIRED_LIBRARIES})
target_link_libraries(TestSolarSystem ${OGRE_LIBRARY} ${OIS_LIBRARY} ${REQUIRED_LIBRARIES})

INSTALL(TARGETS TestAnimation TestBoardControl TestDetection TestDisplay TestGame TestLights TestPairs TestParticles TestSolarSystem RUNTIME DESTINATION bin)

