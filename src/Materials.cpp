#include "Materials.h"




Materials::Materials() {

}
Materials::~Materials() {

}
void Materials::init(std::shared_ptr<Program>& prog) {
	this->prog = prog;
}
void Materials::addMaterial(std::vector<float> ka, std::vector<float> kd, std::vector<float> ks, float s){
	this->ka.push_back(ka);
	this->kd.push_back(kd);
	this->ks.push_back(ks);
	this->s = s;
}
void Materials::toggle() {
	state = (state + 1) % ka.size();
}
void Materials::show() {
	glUniform3f(prog->getUniform("ka"), ka[state][0], ka[state][1], ka[state][2]);
	glUniform3f(prog->getUniform("kd"), kd[state][0], kd[state][1], kd[state][2]);
	glUniform3f(prog->getUniform("ks"), ks[state][0], ks[state][1], ks[state][2]);
	glUniform1f(prog->getUniform("s"), s);
}
