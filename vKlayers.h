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
	std::vector<VkLayerProperties> getAvailableLayers();
	std::vector<const char*> getValidationLayers();

	//Debug
	void printAvailableLayers();
private:

	bool vKlayersEnable;
	std::vector<const char*> validationLayers = {"VK_LAYER_LUNARG_standard_validation"};
	std::vector<VkLayerProperties>  availableLayers = {};

};

#endif /* VKLAYERS_H_ */
