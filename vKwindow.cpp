/*
 * vKwindow.cpp
 *
 *  Created on: Nov 8, 2016
 *      Author: lcarro
 */

#include "vKwindow.h"

static std::vector<const char*> VKextensions = {
	  VK_KHR_SURFACE_EXTENSION_NAME,
	#if defined(VK_USE_PLATFORM_WIN32_KHR)
	  VK_KHR_WIN32_SURFACE_EXTENSION_NAME
	#elif defined(VK_USE_PLATFORM_XCB_KHR)
	  VK_KHR_XCB_SURFACE_EXTENSION_NAME
	#elif defined(VK_USE_PLATFORM_XLIB_KHR)
	  VK_KHR_XLIB_SURFACE_EXTENSION_NAME
	#endif
	};

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

	for( size_t i = 0; i < VKextensions.size(); ++i ) {
	    std::cout << "VK instance extension named \"" << VKextensions[i] << "\"!" << std::endl;
	}

	return true; //ToDo> error management
}

void vKwindow::createSurface(){

};

