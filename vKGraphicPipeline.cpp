/*
 * vKGraphicPipeline.cpp
 *
 *  Created on: Dec 20, 2016
 *      Author: lcarro
 */

#include "vKGraphicPipeline.h"

vKGraphicPipeline::vKGraphicPipeline() {
	// TODO Auto-generated constructor stub

}

vKGraphicPipeline::~vKGraphicPipeline() {
	// TODO Auto-generated destructor stub
}

void vKGraphicPipeline::createGraphicPipeline(){
	auto vertShaderCode = this->loadShaders("Shaders/vert.spv");
	auto fragShaderCode = this->loadShaders("Shaders/frag.spv");
};


const std::vector<char> vKGraphicPipeline::loadShaders(const std::string& filename){

	std::ifstream file(filename,std::ios::ate | std::ios::binary);
	if (!file.is_open()){
		throw std::runtime_error("Failed to open the Shaders code");
	}else{
		std::cout << "Loaded Shader: " << filename <<" \n";fflush(stdout);
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(),fileSize);
	file.close();

	return buffer;

};
