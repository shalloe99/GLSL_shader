#pragma once
#ifndef __Materials__
#define __Materials__
#include <cassert>
#include <cstring>
#include <cmath>
#include <iostream> 
#include <vector> 

#include "Program.h"
class Materials
{
public:

	Materials();
	virtual ~Materials();
	void init(std::shared_ptr<Program>& prog);
	void addMaterial(std::vector<float> ka, std::vector<float> kd, std::vector<float> ks,float s);
	void toggle();
	void show();
private:
	std::shared_ptr<Program> prog;
	int state;
	std::vector<std::vector<float>> ka;
	std::vector<std::vector<float>> kd;
	std::vector<std::vector<float>> ks;
	float s;
};

#endif
