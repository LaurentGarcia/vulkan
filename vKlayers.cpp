/*
 * vKlayers.cpp
 *
 *  Created on: Nov 8, 2016
 *      Author: lcarro
 */

#include "vKlayers.h"


vKlayers::vKlayers() {
	// TODO Auto-generated constructor stub

}

vKlayers::~vKlayers() {
	// TODO Auto-generated destructor stub
}

void vKlayers::printAvailableLayers(){

	std::cout << "Available Vulkan Layers for Debug: \n";
	if (this->availableLayers.size()!=0)
	{
		for (auto i : this->availableLayers)
		    std::cout << i.layerName << ' '<< std::endl;
	}
}

bool vKlayers::initLayerSupport(){

	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount,nullptr);
	this->availableLayers.resize(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount,this->availableLayers.data());

	for (const char* layerName : this->validationLayers)
	{
		bool layerFound = false;

		for (const auto& layerProperties : this->availableLayers){
			if (strcmp(layerName,layerProperties.layerName))
			{
				layerFound = true;
				break;
			}
		}
		if (!layerFound)
		{
			this->vKlayersEnable = false;
			return this->vKlayersEnable;
		}
	}
	this->vKlayersEnable = true;
	return this->vKlayersEnable;
}


std::vector<const char*> vKlayers::getRequiredExtensions(){

	std::vector<const char*> extensions;

	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	//debug

	for (unsigned int i = 0; i<glfwExtensionCount;i++){
		extensions.push_back(glfwExtensions[i]);
		printf("glfwGetRequiredExtension> %s",glfwExtensions[i]);
		fflush(stdout);
	}
	if (this->vKlayersEnable){
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}
	return extensions;
}

bool vKlayers::ValidationLayersActivated(){
	return enableValidationLayers;
};
//Utils and get private members

std::vector<VkLayerProperties> vKlayers::getAvailableLayers(){
	return this->availableLayers;
}

bool vKlayers::getLayersEnable(){
	return this->vKlayersEnable;
}
std::vector<const char*> vKlayers::getValidationLayers(){
	return this->validationLayers;
}
