//////////////////////////////////////////////////////////////////////////////
// This file is part of the Maple Engine                              		//
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace maple
{
	namespace component 
	{
		class LightProbe : public Component
		{
		public:
			constexpr static char* ICON = ICON_MDI_LIGHTBULB;
		};
	}
};        // namespace maple
