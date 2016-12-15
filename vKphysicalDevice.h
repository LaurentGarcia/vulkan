/*
 * vKphysicalDevice.h
 *
 *  Created on: Nov 11, 2016
 *      Author: lcarro
 */

#ifndef VKPHYSICALDEVICE_H_
#define VKPHYSICALDEVICE_H_

#ifdef __linux__
	#define GLFW_INCLUDE_VULKAN
	#include <glfw3.h>
	#include <vector>
	#include "vKdevice.h"
	#include "vKwindow.h"
#endif

class vKphysicalDevice {
public:

	vKphysicalDevice();
	virtual ~vKphysicalDevice();



	void pickPhysicalDevice(const VkInstance* vkDevice,vKwindow* window);

	vKdevice::QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device,vKwindow* window);
	VkPhysicalDevice   getPhysicalDevice();


private:

	VkPhysicalDevice                     physicalDevice = VK_NULL_HANDLE;
	std::vector<VkPhysicalDevice>        availablePhysicalDevices = {};

	void enumerateDevices(const VkInstance* vkDevice);

	bool isDeviceSuitable(VkPhysicalDevice device,vKwindow* window);

};

#endif /* VKPHYSICALDEVICE_H_ */
