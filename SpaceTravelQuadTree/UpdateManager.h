#pragma once
#include "InputManager.h"

//Given the input handles the screen update logic
class UpdateManager
{
private:
	UpdateManager();
	UpdateManager(UpdateManager const&) {};
	void operator=(UpdateManager const&) {};
	float tempxVal, tempzVal, tempAngle;
	float xVal, zVal, angle;

public:
	static UpdateManager& getInstance()
	{
		static UpdateManager instance;
		return instance;
	}
	~UpdateManager();

	void inputHandle();
	void CollisionDetectionOnInput();
	float getXVal(){ return xVal; }
	float getZVal(){ return zVal; }
	float getAngle(){ return angle; }

	
};

