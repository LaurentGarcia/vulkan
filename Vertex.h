/*
 * Vertex.h
 *
 *  Created on: Feb 8, 2017
 *      Author: lcarro
 */

#ifndef VERTEX_H_
#define VERTEX_H_
#include <stdio.h>
#include <vector>
#include <array>
#include <glm/glm.hpp>

class Vertex {

	public:

	Vertex();
	virtual ~Vertex();

	struct VertexAtrr{
		glm::vec2 pos;
		glm::vec3 color;
	};

	static const std::vector<VertexAtrr> vertices; // Def at cpp

	static VkVertexInputBindingDescription                 getBindingDescription();
	static std::array<VkVertexInputAttributeDescription,2> getAttributeDescriptions();

};


#endif /* VERTEX_H_ */
