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

class vKwindow {

public:

	vKwindow();
	virtual ~vKwindow();

	bool initWindow(int witdh,int height,char* windowName);
	GLFWwindow* getWindow();

private:
	GLFWwindow* window;

};

#endif /* VKWINDOW_H_ */
