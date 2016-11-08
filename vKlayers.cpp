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

std::vector<VkLayerProperties> vKlayers::getAvailableLayers(){
	return this->availableLayers;
}

bool vKlayers::getLayersEnable(){
	return this->vKlayersEnable;
}
std::vector<const char*> vKlayers::getValidationLayers(){
	return this->validationLayers;
}
