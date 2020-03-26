#include "Lights.h"

Lights::Lights(){
	state = 0;
	x = 0;
	y = 0;
	z = 0;
	steps = 0.3;
}
Lights::~Lights(){

}
void Lights::addLight(std::vector<float> light, std::string name, std::string intense) {
	all_lights.push_back(light);
	lights_name.push_back(name);
	intense_name.push_back(intense);
}
void Lights::init(std::shared_ptr<Program>& prog) {
	this->prog = prog;
}
void Lights::toggle() {
	state = (state + 1) % all_lights.size();
}
void Lights::show() {
	for (int i = 0; i < all_lights.size(); i++) {
		glUniform3f(prog->getUniform(lights_name[i]), all_lights[i][0], all_lights[i][1], all_lights[i][2]);
		glUniform1f(prog->getUniform(intense_name[i]), all_lights[i][3]);
	}
	
}
void Lights::incrX() {
	all_lights[state][0] += steps;
}
void Lights::decrX() {
	all_lights[state][0] -= steps;
}
void Lights::incrY() {
	all_lights[state][1] += steps;
}
void Lights::decrY() {
	all_lights[state][1] -= steps;
}
void Lights::incrZ() {
	all_lights[state][2] += steps;
}
void Lights::decrZ() {
	all_lights[state][2] -= steps;
}