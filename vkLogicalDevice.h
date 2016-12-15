/*
 * vkLogicalDevice.h
 *
 *  Created on: Nov 14, 2016
 *      Author: lcarro
 */

#ifndef VKLOGICALDEVICE_H_
#define VKLOGICALDEVICE_H_

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>
#include "VDeleter.h"
#include "vKphysicalDevice.h"
#include "vKlayers.h"
#include "vKwindow.h"
#include <set>

class vkLogicalDevice {

public:

	vkLogicalDevice();
	virtual ~vkLogicalDevice();

	void createLogicalDevice(vKphysicalDevice physicalDevice,vKlayers layers,vKwindow* window);

private:

	VkPhysicalDeviceFeatures logicalDeviceFeatures        = {};
	VkDeviceCreateInfo       createLogicalDeviceInfo      = {};
	float queuePriority = 1.0f;

	VDeleter<VkDevice> logicalDevice{vkDestroyDevice};
	VkQueue            graphicQueue;
	VkQueue			   presentQueue;
};

#endif /* VKLOGICALDEVICE_H_ */
