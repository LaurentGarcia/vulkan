/*
 * vKwindow.cpp
 *
 *  Created on: Nov 8, 2016
 *      Author: lcarro
 */

#include "vKwindow.h"


vKwindow::vKwindow() {
	// TODO Auto-generated constructor stub
}

vKwindow::~vKwindow() {
	// TODO Auto-generated destructor stub
}

GLFWwindow* vKwindow::getWindow(){

	return this->window;

}

bool vKwindow::initWindow(int witdh,int height,char* windowName){

	//Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);

	// Creating window
	this->window = glfwCreateWindow(witdh,height,windowName,nullptr,nullptr);

	// Init Surface

	return true; //ToDo> error management
}

void vKwindow::createSurface(){

};

