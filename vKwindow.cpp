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

void vKwindow::error_callback(int error, const char* description)
{
	puts(description);
}

GLFWwindow* vKwindow::getWindow(){

	return this->window;

}

const VkSurfaceKHR* vKwindow::getSurface(){
	return this->surface.operator &();
};
bool vKwindow::initWindow(int witdh,int height,char* windowName){

	//Init GLFW
	glfwInit();
	glfwSetErrorCallback(error_callback);
	glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);

	// Creating window
	this->window = glfwCreateWindow(witdh,height,windowName,nullptr,nullptr);

	return true; //ToDo> error management
}

void vKwindow::createSurface(VkInstance instance){

	VkResult err = glfwCreateWindowSurface(instance,this->window,nullptr,surface.replace());
	if (err!= VK_SUCCESS){
        throw std::runtime_error("failed to create window surface!");
	}
};


