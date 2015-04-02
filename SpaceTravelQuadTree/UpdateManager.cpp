#include "UpdateManager.h"

float angle = 0; // Angle of the spacecraft.
float xVal = 0, zVal = 0; // Co-ordinates of the spacecraft.
int isCollision = 0; // Is there collision between the spacecraft and an asteroid?

void UpdateManager::inputHandle(){
	//InputManager::getInstance().setHasInput(false);

	tempxVal = xVal, tempzVal = zVal, tempAngle = angle;

	if (InputManager::getInstance().getInputState(InputManager::CommandKey::ESCAPE)==InputManager::ActionState::PRESSED)
		exit(0);
	if (InputManager::getInstance().getInputState(InputManager::CommandKey::SPACE) == InputManager::ActionState::RELEASED)
		// only want this to get called once and so call when key
		// is released
	{
			RenderManager::getInstance().setFrustumCulled(1 - RenderManager::getInstance().getFrustumCulled());
			InputManager::getInstance().setInputState(InputManager::CommandKey::SPACE, InputManager::ActionState::INACTIVE);
	}
	if (InputManager::getInstance().getInputState(InputManager::CommandKey::LEFT) == InputManager::ActionState::PRESSED)
		tempAngle = tempAngle + 1.0f;
	
	if (InputManager::getInstance().getInputState(InputManager::CommandKey::RIGHT) == InputManager::ActionState::PRESSED)

		tempAngle = tempAngle - 1.0f;

	if (InputManager::getInstance().getInputState(InputManager::CommandKey::UP) == InputManager::ActionState::PRESSED)
	{
		tempxVal = tempxVal - sin(angle * PI / 180.0f);
		tempzVal = tempzVal - cos(angle * PI / 180.0f);
	}
	if (InputManager::getInstance().getInputState(InputManager::CommandKey::DOWN) == InputManager::ActionState::PRESSED)
	{
		tempxVal = tempxVal + sin(angle * PI / 180.0f);
		tempzVal = tempzVal + cos(angle * PI / 180.0f);
	}

	// Angle correction.
	if (tempAngle > 360.0f) tempAngle -= 360.0f;
	if (tempAngle < 0.0f) tempAngle += 360.0f;

	CollisionDetectionOnInput();
}

// Function to check if two spheres centered at (x1,y1,z1) and (x2,y2,z2) with
// radius r1 and r2 intersect.
int checkSpheresIntersection(float x1, float y1, float z1, float r1,
	float x2, float y2, float z2, float r2)
{
	return ((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2) <= (r1 + r2)*(r1 + r2));
}

// Function to check if the spacecraft collides with an asteroid when the center of the base
// of the craft is at (x, 0, z) and it is aligned at an angle a to to the -z direction.
// Collision detection is approximate as instead of the spacecraft we use a bounding sphere.
int asteroidCraftCollision(float x, float z, float a)
{
	int i, j;

	// Check for collision with each asteroid.
	for (i = 0; i < ROWS; i++)
		for (j = 0; j < COLUMNS; j++)
			if (arrayAsteroids[i][j].getRadius() > 0) // If asteroid exists.
				if (checkSpheresIntersection(x - 5.0f * sin((PI / 180.0f) * a), 0.0f,
					z - 5 * cos((PI / 180.0f) * a), 7.072f,
					arrayAsteroids[i][j].getCenterX(), arrayAsteroids[i][j].getCenterY(),
					arrayAsteroids[i][j].getCenterZ(), arrayAsteroids[i][j].getRadius()))
					return 1;
	return 0;
}

void UpdateManager::CollisionDetectionOnInput(){
	// Move spacecraft to next position only if there will not be collision with an asteroid.
	if (!asteroidCraftCollision(tempxVal, tempzVal, tempAngle))
	{
		isCollision = 0;
		xVal = tempxVal;
		zVal = tempzVal;
		angle = tempAngle;
	}
	else isCollision = 1;
}



UpdateManager::UpdateManager()
{
}


UpdateManager::~UpdateManager()
{
}
