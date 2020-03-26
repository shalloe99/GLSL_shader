#pragma once
#ifndef __Lights__
#define __Lights__
#include <cassert>
#include <cstring>
#include <cmath>
#include <iostream> 
#include <vector> 

#include "Program.h"
class Lights
{
public:
	Lights();
	virtual ~Lights();
	void init(std::shared_ptr<Program>& prog);
	void addLight(std::vector<float> light, std::string name,std::string intense);
	void toggle();
	void show();
	void incrX();
	void decrX();
	void incrY();
	void decrY();
	void incrZ();
	void decrZ();
private:
	std::shared_ptr<Program> prog;
	// all_lights format <xpos, ypos, zpos, intensity>
	std::vector<std::vector<float>> all_lights;
	std::vector<std::string> lights_name;
	std::vector<std::string> intense_name;
	int state;
	float x;
	float y;
	float z;
	float steps;
};

#endif
