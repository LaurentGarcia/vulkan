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

	bool createLogicalDevice(vKphysicalDevice psDevice,vKlayers layers);
	const VkDevice* getLogicalDevice();

private:

	VDeleter<VkDevice>       device{vkDestroyDevice};
	VkQueue                  graphicsQueue;
	VkDeviceQueueCreateInfo  queueCreateInfo = {};
	VkPhysicalDeviceFeatures deviceFeatures  = {};
	VkDeviceCreateInfo       createInfo      = {};
};

#endif /* VKLOGICALDEVICE_H_ */
