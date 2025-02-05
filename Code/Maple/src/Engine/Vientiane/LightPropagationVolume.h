//////////////////////////////////////////////////////////////////////////////
// This file is part of the Maple Engine                              		//
//////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Engine/Renderer/Renderer.h"
#include "Scene/System/ExecutePoint.h"
#include "RHI/Texture.h"
#include <vector>
#include <IconsMaterialDesignIcons.h>

namespace maple
{
	namespace component
	{
		struct LPVGrid
		{
			constexpr static char* ICON = ICON_MDI_TRACK_LIGHT;

			std::shared_ptr<Texture3D> lpvGridR;
			std::shared_ptr<Texture3D> lpvGridG;
			std::shared_ptr<Texture3D> lpvGridB;
			
			std::shared_ptr<Texture3D> lpvGeometryVolumeR;
			std::shared_ptr<Texture3D> lpvGeometryVolumeG;
			std::shared_ptr<Texture3D> lpvGeometryVolumeB;

			std::shared_ptr<Texture3D> lpvAccumulatorR;
			std::shared_ptr<Texture3D> lpvAccumulatorG;
			std::shared_ptr<Texture3D> lpvAccumulatorB;

			std::vector<std::shared_ptr<Texture3D>> lpvRs;
			std::vector<std::shared_ptr<Texture3D>> lpvGs;
			std::vector<std::shared_ptr<Texture3D>> lpvBs;

			int32_t propagateCount = 8;
			float cellSize = 1.f;
			float occlusionAmplifier = 1.0f;
			float indirectLightAttenuation = 1.f;
			bool debugAABB = false;
			bool showGeometry = false;
		};
	};

	namespace light_propagation_volume
	{
		auto registerLPV(ExecuteQueue& begin, ExecuteQueue& renderer, std::shared_ptr<ExecutePoint> executePoint) -> void;
		auto registerLPVDebug(ExecuteQueue& begin, ExecuteQueue& renderer, std::shared_ptr<ExecutePoint> executePoint) -> void;
	};
};        // namespace maple
