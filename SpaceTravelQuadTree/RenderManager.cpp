#include "RenderManager.h"

//Getting InitShader from InitShader.cpp
extern GLuint InitShader(const char* vShaderFile, const char* fShaderFile);

#pragma region Auxiliary Functions

//ASSIGNMENT01-DISTANCE FUNCTION
float Distance2D(float dX0, float dY0, float dX1, float dY1)
{
	return sqrt((dX1 - dX0)*(dX1 - dX0) + (dY1 - dY0)*(dY1 - dY0));
}

// function obtained from tutorial at:
// http://www.freemancw.com/2012/06/opengl-cone-function/
// used in drawing a cone
vec3 perp(const vec3 &v) {
	float min = fabs(v.x);
	vec3 cardinalAxis(1, 0, 0);

	if (fabs(v.y) < min) {
		min = fabs(v.y);
		cardinalAxis = vec3(0, 1, 0);
	}

	if (fabs(v.z) < min) {
		cardinalAxis = vec3(0, 0, 1);
	}

	return cross(v, cardinalAxis);
}


#pragma endregion

//TODO -> Vita PORT!
// function taken from glu
void lookAt(float eyex, float eyey, float eyez, float centerx,
	float centery, float centerz, float upx, float upy, float upz)
{
	int i;
	vec3 forward, side, up;
	float m[4][4];

	// create identity matrix
	for (int i = 0; i < 4; i++) {
		m[i][0] = 0;
		m[i][1] = 0;
		m[i][2] = 0;
		m[i][3] = 0;
	}

	m[0][0] = 1.0f;
	m[1][1] = 1.0f;
	m[2][2] = 1.0f;
	m[3][3] = 1.0f;

	// glu code for lookat
	forward.x = centerx - eyex;
	forward.y = centery - eyey;
	forward.z = centerz - eyez;

	up.x = upx;
	up.y = upy;
	up.z = upz;

	forward = normalize(forward);

	// Side = forward x up 
	side = cross(forward, up);
	side = normalize(side);

	// Recompute up as: up = side x forward 
	up = cross(side, forward);

	m[0][0] = side[0];
	m[1][0] = side[1];
	m[2][0] = side[2];

	m[0][1] = up[0];
	m[1][1] = up[1];
	m[2][1] = up[2];

	m[0][2] = -forward[0];
	m[1][2] = -forward[1];
	m[2][2] = -forward[2];

	glMultMatrixf((const GLfloat *)m[0]);
	glTranslated(-eyex, -eyey, -eyez);
}

#pragma region Create Shapes

// function derived from tutorial at:
// http://www.freemancw.com/2012/06/opengl-cone-function/
void RenderManager::CreateCone(const vec3 &d, const vec3 &a,
	const float h, const float rd, const int n, int offset) {
	vec3 c;
	c.x = a.x + (-d.x * h);
	c.y = a.y + (-d.y * h);
	c.z = a.z + (-d.z * h);
	vec3 e0 = perp(d);
	vec3 e1 = cross(e0, d);
	float angInc = 360.0 / n * (PI / 180.0f);

	// calculate points around directrix
	vector <vec3> pts;
	for (int i = 0; i < n; ++i) {
		float rad = angInc * i;
		vec3 p = c + (((e0 * cos(rad)) + (e1 * sin(rad))) * rd);
		pts.push_back(p);
	}

	// draw cone top
	int i = 0;
	int o = offset + i;
	points[o].x = a.x;
	points[o].y = a.y;
	points[o].z = a.z;


	for (i = 1; i < n + 1; ++i) {
		o = i + offset;
		points[o].x = pts[i - 1].x;
		points[o].y = pts[i - 1].y;
		points[o].z = pts[i - 1].z;
	}

	o = i + offset;
	points[o].x = pts[0].x;
	points[o].y = pts[0].y;
	points[o].z = pts[0].z;

	// original tutorial has cone bottom
	// not necessary when cone is a spaceship!
}

// function derived from tutorial at:
// http://www.swiftless.com/tutorials/opengl/sphere.html
void RenderManager::CreateSphere(double R, double H, double K, double Z, int offset) {
	int n;
	double a;
	double b;
	n = 0;
	const int space = 30;

	// draw the front half
	for (b = 0; b <= 90 - space; b += space){
		for (a = 0; a <= 360 - space; a += space){

			points[n + offset].x = R * sin((a) / 180 * PI) * sin((b) / 180 * PI) - H;
			points[n + offset].y = R * cos((a) / 180 * PI) * sin((b) / 180 * PI) + K;
			points[n + offset].z = R * cos((b) / 180 * PI) - Z;
			n++;

			points[n + offset].x = R * sin((a) / 180 * PI) * sin((b + space) / 180 * PI) - H;
			points[n + offset].y = R * cos((a) / 180 * PI) * sin((b + space) / 180 * PI) + K;
			points[n + offset].z = R * cos((b + space) / 180 * PI) - Z;
			n++;
			points[n + offset].x = R * sin((a + space) / 180 * PI) * sin((b) / 180 * PI) - H;
			points[n + offset].y = R * cos((a + space) / 180 * PI) * sin((b) / 180 * PI) + K;
			points[n + offset].z = R * cos((b) / 180 * PI) - Z;
			n++;
			points[n + offset].x = R * sin((a + space) / 180 * PI) * sin((b + space) / 180 * PI) - H;
			points[n + offset].y = R * cos((a + space) / 180 * PI) * sin((b + space) / 180 * PI) + K;
			points[n + offset].z = R * cos((b + space) / 180 * PI) - Z;
			n++;
		}
	}

	// draw the back half
	for (b = 0; b <= 90 - space; b += space){
		for (a = 0; a <= 360 - space; a += space){

			points[n + offset].x = R * sin((a) / 180 * PI) * sin((b) / 180 * PI) - H;
			points[n + offset].y = R * cos((a) / 180 * PI) * sin((b) / 180 * PI) + K;
			points[n + offset].z = -1 * (R * cos((b) / 180 * PI)) - Z;
			n++;

			points[n + offset].x = R * sin((a) / 180 * PI) * sin((b + space) / 180 * PI) - H;
			points[n + offset].y = R * cos((a) / 180 * PI) * sin((b + space) / 180 * PI) + K;
			points[n + offset].z = -1 * (R * cos((b + space) / 180 * PI)) - Z;
			n++;
			points[n + offset].x = R * sin((a + space) / 180 * PI) * sin((b) / 180 * PI) - H;
			points[n + offset].y = R * cos((a + space) / 180 * PI) * sin((b) / 180 * PI) + K;
			points[n + offset].z = -1 * (R * cos((b) / 180 * PI)) - Z;
			n++;
			points[n + offset].x = R * sin((a + space) / 180 * PI) * sin((b + space) / 180 * PI) - H;
			points[n + offset].y = R * cos((a + space) / 180 * PI) * sin((b + space) / 180 * PI) + K;
			points[n + offset].z = -1 * (R * cos((b + space) / 180 * PI)) - Z;
			n++;
		}
	}

}

#pragma endregion



#pragma region InitializationRoutines 
//TODO -> Vita PORT!
void RenderManager::resize(GLFWwindow* window, int w, int h) {
	// OpenGL window reshape routine.
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 250.0);
	glMatrixMode(GL_MODELVIEW);

	// Pass the size of the OpenGL window.
	getInstance().width = w;
	getInstance().height = h;
}

bool RenderManager::initWindow()
{
	
	// Initialize the library 
	if (!glfwInit())
		return false;

	// Create a windowed mode window and its OpenGL context 
	window = glfwCreateWindow(WINDOW_X, WINDOW_Y, "spaceTravelFrustumCulled.cpp", nullptr, nullptr);
	resize(window, WINDOW_X, WINDOW_Y);
	glfwSetWindowSizeCallback(window, resize);


	//
	if (!window)
	{
		glfwTerminate();
		return false;
	}

	// Make the window's context current 
	glfwMakeContextCurrent(window);

	// Init GLEW 
	glewInit();
	return true;
}

void RenderManager::renderManagerInit()
{
	// initialize the graphics
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glViewport(0, 0, (GLsizei)WINDOW_X, (GLsizei)WINDOW_Y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 250.0);
	glMatrixMode(GL_MODELVIEW);

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object for each circle
	GLuint aBuffer;
	glGenBuffers(1, &aBuffer);
	myBuffer = aBuffer;
	glBindBuffer(GL_ARRAY_BUFFER, myBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	myShaderProgram = program;
	glUseProgram(myShaderProgram);

	// Initialize the vertex position attribute from the vertex shader
	GLuint loc = glGetAttribLocation(myShaderProgram, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Turn on wireframe mode
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);

}

// Initialization routine.
void RenderManager::vertexSetup(void)
{
	int i, j;
	float initialSize;
	// create meory for each potential asteroid
	arrayAsteroids = new Asteroid *[ROWS];
	for (int i = 0; i < ROWS; i++) {
		arrayAsteroids[i] = new Asteroid[COLUMNS];
	}

	// create the cone for a spaceship
	vec3 direction(0, 1, 0);
	vec3 apex(0, 10, 0);
	getInstance().CreateCone(direction, apex, 10, 5, 10, cone_index);

	// create where the spheres are going in the field   
	int index = sphere_index;
	// Initialize global arrayAsteroids.
	for (i = 0; i < ROWS; i++)
		for (j = 0; j < COLUMNS; j++)
			if (rand() % 100 < FILL_PROBABILITY)
				// If rand()%100 >= FILL_PROBABILITY the default constructor asteroid remains in the slot which
				// indicates that there is no asteroid there because the default's radius is 0.
			{
		// Position the asteroids depending on if there is an even or odd number of columns
		// so that the spacecraft faces the middle of the asteroid field.
		if (COLUMNS % 2) // Odd number of columns. 
		{
			arrayAsteroids[i][j] = Asteroid(30.0*(-COLUMNS / 2 + j), 0.0, -40.0 - 30.0*i, 3.0,
				rand() % 256, rand() % 256, rand() % 256);
			arrayAsteroids[i][j].setIndex(index);
			RenderManager::getInstance().CreateSphere(SPHERE_SIZE, 0, 0, 0, index);
			index += SPHERE_VERTEX_COUNT;
		}
		else // Even number of columns. 
		{
			arrayAsteroids[i][j] = Asteroid(15.0 + 30.0*(-COLUMNS / 2 + j), 0.0, -40.0 - 30.0*i, 3.0,
				rand() % 256, rand() % 256, rand() % 256);
			arrayAsteroids[i][j].setIndex(index);
			RenderManager::getInstance().CreateSphere(SPHERE_SIZE, 0, 0, 0, index);
			index += SPHERE_VERTEX_COUNT;
		}
			}

	if (ROWS <= COLUMNS) initialSize = (COLUMNS - 1)*30.0 + 6.0;
	else initialSize = (ROWS - 1)*30.0 + 6.0;
	
}


#pragma region Draw 
//ASSIGNMENT02-SELECT ASTEROIDS TO DRAW
void RenderManager::drawAsteroidField(const int drawingRadius, const float xVal, const float zVal)
{
	int i, j;

	if (!getFrustumCulled())
	{
		// Draw all the asteroids in arrayAsteroids.
		for (i = 0; i < ROWS; i++)
			for (j = 0; j < COLUMNS; j++)

				drawAsteroid(&arrayAsteroids[i][j]);
	}
	else
	{

		for (i = 0; i < ROWS; i++)
			for (j = 0; j < COLUMNS; j++)

			{
			//ASSIGNMENT01--Changed the culling algorithm to draw only close asteroids
			if (Distance2D(arrayAsteroids[i][j].getCenterX(),
				arrayAsteroids[i][j].getCenterZ(), xVal, zVal) < drawingRadius)

				drawAsteroid(&arrayAsteroids[i][j]);
			}

	}
}

//TODO -> Vita PORT!
// Function to draw asteroid.
void RenderManager::drawAsteroid(Asteroid * asteroid)
{
	if (asteroid->getRadius() > 0.0) // If asteroid exists.
	{
		glPushMatrix();
		glTranslatef(asteroid->getCenterX(), asteroid->getCenterY(), asteroid->getCenterZ());
		glColor3ubv(asteroid->getColorArray());

		// draw sphere
		glDrawArrays(GL_TRIANGLE_FAN, asteroid->getIndex(), SPHERE_VERTEX_COUNT);

		glPopMatrix();
	}
}

// Drawing routine.
void RenderManager::drawScene(const float xVal, const float zVal, const float angle)
{
	int i, j;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glViewport(width * 3.0 / 4.0, height * 3.0 / 4.0, width / 6.0, height / 6.0);
	glLoadIdentity();

	//ASSIGNMENT01 - MINIMAP!
	lookAt(xVal - 10 * sin((PI / 180.0) * angle),
		200.0,
		zVal - 10 * cos((PI / 180.0) * angle),
		xVal - 11 * sin((PI / 180.0) * angle),
		0.0,
		zVal - 11 * cos((PI / 180.0) * angle),
		0.0,
		1.0,
		0.0);

	//Select asteroids to be drawed on the minimap
	drawAsteroidField(cullingRadiusMiniMap, xVal, zVal);

	// off is white spaceship and on it red
	if (isFrustumCulled)
		glColor3f(1.0, 0.0, 0.0);
	else
		glColor3f(1.0, 1.0, 1.0);

	// spacecraft moves and so we translate/rotate according to the movement
	glPushMatrix();
	glTranslatef(xVal, 0, zVal);
	glRotatef(angle, 0.0, 1.0, 0.0);

	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0); // To make the spacecraft point down the $z$-axis initially.

	//ASSIGNMENT01-Draw SpaceShip
	glDrawArrays(GL_TRIANGLE_FAN, cone_index, CONE_VERTEX_COUNT);
	glPopMatrix();


	// Begin right viewport.
	glViewport(0, 0, width, height);
	glLoadIdentity();

	// Locate the camera at the tip of the cone and pointing in the direction of the cone.
	lookAt(xVal - 10 * sin((PI / 180.0) * angle),
		0.0,
		zVal - 10 * cos((PI / 180.0) * angle),
		xVal - 11 * sin((PI / 180.0) * angle),
		0.0,
		zVal - 11 * cos((PI / 180.0) * angle),
		0.0,
		1.0,
		0.0);

	//Select asteroids to be drawed
	drawAsteroidField(cullingRadius, xVal, zVal);
	// End right viewport.
}

#pragma endregion



RenderManager::RenderManager()
{
}


RenderManager::~RenderManager()
{
}



