//////////////////////////////////////////////////////////////////////////////
// This file is part of the Maple Engine                              		//
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Engine/Buffer.h"
#include "Engine/Core.h"
#include "RHI/DescriptorSet.h"
#include "VulkanHelper.h"

namespace maple
{
	constexpr int32_t MAX_BUFFER_INFOS      = 32;
	constexpr int32_t MAX_IMAGE_INFOS       = 32;
	constexpr int32_t MAX_WRITE_DESCTIPTORS = 32;

	class VulkanDescriptorSet final : public DescriptorSet
	{
	  public:
		VulkanDescriptorSet(const DescriptorInfo &info);
		~VulkanDescriptorSet();
		NO_COPYABLE(VulkanDescriptorSet);
		auto update() -> void override;

		inline auto setDynamicOffset(uint32_t offset) -> void override
		{
			dynamicOffset = offset;
		}
		inline auto getDynamicOffset() const -> uint32_t override
		{
			return dynamicOffset;
		}

		inline auto isDynamic() const
		{
			return dynamic;
		}

		auto getDescriptorSet() -> VkDescriptorSet;

		auto setTexture(const std::string &name, const std::vector<std::shared_ptr<Texture>> &textures) -> void override;
		auto setTexture(const std::string &name, const std::shared_ptr<Texture> &textures) -> void override;
		auto setBuffer(const std::string &name, const std::shared_ptr<UniformBuffer> &buffer) -> void override;
		auto getUnifromBuffer(const std::string &name) -> std::shared_ptr<UniformBuffer> override;
		auto setUniform(const std::string &bufferName, const std::string &uniformName, const void *data, bool dynamic) -> void override;
		auto setUniform(const std::string &bufferName, const std::string &uniformName, const void *data, uint32_t size, bool dynamic) -> void override;
		auto setUniformBufferData(const std::string &bufferName, const void *data) -> void override;
		auto getDescriptors() const -> const std::vector<Descriptor>& override { return descriptors; }

	  private:
		uint32_t dynamicOffset      = 0;
		Shader * shader             = nullptr;
		bool     dynamic            = false;
		bool     descriptorDirty[3] = {};

		std::vector<Descriptor> descriptors;

		std::array<VkDescriptorBufferInfo, MAX_BUFFER_INFOS>    bufferInfoPool;
		std::array<VkDescriptorImageInfo, MAX_IMAGE_INFOS>      imageInfoPool;
		std::array<VkWriteDescriptorSet, MAX_WRITE_DESCTIPTORS> writeDescriptorSetPool;

		uint32_t framesInFlight = 0;

		struct UniformBufferInfo
		{
			std::vector<BufferMemberInfo> members;
			Buffer                        localStorage;
			bool                          dynamic        = false;
			bool                          hasUpdated[10] = {};
		};

		std::vector<VkDescriptorSet>                                                 descriptorSet;
		std::vector<std::unordered_map<std::string, std::shared_ptr<UniformBuffer>>> uniformBuffers;
		std::unordered_map<std::string, UniformBufferInfo>                           uniformBuffersData;
	};
};        // namespace maple
