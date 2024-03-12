#include "CommandPool.h"
#include "vulkanbase/VulkanBase.h"

CommandPool::CommandPool(const VkDevice& vkDevice , uint32_t queueFamilyIdx)
{
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIdx;

	if (vkCreateCommandPool(vkDevice, &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}
}