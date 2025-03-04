//////////////////////////////////////////////////////////////////////////////
// This file is part of the Maple Engine                              		//
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Renderer.h"
#include "Scene/System/ExecutePoint.h"

namespace maple
{
	namespace cloud_renderer
	{
		auto registerCloudRenderer(ExecuteQueue& begin, ExecuteQueue& renderer, std::shared_ptr<ExecutePoint> executePoint) -> void;
	}
}        // namespace maple
