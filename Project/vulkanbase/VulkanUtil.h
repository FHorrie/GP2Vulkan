#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <optional>
#include <vector>
#include <fstream>

namespace VkUtils
{
	static inline constexpr uint32_t SCREENWIDTH{ 800 };
	static inline constexpr uint32_t SCREENHEIGHT{ 600 };
	
	#ifdef NDEBUG
	constexpr bool VALIDATIONLAYERS = false;
	#else
	constexpr bool VALIDATIONLAYERS = true;
	#endif
	
	// STOLEN HACK: this should be a singleton (THX MAT)
	inline VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
	std::vector<char> ReadFile(const std::string& filename);

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() const
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags memoryPropertyFlags);

	std::tuple<VkBuffer, VkDeviceMemory> CreateBuffer(VkDevice device,
		VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryPropertyFlags);

	void CopyBuffer(VkDevice device, VkBuffer fromBuffer, VkBuffer toBuffer, VkDeviceSize size, VkQueue graphicsQueue);
}



