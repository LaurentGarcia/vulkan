/*
 * vKwindow.h
 *
 *  Created on: Nov 8, 2016
 *      Author: lcarro
 */

#ifndef VKWINDOW_H_
#define VKWINDOW_H_

#ifdef __linux__
	#if defined(VK_USE_PLATFORM_XLIB_KHR)
		#include <X11/Xlib.h>
		#include <X11/Xutil.h>
		#include <dlfcn.h>
		#include <cstdlib>
	#elif defined(VK_USE_PLATFORM_XCB_KHR)
		#include <xcb/xcb.h>
		#include <dlfcn.h>
		#include <cstdlib>
	#endif
#elif _WIN32
//ToDo> Port to win32
#endif


#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>
#include <vulkan.h>
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
