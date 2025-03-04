//////////////////////////////////////////////////////////////////////////////
// This file is part of the Maple Engine                              		//
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "TextureAtlas.h"
#include <unordered_map>

namespace maple
{
	class MAPLE_EXPORT TexturePool final
	{
	public:
		TexturePool() = default;

		auto addSprite(const std::string& file)->Quad2D*;
		auto addSprite(const std::string &uniqueName, const std::vector<uint8_t> &, uint32_t w, uint32_t h, bool flipY = false) -> Quad2D *;

	private:
		auto createTextureAtlas()->TextureAtlas*;

		std::vector<TextureAtlas> atlas;
		std::unordered_map<std::string, TextureAtlas*> mapping;
	};
};