//////////////////////////////////////////////////////////////////////////////
// This file is part of the Maple Engine                              		//
//////////////////////////////////////////////////////////////////////////////
#include "ImGuiSystem.h"
#include "Application.h"
#include "Window/WindowWin.h"

#include "Engine/Profiler.h"
#include "ImGuiHelpers.h"
#include "RHI/ImGuiRenderer.h"

#include <IconsMaterialDesignIcons.h>
#include <MaterialDesign.inl>
#include <RobotoRegular.inl>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_notify.h>
#include <tahoma.h>

namespace maple
{
	ImGuiSystem::ImGuiSystem(bool clearScreen) :
	    clearScreen(clearScreen)
	{
	}
	ImGuiSystem::~ImGuiSystem()
	{
	}

	auto ImGuiSystem::newFrame(const Timestep &step) -> void
	{
		imguiRender->newFrame(step);
	}

	auto ImGuiSystem::onInit() -> void
	{
		PROFILE_FUNCTION();
		Application::get()->getEventDispatcher().addEventHandler(&handler);

		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO &io = ImGui::GetIO();

		io.DisplaySize = ImVec2(
		    static_cast<float>(Application::getWindow()->getWidth()),
		    static_cast<float>(Application::getWindow()->getHeight()));

		addIcon();

		imguiRender = ImGuiRenderer::create(
		    Application::getWindow()->getWidth(),
		    Application::getWindow()->getHeight(),
		    clearScreen);

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;

		imguiRender->init();

		/*io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.ConfigWindowsMoveFromTitleBarOnly = true;*/
		setTheme();
	}

	auto ImGuiSystem::onUpdate(float dt, Scene *scene) -> void
	{
		PROFILE_FUNCTION();
		ImGui::Render();
	}

	auto ImGuiSystem::onRender(Scene *scene) -> void
	{
		PROFILE_FUNCTION();
		imguiRender->render(Application::getGraphicsContext()->getSwapChain()->getCurrentCommandBuffer());
	}

	auto ImGuiSystem::addIcon() -> void
	{
		PROFILE_FUNCTION();
		ImGuiIO &            io             = ImGui::GetIO();
		static const ImWchar icons_ranges[] = {ICON_MIN_MDI, ICON_MAX_MDI, 0};
		//io.Fonts->AddFontFromFileTTF("fonts/simsun.ttf", 16.f, NULL, io.Fonts->GetGlyphRangesChineseFull());
		//	io.Fonts->AddFontDefault();

		ImFontConfig iconsConfig;
		iconsConfig.MergeMode   = false;
		iconsConfig.PixelSnapH  = true;
		iconsConfig.OversampleH = iconsConfig.OversampleV = 1;
		iconsConfig.GlyphMinAdvanceX                      = 4.0f;
		iconsConfig.SizePixels                            = 16.f;

		static const ImWchar ranges[] = {
		    0x0020,
		    0x00FF,
		    0x0400,
		    0x044F,
		    0,
		};

		io.Fonts->AddFontFromMemoryCompressedTTF(
		    RobotoRegular_compressed_data,
		    RobotoRegular_compressed_size, 16.f,
		    &iconsConfig,
		    ranges);

		iconsConfig.MergeMode     = true;
		iconsConfig.PixelSnapH    = true;
		iconsConfig.GlyphOffset.y = 1.0f;
		iconsConfig.OversampleH = iconsConfig.OversampleV = 1;
		iconsConfig.PixelSnapH                            = true;
		iconsConfig.SizePixels                            = 16.f;
		io.Fonts->AddFontFromMemoryCompressedTTF(
		    MaterialDesign_compressed_data,
		    MaterialDesign_compressed_size, 16.f,
		    &iconsConfig, icons_ranges);

		io.Fonts->AddFontDefault();

		ImFontConfig font_cfg;
		font_cfg.FontDataOwnedByAtlas = false;
		io.Fonts->AddFontFromMemoryTTF((void *) tahoma, sizeof(tahoma), 16.f, &font_cfg);

		// Initialize notify
		ImGui::MergeIconsWithLatestFont(16.f, false);
	}

	auto ImGuiSystem::onResize(uint32_t w, uint32_t h) -> void
	{
		PROFILE_FUNCTION();
		imguiRender->onResize(w, h);
	}

	auto ImGuiSystem::setTheme() -> void
	{
	}


	namespace on_imgui 
	{
		auto registerImGui(std::shared_ptr<ExecutePoint> executePoint) -> void
		{
		}
	}

};        // namespace maple
