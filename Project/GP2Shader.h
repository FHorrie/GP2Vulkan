#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include <string>

#include "Vertex.h"

using std::string;

class GP2Shader
{
public:

	void Init(const VkDevice& vkDevice);
	void DestroyShaderModules(const VkDevice& vkDevice);
	inline const std::vector<VkPipelineShaderStageCreateInfo>& GetShaderStages()  { return m_ShaderStages; }
	VkPipelineVertexInputStateCreateInfo CreateVertexInputStateInfo();
	VkPipelineInputAssemblyStateCreateInfo CreateInputAssemblyStateInfo();

	GP2Shader(const string& vertexShaderFile, const string& fragmentShaderFile)
		: m_VertexShaderFile{ vertexShaderFile }
		, m_FragmentShaderFile{ fragmentShaderFile }
		, m_BindingDescription{ Vertex::getBindingDescription() }
	{}
	~GP2Shader() = default;
	GP2Shader(const GP2Shader&)	= delete;
	GP2Shader(GP2Shader&&) = delete;
	GP2Shader& operator=(const GP2Shader&) = delete;
	GP2Shader& operator=(GP2Shader&&) = delete;

private:
	VkPipelineShaderStageCreateInfo CreateFragmentShaderInfo(const VkDevice& vkDevice);
	VkPipelineShaderStageCreateInfo CreateVertexShaderInfo(const VkDevice& vkDevice);
	VkShaderModule CreateShaderModule(const VkDevice& vkDevice, const std::vector<char>& code);

	string m_VertexShaderFile;
	string m_FragmentShaderFile;
	static constexpr int ATTRIBUTECOUNT{ 2 };

	VkVertexInputBindingDescription m_BindingDescription;
	std::array<VkVertexInputAttributeDescription, ATTRIBUTECOUNT> m_AttributeDescriptions{};
	
	std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;

};
