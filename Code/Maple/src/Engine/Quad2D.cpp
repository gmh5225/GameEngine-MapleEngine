//////////////////////////////////////////////////////////////////////////////
// This file is part of the Maple Engine                              		//
//////////////////////////////////////////////////////////////////////////////
#include "Quad2D.h"
#include "RHI/Texture.h"

namespace maple
{
	auto Quad2D::getDefaultTexCoords() -> const std::array<glm::vec2, 4> &
	{
		static std::array<glm::vec2, 4> results =
		    {
		        glm::vec2{0, 0},
		        glm::vec2{1, 0},
		        glm::vec2{0, 1},
		        glm::vec2{1, 1}
			};
		return results;
	}

	Quad2D::Quad2D()
	{
		texCoords = getDefaultTexCoords();
		color     = glm::vec4(1.f);
	}

	auto Quad2D::getTexCoords(const glm::vec2 &min, const glm::vec2 &max) -> const std::array<glm::vec2, 4> &
	{
		static std::array<glm::vec2, 4> results;
		{
			results[3] = {min.x, max.y};
			results[2] = max;
			results[1] = {max.x, min.y};
			results[0] = min;
		}
		return results;
	}

	auto Quad2D::setTexCoords(uint32_t x, uint32_t y, uint32_t w, uint32_t h, bool flipY) -> void
	{
		this->w      = w;
		this->h      = h;
		if (flipY)
		{
			texCoords[0] = {x / (float) texture->getWidth(), (y + h) / (float) texture->getHeight()};
			texCoords[1] = {(x + w) / (float) texture->getWidth(), (y + h) / (float) texture->getHeight()};
			texCoords[2] = {(x + w) / (float) texture->getWidth(), y / (float) texture->getHeight()};
			texCoords[3] = {x / (float) texture->getWidth(), y / (float) texture->getHeight()};
		}
		else
		{
			texCoords[3] = {x / (float) texture->getWidth(), (y + h) / (float) texture->getHeight()};
			texCoords[2] = {(x + w) / (float) texture->getWidth(), (y + h) / (float) texture->getHeight()};
			texCoords[1] = {(x + w) / (float) texture->getWidth(), y / (float) texture->getHeight()};
			texCoords[0] = {x / (float) texture->getWidth(), y / (float) texture->getHeight()};
		}

	}

	maple::Quad2D Quad2D::nullQuad;

};        // namespace maple
