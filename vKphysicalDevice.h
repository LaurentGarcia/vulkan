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
#endif

class vKphysicalDevice {
public:

	vKphysicalDevice();
	virtual ~vKphysicalDevice();

	struct QueueFamilyIndices{
			int  graphicFamily = -1;
			bool isComplete(){
				return graphicFamily >=0;
			}
		};
	void pickPhysicalDevice(const VkInstance* vkDevice);

private:

	VkPhysicalDevice                     physicalDevice = VK_NULL_HANDLE;
	std::vector<VkPhysicalDevice>        availablePhysicalDevices = {};

	void enumerateDevices(const VkInstance* vkDevice);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	bool isDeviceSuitable(VkPhysicalDevice device);

};

#endif /* VKPHYSICALDEVICE_H_ */
