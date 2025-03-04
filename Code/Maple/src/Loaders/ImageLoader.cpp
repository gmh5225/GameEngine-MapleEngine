//////////////////////////////////////////////////////////////////////////////
// This file is part of the Maple Engine                              		//
//////////////////////////////////////////////////////////////////////////////
#include "ImageLoader.h"
#include <ktx.h>
#include <memory>
#include <stdexcept>

#define STBI_NO_PSD
#define STBI_NO_PIC
#define STBI_NO_GIF
#define STBI_NO_PNM

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Engine/Profiler.h"
#include "Others/StringUtils.h"
#include "RHI/Definitions.h"
#include "Others/Console.h"

#ifdef MAPLE_VULKAN
#include "RHI/Vulkan/VulkanTexture.h"
#endif

#ifdef MAPLE_OPENGL
#include "RHI/OpenGL/GLTexture.h"
#endif

namespace maple
{
	auto ImageLoader::loadAsset(const std::string &name, bool mipmaps, bool flipY) -> std::unique_ptr<Image>
	{
		PROFILE_FUNCTION();
		bool hdr = stbi_is_hdr(name.c_str());
		LOGI("load image : {0}",name);

		stbi_set_flip_vertically_on_load(flipY);

		int32_t       width;
		int32_t       height;
		int32_t       channels;
		TextureFormat format = hdr ? TextureFormat::RGBA32 : TextureFormat::RGBA8;
		uint8_t *     data   = hdr ?
                                   (uint8_t *) stbi_loadf(name.c_str(), &width, &height, &channels, STBI_rgb_alpha) :
                                   stbi_load(name.c_str(), &width, &height, &channels, STBI_rgb_alpha);

		uint32_t imageSize = width * height * 4 * (hdr ? sizeof(float) : sizeof(uint8_t));
		assert(data);
		return std::make_unique<Image>(format, width, height, data, imageSize, channels, mipmaps, hdr);
	}

	auto ImageLoader::loadAsset(const std::string &name, Image *image) -> void
	{
		PROFILE_FUNCTION();
		bool hdr = stbi_is_hdr(name.c_str());

		stbi_set_flip_vertically_on_load(1);

		int32_t       width;
		int32_t       height;
		int32_t       channels;
		TextureFormat format    = TextureFormat::RGBA8;
		uint8_t *     data      = stbi_load(name.c_str(), &width, &height, &channels, STBI_rgb_alpha);
		uint32_t      imageSize = width * height * 4;
		assert(data);

		image->setChannel(4);
		image->setWidth(width);
		image->setHeight(height);
		image->setPixelFormat(format);
		image->setData(data);
		image->setSize(imageSize);
	}

}        // namespace maple
