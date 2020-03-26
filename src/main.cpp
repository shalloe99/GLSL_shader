#include <cassert>
#include <cstring>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "GLSL.h"
#include "MatrixStack.h"
#include "Program.h"
#include "Shape.h"

#include "Materials.h"
#include "Lights.h"

using namespace std;

GLFWwindow *window; // Main application window
string RESOURCE_DIR = "./"; // Where the resources are loaded from

shared_ptr<Camera> camera;
shared_ptr<Program> prog;
shared_ptr<Program> aprog;
shared_ptr<Shape> shape;
Materials material;
Lights light;
bool shader_toggle;
bool keyToggles[256] = {false}; // only for English keyboards!

// This function is called when a GLFW error occurs
static void error_callback(int error, const char *description)
{
	cerr << description << endl;
}

// This function is called when a key is pressed
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

// This function is called when the mouse is clicked
static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	// Get the current mouse position.
	double xmouse, ymouse;
	glfwGetCursorPos(window, &xmouse, &ymouse);
	// Get current window size.
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	if(action == GLFW_PRESS) {
		bool shift = (mods & GLFW_MOD_SHIFT) != 0;
		bool ctrl  = (mods & GLFW_MOD_CONTROL) != 0;
		bool alt   = (mods & GLFW_MOD_ALT) != 0;
		camera->mouseClicked((float)xmouse, (float)ymouse, shift, ctrl, alt);
	}
}

// This function is called when the mouse moves
static void cursor_position_callback(GLFWwindow* window, double xmouse, double ymouse)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if(state == GLFW_PRESS) {
		camera->mouseMoved((float)xmouse, (float)ymouse);
	}
}

static void char_callback(GLFWwindow *window, unsigned int key)
{
	keyToggles[key] = !keyToggles[key];
	if (key == 120) { //x
		light.incrX();
	}
	if (key == 88) { //X
		light.decrX();
	}
	if (key == 121) { //y
		light.incrY();
	}
	if (key == 89) { //Y
		light.decrY();
	}
	if (key == 122) { //z
		light.incrZ();
	}
	if (key == 90) { //Z
		light.decrZ();
	}
	if (key == 108 || key == 76) { //l or L
		light.toggle();
	}

	if (key == 115 || key == 83) { //s or S
		shader_toggle = !shader_toggle;
	}
	if (key == 109 || key == 77) { //m or M
		material.toggle();
	}
}

// If the window is resized, capture the new size and reset the viewport
static void resize_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// This function is called once to initialize the scene and OpenGL
static void init()
{
	// Initialize time.
	glfwSetTime(0.0);
	
	// Set background color.
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Enable z-buffer test.
	glEnable(GL_DEPTH_TEST);

	prog = make_shared<Program>();
	prog->setShaderNames(RESOURCE_DIR + "vert.glsl", RESOURCE_DIR + "frag.glsl");
	prog->setVerbose(true);
	prog->init();
	prog->addAttribute("aPos");
	prog->addAttribute("aNor");
	prog->addUniform("MV");
	prog->addUniform("P");
	prog->addUniform("lightPos");
	prog->addUniform("alightPos");
	prog->addUniform("lightInt");
	prog->addUniform("alightInt");
	prog->addUniform("ka");
	prog->addUniform("kd");
	prog->addUniform("ks");
	prog->addUniform("s");
	prog->setVerbose(false);
	
	aprog = make_shared<Program>();
	aprog->setShaderNames(RESOURCE_DIR + "silhouette_vert.glsl", RESOURCE_DIR + "silhouette_frag.glsl");
	aprog->setVerbose(true);
	aprog->init();
	aprog->addAttribute("aPos");
	aprog->addAttribute("aNor");
	aprog->addUniform("MV");
	aprog->addUniform("P");
	aprog->addUniform("lightPos");
	aprog->addUniform("alightPos");
	aprog->addUniform("lightInt");
	aprog->addUniform("alightInt");
	aprog->addUniform("ka");
	aprog->addUniform("kd");
	aprog->addUniform("ks");
	aprog->addUniform("s");
	aprog->setVerbose(false);

	camera = make_shared<Camera>();
	camera->setInitDistance(2.0f);
	
	shape = make_shared<Shape>();
	shape->loadMesh(RESOURCE_DIR + "bunny.obj");
	//shape->loadMesh(RESOURCE_DIR + "cube.obj"); // different test sample
	shape->fitToUnitBox();
	shape->init();
	
	shader_toggle = true;

	// Light Initilization
	light.init(prog);
	std::vector<float> first = { 1.0f,1.0f,1.0f,0.8 };
	std::vector<float> second = { -1.0f,1.0f,1.0f,0.2 };
	string first_name = "lightPos";
	string second_name = "alightPos";
	string first_intense = "lightInt";
	string second_intense = "alightInt";
	light.addLight(first, first_name, first_intense);
	light.addLight(second, second_name, second_intense);

	// Materials Initialization
	material.init(prog);
	vector<float> pinkka = { 0.2f, 0.2f, 0.2f };
	vector<float> pinkkd = { 0.8f, 0.7f, 0.7f };
	vector<float> pinkks = { 1.0f, 0.9f, 0.8f };
	float pinks = 200.0f;
	vector<float> blueka = { 0.2f, 0.2f, 0.2f };
	vector<float> bluekd = { 0.2f, 0.2f, 0.5f };
	vector<float> blueks = { 0.2f, 1.9f, 1.8f };
	float blues = 200.0f;
	vector<float> greyka = { 0.2f, 0.2f, 0.2f };
	vector<float> greykd = { 0.3f, 0.3f, 0.4f };
	vector<float> greyks = { 0.1f, 0.1f, 0.1f };
	float greys = 200.0f;
	material.addMaterial(greyka, greykd, greyks, greys);
	material.addMaterial(blueka, bluekd, blueks, blues);
	material.addMaterial(pinkka, pinkkd, pinkks, pinks);


	GLSL::checkError(GET_FILE_LINE);
}

// This function is called every frame to draw the scene.
static void render()
{
	// Clear framebuffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(keyToggles[(unsigned)'c']) {
		glEnable(GL_CULL_FACE);
	} else {
		glDisable(GL_CULL_FACE);
	}
	if(keyToggles[(unsigned)'z']) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	
	// Get current frame buffer size.
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	camera->setAspect((float)width/(float)height);
	
	// Matrix stacks
	auto P = make_shared<MatrixStack>();
	auto MV = make_shared<MatrixStack>();
	
	// Apply camera transforms
	P->pushMatrix();
	camera->applyProjectionMatrix(P);
	MV->pushMatrix();
	camera->applyViewMatrix(MV);
	
	if (shader_toggle) {
		prog->bind();

		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
		glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));

		light.show();
		material.show();
		shape->draw(prog);
		prog->unbind();
	}
	else {
		aprog->bind();

		glUniformMatrix4fv(aprog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
		glUniformMatrix4fv(aprog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));

		shape->draw(aprog);
		aprog->unbind();
	}
	

	
	MV->popMatrix();
	P->popMatrix();
	
	GLSL::checkError(GET_FILE_LINE);
}

int main(int argc, char **argv)
{
	if(argc < 2) {
		cout << "Please specify the resource directory." << endl;
		return 0;
	}
	RESOURCE_DIR = argv[1] + string("/");

	// Set error callback.
	glfwSetErrorCallback(error_callback);
	// Initialize the library.
	if(!glfwInit()) {
		return -1;
	}
	// Create a windowed mode window and its OpenGL context.
	window = glfwCreateWindow(640, 480, "Zengxiaoran Kang", NULL, NULL);
	if(!window) {
		glfwTerminate();
		return -1;
	}
	// Make the window's context current.
	glfwMakeContextCurrent(window);
	// Initialize GLEW.
	glewExperimental = true;
	if(glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW" << endl;
		return -1;
	}
	glGetError(); // A bug in glewInit() causes an error that we can safely ignore.
	cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
	cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	GLSL::checkVersion();
	// Set vsync.
	glfwSwapInterval(1);
	// Set keyboard callback.
	glfwSetKeyCallback(window, key_callback);
	// Set char callback.
	glfwSetCharCallback(window, char_callback);
	// Set cursor position callback.
	glfwSetCursorPosCallback(window, cursor_position_callback);
	// Set mouse button callback.
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	// Set the window resize call back.
	glfwSetFramebufferSizeCallback(window, resize_callback);
	// Initialize scene.
	init();
	// Loop until the user closes the window.
	while(!glfwWindowShouldClose(window)) {
		// Render scene.
		render();
		// Swap front and back buffers.
		glfwSwapBuffers(window);
		// Poll for and process events.
		glfwPollEvents();
	}
	// Quit program.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
