#include "GP2Shader.h"
#include "vulkanbase/VulkanUtil.h"

GP2Shader::GP2Shader(const VkDevice& vkDevice, const string& vertexShaderFile, const string& fragmentShaderFile)
	: m_VertexShaderFile{ vertexShaderFile }
	, m_FragmentShaderFile{ fragmentShaderFile }
	, m_VertexInfo{ CreateVertexShaderInfo(vkDevice) }
	, m_FragmentInfo{ CreateFragmentShaderInfo(vkDevice) }
{}

VkPipelineShaderStageCreateInfo GP2Shader::CreateFragmentShaderInfo(const VkDevice& vkDevice) {
	std::vector<char> fragShaderCode = VkUtils::ReadFile(m_FragmentShaderFile);
	VkShaderModule fragShaderModule = CreateShaderModule(vkDevice, fragShaderCode);

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	return fragShaderStageInfo;
}

VkPipelineShaderStageCreateInfo GP2Shader::CreateVertexShaderInfo(const VkDevice& vkDevice) {
	std::vector<char> vertShaderCode = VkUtils::ReadFile(m_VertexShaderFile);
	VkShaderModule vertShaderModule = CreateShaderModule(vkDevice, vertShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";
	return vertShaderStageInfo;
}

GP2Shader::~GP2Shader()
{
	vkDestroyShaderModule(m_Device, m_VertexInfo.module, nullptr);
	vkDestroyShaderModule(m_Device, m_FragmentInfo.module, nullptr);
}

VkPipelineVertexInputStateCreateInfo GP2Shader::CreateVertexInputStateInfo()
{
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = int(GP2Mesh::Vertex::ATTRIBUTE_DESCS.size());

	vertexInputInfo.pVertexBindingDescriptions = &GP2Mesh::Vertex::BINDING_DESC;
	vertexInputInfo.pVertexAttributeDescriptions = GP2Mesh::Vertex::ATTRIBUTE_DESCS.data();

	return vertexInputInfo;
}

VkPipelineInputAssemblyStateCreateInfo GP2Shader::CreateInputAssemblyStateInfo()
{
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;
	return inputAssembly;
}

VkShaderModule GP2Shader::CreateShaderModule(const VkDevice& vkDevice, const std::vector<char>& code) {
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(vkDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to Create shader module!");
	}

	return shaderModule;
}
