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
#include "vKlayers.h"
#include "VDeleter.h"
#include <vector>


class vKdevice {

public:

	vKdevice();
	virtual ~vKdevice();

	VkResult initVulkan();
	VkResult initVulkan(vKlayers* vklayersInfo);

private:

	VDeleter<VkInstance> 				instance{vkDestroyInstance};
	VkDebugReportCallbackEXT 			callback;
	VkApplicationInfo    				appVkInfo    = {};
	VkInstanceCreateInfo 				createVkInfo = {};
	VkDebugReportCallbackCreateInfoEXT  createCallbackInfo = {};
	std::vector<VkExtensionProperties>	extensions;

	void fillAppVkInfo();
	void fillAppVkInfo(vKlayers* vklayersInfo);
	void fillVkInfo();
	void fillExtensionsProperties();
	void setupDebugCallback();
};

#endif /* VKDEVICE_H_ */
