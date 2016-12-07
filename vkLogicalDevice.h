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

class vkLogicalDevice {

public:

	vkLogicalDevice();
	virtual ~vkLogicalDevice();

	void createLogicalDevice(vKphysicalDevice physicalDevice,vKlayers layers);

private:

	VkDeviceQueueCreateInfo  queueLogicalDeviceCreateInfo = {};
	VkPhysicalDeviceFeatures logicalDeviceFeatures        = {};
	VkDeviceCreateInfo       createLogicalDeviceInfo      = {};
	float queuePriority = 1.0f;

	VDeleter<VkDevice> logicalDevice{vkDestroyDevice};
	VkQueue            graphicQueue;
};

#endif /* VKLOGICALDEVICE_H_ */
