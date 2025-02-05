//////////////////////////////////////////////////////////////////////////////
// This file is part of the Maple Engine                              		//
//////////////////////////////////////////////////////////////////////////////

#include "Console.h"
#include "Scripts/Lua/LuaVirtualMachine.h"
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}
#include <LuaBridge/LuaBridge.h>

namespace maple 
{
	auto LogExport::exportLua(lua_State* L) -> void
	{
		luabridge::getGlobalNamespace(L)
			.addFunction("LOGE", std::function <void(const std::string&)>([](const std::string& str) {
			LOGE(str);
				}))
			.addFunction("LOGV", std::function <void(const std::string&)>([](const std::string& str) {
					LOGV(str);
				}))
					.addFunction("LOGI", std::function <void(const std::string&)>([](const std::string& str) {
					LOGI(str);
						}))
					.addFunction("LOGC", std::function <void(const std::string&)>([](const std::string& str) {
							LOGC(str);
						}));
	}
};
