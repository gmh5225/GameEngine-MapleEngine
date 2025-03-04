//////////////////////////////////////////////////////////////////////////////
// This file is part of the Maple Engine                              		//
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Mono.h"
#include <string>
#include <vector>

namespace maple
{
	class MAPLE_EXPORT MapleMonoObject
	{
	public:
		MapleMonoObject(MonoObject* rawPtr, MapleMonoClass * clazz);
		~MapleMonoObject();
		inline auto getRawPtr() { return rawPtr; }
		inline auto getRawPtr() const { return rawPtr; }

		auto setValue(void* ptr, const std::string& name)->void;
		auto construct() -> void;//call default ctor

	private:
		MonoObject* rawPtr = nullptr;
		MapleMonoClass* clazz = nullptr;
	};
};