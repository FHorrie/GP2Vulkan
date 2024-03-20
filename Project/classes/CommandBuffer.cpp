#include <cassert>

#include "CommandBuffer.h"
#include "vulkanbase/VulkanBase.h"

CommandBuffer::CommandBuffer(const VkDevice& vkDevice, uint32_t queueFamilyIdx)
{
	if(!COMMANDPOOL)
	{
		assert(queueFamilyIdx != 9999);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = queueFamilyIdx;

		if (vkCreateCommandPool(vkDevice, &poolInfo, nullptr, &COMMANDPOOL) != VK_SUCCESS)
			throw std::runtime_error("failed to create command pool!");
	}

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = COMMANDPOOL;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;

	if (vkAllocateCommandBuffers(vkDevice, &allocInfo, &m_CommandBuffer) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate command buffers!");

	NUMBUFFERS++;
}

CommandBuffer::CommandBuffer(const VkDevice& vkDevice)
	: CommandBuffer(vkDevice, 1000)
{}

CommandBuffer::~CommandBuffer()
{
	vkFreeCommandBuffers(m_Device, COMMANDPOOL, 1, &m_CommandBuffer);
	NUMBUFFERS--;

	if (NUMBUFFERS <= 0)
	{
		vkDestroyCommandPool(m_Device, COMMANDPOOL, nullptr);
		COMMANDPOOL = nullptr;
	}
}

void CommandBuffer::BeginRecord() const
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0; // Optional
	beginInfo.pInheritanceInfo = nullptr; // Optional

	if (vkBeginCommandBuffer(m_CommandBuffer, &beginInfo) != VK_SUCCESS)
		throw std::runtime_error("failed to begin recording command buffer!");
}

void CommandBuffer::EndRecord() const 
{
	if (vkEndCommandBuffer(m_CommandBuffer) != VK_SUCCESS)
		throw std::runtime_error("failed to end recording command buffer!");
}