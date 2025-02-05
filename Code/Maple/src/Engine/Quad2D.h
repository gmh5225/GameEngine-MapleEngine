//////////////////////////////////////////////////////////////////////////////
// This file is part of the Maple Engine                              		//
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <array>
#include "Engine/Core.h"
namespace maple 
{
	class Texture2D;

	class MAPLE_EXPORT Quad2D
	{
	public:
		Quad2D();
		virtual ~Quad2D() = default;
		inline auto getTexture() const { return texture; }
		inline auto& getColor() const { return color; }
		inline auto& getOffset() const { return offset; }
		inline auto& getTexCoords() const { return texCoords; }
		
		static auto getDefaultTexCoords() -> const std::array<glm::vec2, 4>&;
		static auto getTexCoords(const glm::vec2& min, const glm::vec2& max)-> 	const std::array<glm::vec2, 4>&;

		inline auto setColor(const glm::vec4& c) { color = c; }

		inline auto setOffset(const glm::vec2& c) { offset = c; }

		inline auto setTexture(const std::shared_ptr<Texture2D>& texture) { this->texture = texture; }
		friend class Sprite;

		auto setTexCoords(uint32_t x, uint32_t y, uint32_t w, uint32_t h, bool flipY) -> void;

		inline auto getHeight() const { return h; }
		inline auto getWidth() const { return w; }
		inline auto isValid() const { return this != &nullQuad; }

		static Quad2D nullQuad;
		inline auto& getPivot() const { return pivot; }
		inline auto setPivot(const glm::vec2 & val) { pivot = val; }
	protected:
		std::shared_ptr<Texture2D> texture;
		glm::vec4 color = {};
		glm::vec2 offset = {};
		std::array<glm::vec2, 4> texCoords = {};
		uint32_t w = 0;
		uint32_t h = 0;
		glm::vec2 pivot = {};
	};
};