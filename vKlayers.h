/*
 * vKlayers.h
 *
 *  Created on: Nov 8, 2016
 *      Author: lcarro
 */

#ifndef VKLAYERS_H_
#define VKLAYERS_H_

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>
#include <vector>
#include <cstring>
#include <iostream>
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

class vKlayers {

public:

    vKlayers();
	virtual ~vKlayers();

	bool getLayersEnable();

	bool initLayerSupport();

	std::vector<const char*> getRequiredExtensions();

	// Static members Callback
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
														VkDebugReportFlagsEXT flags,
														VkDebugReportObjectTypeEXT objType,
														uint64_t obj,
														size_t location,
														int32_t code,
														const char* layerPrefix,
														const char* msg,
														void* userData) {
		std::cerr << "validation layer: " << msg << std::endl;

	    return VK_FALSE;
	};

	static VkResult CreateDebugReportCallbackEXT      (VkInstance instance,
													   const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
													   const VkAllocationCallbacks* pAllocator,
													   VkDebugReportCallbackEXT* pCallback) {

	    auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	    if (func != nullptr) {
	        return func(instance, pCreateInfo, pAllocator, pCallback);
	    } else {
	        return VK_ERROR_EXTENSION_NOT_PRESENT;
	    }
	}

	bool ValidationLayersActivated();
	// Get private members
	std::vector<VkLayerProperties> getAvailableLayers();

	std::vector<const char*> getValidationLayers();


	//Debug
	void printAvailableLayers();

private:

	bool vKlayersEnable;
	const std::vector<const char*>  validationLayers = {"VK_LAYER_LUNARG_standard_validation"};
	std::vector<VkLayerProperties>  availableLayers = {};

};

#endif /* VKLAYERS_H_ */
