/*
 * vKdevice.h
 *
 *  Created on: Nov 8, 2016
 *      Author: lcarro
 */

#ifndef VKDEVICE_H_
#define VKDEVICE_H_


#include <vulkan.h>
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
	const VkInstance* getInstance();
	std::vector<const char*> getRequiredExtensions(vKlayers* layers);

protected:

	VDeleter<VkInstance> 				instance{vkDestroyInstance};
	//VkDebugReportCallbackEXT 			callback;
	VDeleter<VkDebugReportCallbackEXT>  callback{instance,vKlayers::DestroyDebugReportCallbackEXT};
	VkApplicationInfo    				appVkInfo    = {};
	VkInstanceCreateInfo 				createVkInfo = {};
	VkDebugReportCallbackCreateInfoEXT  createCallbackInfo = {};
	std::vector<VkExtensionProperties>	instanceExtensionsProperties;

	//
	std::vector<const char*>  vulkanExtensions;
    std::vector<const char*>  vulkanLayersAvailable;


private:

	void fillAppVkInfo();
	void fillAppVkInfo(vKlayers* vklayersInfo);
	void fillVkInfo();
	void fillExtensionsProperties();
	void setupDebugCallback();
};

#endif /* VKDEVICE_H_ */
