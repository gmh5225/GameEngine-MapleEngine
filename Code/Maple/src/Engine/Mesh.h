//////////////////////////////////////////////////////////////////////////////
// This file is part of the Maple Engine                              		//
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Engine/Core.h"
#include "Engine/Vertex.h"
#include "RHI/IndexBuffer.h"
#include "RHI/Texture.h"
#include "RHI/VertexBuffer.h"

#include "Timestep.h"
#include <memory>
#include <string>
#include <vector>
namespace maple
{
	enum class MeshType
	{
		MESH,
		TERRAIN,
	};

	class DescriptorSet;
	class Camera;
	class BoundingBox;
	class Material;

	class MAPLE_EXPORT Mesh
	{
	  public:
		Mesh() = default;
		Mesh(const std::shared_ptr<VertexBuffer> &vertexBuffer,
		     const std::shared_ptr<IndexBuffer> & indexBuffer);
		Mesh(const std::vector<uint32_t>& indices, const std::vector<Vertex>& vertices);
		Mesh(const std::vector<uint32_t>& indices, const std::vector<SkinnedVertex>& vertices);

		inline auto setMaterial(const std::shared_ptr<Material> &material)
		{
			materials.clear();
			materials.emplace_back(material);
		}

		inline auto setMaterial(const std::vector<std::shared_ptr<Material>> & material)
		{
			materials = material;
		}

		inline auto setIndicesSize(uint32_t size)
		{
			this->size = size;
		}
		inline auto getSize()
		{
			return size;
		}

		inline auto setSubMeshCount(uint32_t subMesh) 
		{
			subMeshCount = subMesh;
		}

		inline auto setSubMeshIndex(const std::vector<uint32_t> & idx)
		{
			subMeshIndex = idx;
		}

		inline auto getSubMeshCount() const { return subMeshCount; }

		inline auto& getSubMeshIndex() const { return subMeshIndex; }

		inline auto &getIndexBuffer()
		{
			return indexBuffer;
		}
		inline auto &getVertexBuffer()
		{
			return vertexBuffer;
		}
		inline auto &getMaterial()
		{
			return materials;
		}
		inline auto &getDescriptorSet()
		{
			return descriptorSet;
		}
		inline void setDescriptorSet(const std::shared_ptr<DescriptorSet> &set)
		{
			descriptorSet = set;
		}
		inline auto &isActive() const
		{
			return active;
		}
		inline auto setActive(bool active)
		{
			this->active = active;
		}

		inline auto &getBoundingBox() const
		{
			return boundingBox;
		}

		inline auto &getName() const
		{
			return name;
		}
		inline auto setName(const std::string &name)
		{
			this->name = name;
		}

		virtual auto getType() -> MeshType
		{
			return MeshType::MESH;
		}

		auto setIndicies(uint32_t range) -> void;

		static auto createQuad(bool screen = false) -> std::shared_ptr<Mesh>;
		static auto createQuaterScreenQuad() -> std::shared_ptr<Mesh>;
		static auto createCube() -> std::shared_ptr<Mesh>;
		static auto createPyramid() -> std::shared_ptr<Mesh>;
		static auto createSphere(uint32_t xSegments = 64, uint32_t ySegments = 64) -> std::shared_ptr<Mesh>;
		static auto createPlane(float w, float h, const glm::vec3 &normal) -> std::shared_ptr<Mesh>;

		static auto generateNormals(std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices) -> void;
		static auto generateTangents(std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices) -> void;

		inline auto& getBlendIndices(int32_t index)
		{
			return blendIndices[index];
		}

		inline auto& getBlendWeights(int32_t index)
		{
			return blendWeights[index];
		}

		inline auto resizeBlendWeight(uint32_t size)
		{
			blendIndices.resize(size);
			blendWeights.resize(size);
		}

	  protected:
		static auto generateTangent(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c, const glm::vec2 &ta, const glm::vec2 &tb, const glm::vec2 &tc) -> glm::vec3;

		std::shared_ptr<IndexBuffer>   indexBuffer;
		std::shared_ptr<VertexBuffer>  vertexBuffer;
		std::shared_ptr<Texture>       texture;
		std::shared_ptr<DescriptorSet> descriptorSet;
		std::vector<std::shared_ptr<Material>> materials;

		std::shared_ptr<BoundingBox> boundingBox;

		uint32_t    size   = 0;
		bool        active = true;
		std::string name;
		uint32_t subMeshCount = 0;
		std::vector<uint32_t> subMeshIndex;

		/// Skinned mesh blend indices (max 4 per bone)
		std::vector<glm::ivec4> blendIndices;
		/// Skinned mesh index buffer (max 4 per bone)
		std::vector<glm::vec4> blendWeights;
	};
};        // namespace maple
