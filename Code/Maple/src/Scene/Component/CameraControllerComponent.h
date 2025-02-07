//////////////////////////////////////////////////////////////////////////////
// This file is part of the Maple Engine                              		//
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Component.h"
#include <memory>
#include <string>

namespace maple
{
	namespace component 
	{
		class CameraController;
		class MAPLE_EXPORT CameraControllerComponent : public Component
		{
		public:
			constexpr static char* ICON = ICON_MDI_CONTROLLER_CLASSIC;

			enum class ControllerType : int32_t
			{
				FPS = 0,
				EditorCamera,
				Custom
			};

			static std::string    typeToString(ControllerType type);
			static ControllerType stringToType(const std::string& type);

			CameraControllerComponent() :
				type(ControllerType::FPS)
			{
			}

			CameraControllerComponent(ControllerType type);

			auto setControllerType(CameraControllerComponent::ControllerType type) -> void;

			inline auto& getController()
			{
				return cameraController;
			}

			template <typename Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("ControllerType", type), cereal::make_nvp("Id", entity));
			}

			template <typename Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("ControllerType", type), cereal::make_nvp("Id", entity));
				setControllerType(type);
			}

			inline auto getType() const
			{
				return type;
			}

		private:
			ControllerType                    type = ControllerType::FPS;
			std::shared_ptr<CameraController> cameraController;
		};
	}
};        // namespace maple
