#pragma once
#include <vulkan/vulkan_core.h>

class CommandPool
{
public:
	CommandPool(const VkDevice& vkDevice, uint32_t queueFamilyIdx);
	~CommandPool() = default;
	CommandPool(const CommandPool&) = delete;
	CommandPool(CommandPool&&) = delete;
	CommandPool& operator=(const CommandPool&) = delete;
	CommandPool& operator=(CommandPool&&) = delete;

	VkCommandPool GetVkCommandPool() { return m_CommandPool; }

private:
	VkPhysicalDevice m_PhysicalDevice{ VK_NULL_HANDLE };
	VkCommandPool m_CommandPool{};

};
