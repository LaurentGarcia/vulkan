/*
 * vKwindow.h
 *
 *  Created on: Nov 8, 2016
 *      Author: lcarro
 */

#ifndef VKWINDOW_H_
#define VKWINDOW_H_

#if __linux__
	#define  GLFW_INCLUDE_VULKAN
	#include <glfw3.h>
	#include "VDeleter.h"
	#include "vKdevice.h"
#endif


class vKwindow : public vKdevice {

public:

	vKwindow();
	virtual ~vKwindow();

	bool initWindow(int witdh,int height,char* windowName);
	void createSurface(VkInstance instance);

	GLFWwindow*         getWindow();
	const VkSurfaceKHR* getSurface();


	static void error_callback(int error, const char* description);

private:

	GLFWwindow*            		  window;
	VDeleter<VkSurfaceKHR>        surface{vKdevice::instance,vkDestroySurfaceKHR};

};

#endif /* VKWINDOW_H_ */
