//////////////////////////////////////////////////////////////////////////////
// This file is part of the Maple Engine                              		//
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <vulkan/vulkan.h>
#ifdef USE_VMA_ALLOCATOR
#	include <vulkan/vk_mem_alloc.h>
#endif        // USE_VMA_ALLOCATOR

namespace maple
{
	struct VkConfig
	{
		static constexpr bool StandardValidationLayer = false;
		static constexpr bool AssistanceLayer         = false;
		static constexpr bool EnableValidationLayers  = true;
	};

	class VulkanDevice;
	class VulkanBuffer;
	class VulkanImage;
	class VulkanImageView;
	class VulkanInstance;
	class VulkanQueryPool;
	class VulkanSurface;
	class VulkanSwapChain;
	class VulkanDescriptorPool;
	class VulkanFence;
	class VulkanRenderPass;
	class VulkanFrameBuffer;
	class VulkanShader;
	class VulkanCommandPool;
	class VulkanCommandBuffer;
}        // namespace maple
