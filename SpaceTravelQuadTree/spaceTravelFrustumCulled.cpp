//////////////////////////////////////////////////////////////////////////////////////         
// spaceTravelFrustumCulled.cpp
//
// This program is based on spaceTravel.cpp with an added frustum culling option.
// It draws a conical spacecraft that can travel and an array of fixed spherical 
// asteroids. The view in the left viewport is from a fixed camera; the view in 
// the right viewport is from the spacecraft.There is approximate collision detection.  
// Frustum culling is implemented by means of a quadtree data structure.
// 
// COMPILE NOTE: File intersectionDetectionRoutines.cpp must be in the same folder.
// EXECUTION NOTE: If ROWS and COLUMNS are large the quadtree takes time to build so
//                 the display may take several seconds to come up.
//
// User-defined constants: 
// ROWS is the number of rows of  asteroids.
// COLUMNS is the number of columns of asteroids.
// FILL_PROBABILITY is the percentage probability that a particular row-column slot
// will be filled with an asteroid.
//
// Interaction:
// Press the left/right arrow keys to turn the craft.
// Press the up/down arrow keys to move the craft.
// Press space to toggle between frustum culling enabled and disabled.
// 
// Sumanta Guha.
////////////////////////////////////////////////////////////////////////////////////// 

#include "InputManager.h"
#include "UpdateManager.h"

// the asteroids from the initial program
Asteroid **arrayAsteroids; // Global array of asteroids.

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	cout << "Interaction:" << endl;
	cout << "Press the left/right arrow keys to turn the craft." << endl
		<< "Press the up/down arrow keys to move the craft." << endl
		<< "Press space to toggle between frustum culling enabled and disabled." << endl;
}


// Main routine.
int main(int argc, char **argv)
{
	
	srand((unsigned)time(0));
	printInteraction();

	RenderManager::getInstance().initWindow();
	// init the graphics and rest of the app
	RenderManager::getInstance().vertexSetup();

	InputManager::getInstance().init();

	RenderManager::getInstance().renderManagerInit();

	// run!
	while (!RenderManager::getInstance().windowShouldClose())
	{
		
		UpdateManager::getInstance().inputHandle();	

		RenderManager::getInstance().drawScene(UpdateManager::getInstance().getXVal(), UpdateManager::getInstance().getZVal(), UpdateManager::getInstance().getAngle());
		RenderManager::getInstance().swapBuffers();

		// Poll for and process events 
		InputManager::getInstance().pollEvents();   
	}

	RenderManager::getInstance().terminate();

	return 0;

}

