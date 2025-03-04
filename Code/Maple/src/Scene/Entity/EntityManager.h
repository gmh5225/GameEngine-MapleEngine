//////////////////////////////////////////////////////////////////////////////
// This file is part of the Maple Engine                              		//
//////////////////////////////////////////////////////////////////////////////
#include <string>
#include "Entity.h"

namespace maple
{
	class Scene;

	class MAPLE_EXPORT EntityManager final
	{
	  public:
		EntityManager(Scene *initScene) :
		    scene(initScene){};

		auto create() -> Entity;
		auto create(const std::string &name) -> Entity;

		template <typename... Components>
		inline auto getEntitiesWithTypes()
		{
			if constexpr (sizeof...(Components) == 1)
			{
				return registry.view<Components...>();
			}
			else
			{
				return registry.group<Components...>();
			}
		}

		auto getEntityByName(const std::string &name) -> Entity;

		template <typename R, typename T>
		inline auto addDependency() -> void
		{
			registry.on_construct<R>().connect<&entt::registry::get_or_emplace<T>>();
		}

		inline auto &getRegistry()
		{
			return registry;
		}

		auto clear() -> void;

		auto removeAllChildren(entt::entity entity, bool root = true) -> void;
		auto removeEntity(entt::entity entity) -> void;

	  private:
		Scene *        scene = nullptr;
		entt::registry registry;
	};
};        // namespace maple
