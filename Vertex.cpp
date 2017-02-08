/*
 * Vertex.cpp
 *
 *  Created on: Feb 8, 2017
 *      Author: lcarro
 */

#include "Vertex.h"

Vertex::Vertex() {
	// TODO Auto-generated constructor stub

}

Vertex::~Vertex() {
	// TODO Auto-generated destructor stub
}

VkVertexInputBindingDescription Vertex::getBindingDescription(){

	VkVertexInputBindingDescription bindingDescription = {};

	bindingDescription.binding 	 = 0;
	bindingDescription.stride  	 = sizeof(Vertex::VertexAtributes);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescription;
};


std::array<VkVertexInputAttributeDescription,2> Vertex::getAttributeDescriptions(){

	std::array<VkVertexInputAttributeDescription,2> attributeDescription = {};

	attributeDescription[0].binding  = 0;
	attributeDescription[0].location = 0;
	attributeDescription[0].format   = VK_FORMAT_R32G32_SFLOAT;
	attributeDescription[0].offset   = offsetof(Vertex::VertexAtributes,pos);

	attributeDescription[1].binding  = 0;
	attributeDescription[1].location = 1;
	attributeDescription[1].format   = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescription[1].offset   = offsetof(Vertex::VertexAtributes, color);

	return attributeDescription;
}
