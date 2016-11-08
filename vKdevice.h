/*
 * vKdevice.h
 *
 *  Created on: Nov 8, 2016
 *      Author: lcarro
 */

#ifndef VKDEVICE_H_
#define VKDEVICE_H_

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>
#include "VDeleter.h"
#include <vector>

class vKdevice {

public:

	vKdevice();
	virtual ~vKdevice();

	VkResult initVulkan();

private:

	VDeleter<VkInstance> instance{vkDestroyInstance};
	VkApplicationInfo    appVkInfo    = {};
	VkInstanceCreateInfo createVkInfo = {};
	std::vector<VkExtensionProperties>extensions;

	void fillAppVkInfo();
	void fillVkInfo();
	void fillExtensionsProperties();
};

#endif /* VKDEVICE_H_ */
