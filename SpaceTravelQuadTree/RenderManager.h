#include <malloc.h>
#include <cstdlib>
#include <ctime> 
#include <cmath>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <list>
#include <vector>
#include "intersectionDetectionRoutines.h"
#include "Asteroid.h"
#include "vec.h"

using namespace std;

#pragma comment ( lib, "opengl32.lib" )
#pragma comment ( lib, "glew32.lib" )
#pragma comment ( lib, "glfw3.lib" )


#define ROWS 100  // Number of rows of asteroids.
#define COLUMNS 100 // Number of columns of asteroids.
#define FILL_PROBABILITY 100 // Percentage probability that a particular row-column slot will be 
// filled with an asteroid. It should be an integer between 0 and 100.
#define WINDOW_X  800
#define WINDOW_Y  800


// vertex counting for where everything goes in the global array
// fixed number of vertices for cone and sphere
#define CONE_VERTEX_COUNT 12
#define LINE_VERTEX_COUNT 2

static float cullingRadiusMiniMap = 160;
static float cullingRadius = 160;

// the asteroids and quad tree from the initial program
extern Asteroid **arrayAsteroids; // Global array of asteroids.


class RenderManager
{

private:

	// Size of the OpenGL window.
	int width, height; 

	// initial indices where data starts getting drawn for different data types
	int cone_index = 0;
	int line_index = cone_index + CONE_VERTEX_COUNT;
	int sphere_index = line_index + LINE_VERTEX_COUNT;

	int isFrustumCulled;

	vec3 points[CONE_VERTEX_COUNT + LINE_VERTEX_COUNT + SPHERE_VERTEX_COUNT*ROWS*COLUMNS];
	RenderManager();
	RenderManager(RenderManager const&) {};
	void operator=(RenderManager const&) {};

	//TODO -> Vita PORT!
	// set up the window
	GLFWwindow* window;
	//shader stuff
	GLuint  myShaderProgram;
	GLuint	myBuffer;

public:
	static RenderManager& getInstance()
	{
		static RenderManager instance;
		return instance;
	}
	~RenderManager();
	void vertexSetup();
	void CreateCone(const vec3 &d, const vec3 &a, const float h, const float rd, const int n, const int offset);
	void CreateSphere(double R, double H, double K, double Z, int offset);
	void drawAsteroid(Asteroid * asteroid);
	void drawAsteroidField(const int drawingRadius, const float xVal, const float zVal);
	void drawScene(const float xVal,const float zVal,const float angle);
	void renderManagerInit();
	bool initWindow();
	int getFrustumCulled() { return isFrustumCulled; }
	void setFrustumCulled(int status){ isFrustumCulled = status; }


	//TODO -> Vita PORT!
	static void resize(GLFWwindow* window, int w, int h);
	GLFWwindow * getWindow() { return window; }
	void swapBuffers(){ glfwSwapBuffers(getWindow()); }
	void terminate(){ glfwTerminate(); }
	bool windowShouldClose() { return glfwWindowShouldClose(getWindow()); }

};


