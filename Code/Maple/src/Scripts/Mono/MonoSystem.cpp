//////////////////////////////////////////////////////////////////////////////
// This file is part of the Maple Engine                              		//
//////////////////////////////////////////////////////////////////////////////
#include "Mono.h"
#include "MonoSystem.h"
#include "MonoHelper.h"
#include "MonoExporter.h"
#include "MonoScript.h"
#include "MonoVirtualMachine.h"

#include "Scene/Scene.h"
#include "Others/StringUtils.h"
#include "MonoComponent.h"
#include "MonoScriptInstance.h"

#include "Scene/Component/Transform.h"
#include "Scene/Entity/Entity.h"
#include "Application.h"


namespace maple
{
	auto MonoSystem::onInit() -> void
	{
		MonoVirtualMachine::get()->loadAssembly("./", "MapleLibrary.dll");
		//MonoVirtualMachine::get()->loadAssembly("./", "MapleAssembly.dll");
		handler.compileHandler = [&](RecompileScriptsEvent * event) {
			auto view = event->scene->getRegistry().view<component::MonoComponent>();
			for (auto v : view)
			{
				auto& mono = event->scene->getRegistry().get<component::MonoComponent>(v);
				for (auto& script : mono.getScripts())
				{
					script.second->loadFunction();
				}
			}
			return true;
		};
		Application::get()->getEventDispatcher().addEventHandler(&handler);
	}

	auto MonoSystem::onStart(Scene* scene) -> void
	{
		if (Application::get()->getEditorState() == EditorState::Play)
		{
			auto view = scene->getRegistry().view<component::MonoComponent>();
			for (auto v : view)
			{
				auto& mono = scene->getRegistry().get<component::MonoComponent>(v);
				for (auto& script : mono.getScripts())
				{
					script.second->onStart(this);
				}
			}
		}
	}


	auto MonoSystem::onUpdate(float dt, Scene* scene)-> void
	{
		if (Application::get()->getEditorState() == EditorState::Play) 
		{
			auto view = scene->getRegistry().view<component::MonoComponent>();
			for (auto v : view)
			{
				auto& mono = scene->getRegistry().get<component::MonoComponent>(v);
				for (auto & script : mono.getScripts())
				{
					script.second->onUpdate(dt,this);
				}
			}
		}
	}

	auto MonoSystem::onImGui() -> void
	{

	}


	MonoSystem::~MonoSystem()
	{
		
	}

};