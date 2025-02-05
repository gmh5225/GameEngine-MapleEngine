//////////////////////////////////////////////////////////////////////////////
// This file is part of the Maple Engine                              		//
//////////////////////////////////////////////////////////////////////////////

#include "MetaFile.h"
#include "LuaComponent.h"
#include <fstream>
#include "cereal/archives/json.hpp"
#include "Scene/Entity/Entity.h"
#include "Others/Serialization.h"
#include "Others/Console.h"

namespace maple
{
	auto MetaFile::save(const component::LuaComponent* comp, const std::string & fileName) const -> void
	{
		std::ofstream os(fileName, std::ios::binary);
		cereal::JSONOutputArchive archive(os);

		if (comp->table) 
		{
			for (auto&& pair : luabridge::pairs(*comp->table))
			{
				auto name = pair.first.tostring();
				if (name == "__cname" || name == "__index") {
					continue;
				}

				if (pair.second.isNumber())
				{
					float value = pair.second;
					archive(cereal::make_nvp(name, value));
				}
				else if (pair.second.isString())
				{
					std::string value = pair.second;
					archive(cereal::make_nvp(name, value));
				}
				else if (pair.second.isBool())
				{
					bool value = pair.second;
					archive(cereal::make_nvp(name, value));
				}
				else if (pair.second.isTable())
				{
					//PRINT_FUNC();
					LOGW("Table serialization Not implementation");
				}
				else if (pair.second.isUserdata())
				{
					if (pair.second.isInstance<glm::vec2>())
					{
						glm::vec2* v = pair.second;
						archive(cereal::make_nvp(name, *v));
					}
					else if (pair.second.isInstance<glm::vec3>())
					{
						glm::vec3* v = pair.second;
						archive(cereal::make_nvp(name, *v));
					}
					else if (pair.second.isInstance<glm::vec4>())
					{
						glm::vec4* v = pair.second;
						archive(cereal::make_nvp(name, *v));

					}
					else if ((pair.second.isInstance<Entity>() && name != "entity"))
					{
						Entity * v = pair.second;
						archive(cereal::make_nvp(name, v->getHandle()));
					}
				}
			}
		}
	}

	auto MetaFile::load(component::LuaComponent* comp, const std::string& fileName,Scene * scene) -> void
	{
		std::ifstream os(fileName);
		if (os.good()) 
		{
			os.seekg(0, std::ios::end);
			auto len = os.tellg();
			os.seekg(0, std::ios::beg);
			if (len > 0) {
				cereal::JSONInputArchive archive(os);
				auto table = comp->table;

				for (auto&& pair : luabridge::pairs(*comp->table))
				{
					auto name = pair.first.tostring();
					if (pair.second.isNumber())
					{
						float value = pair.second;
						archive(cereal::make_nvp(name, value));
						(*comp->table)[name] = value;
					}
					else if (pair.second.isString())
					{
						std::string value = pair.second;
						archive(cereal::make_nvp(name, value));
						(*comp->table)[name] = value;
					}
					else if (pair.second.isBool())
					{
						bool value = pair.second;
						archive(cereal::make_nvp(name, value));
						pair.second = value;
						(*comp->table)[name] = value;
					}
					else if (pair.second.isTable())
					{
						LOGC("Table serialization Not implementation");
					}
					else if (pair.second.isUserdata())
					{
						if (pair.second.isInstance<glm::vec2>())
						{
							glm::vec2* v = pair.second;
							archive(cereal::make_nvp(name, *v));
						}
						else if (pair.second.isInstance<glm::vec3>())
						{
							glm::vec3* v = pair.second;
							archive(cereal::make_nvp(name, *v));
						}
						else if (pair.second.isInstance<glm::vec4>())
						{
							glm::vec4* v = pair.second;
							archive(cereal::make_nvp(name, *v));

						}
						else if ((pair.second.isInstance<Entity>() && name != "entity"))
						{
							Entity* v = pair.second;
							entt::entity e;
							archive(cereal::make_nvp(name, e));
							v->setHandle(e);
						}
					}
				}
			}
		}
	}
};