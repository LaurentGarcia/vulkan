/*
 * vKwindow.h
 *
 *  Created on: Nov 8, 2016
 *      Author: lcarro
 */

#ifndef VKWINDOW_H_
#define VKWINDOW_H_

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>
#include "VDeleter.h"
#include "vKdevice.h"



class vKwindow : public vKdevice {

public:

	vKwindow();
	virtual ~vKwindow();

	bool initWindow(int witdh,int height,char* windowName);
	GLFWwindow* getWindow();

private:

	GLFWwindow*            		  window;
	VDeleter<VkSurfaceKHR>        surface{instance,vkDestroySurfaceKHR};
	VkDisplaySurfaceCreateInfoKHR surfaceProperties = {};

	void createSurface();
};

#endif /* VKWINDOW_H_ */
