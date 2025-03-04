// for the license, see the end of the file
#pragma once
#include "IconsMaterialDesignIcons.h"
#include <map>
#include <set>
#include <functional>
#include <string>

#include <entt/entt.hpp>
#include <imgui.h>

#ifndef MM_IEEE_ASSERT
#define MM_IEEE_ASSERT(x) assert(x)
#endif

#define MM_IEEE_IMGUI_PAYLOAD_TYPE_ENTITY "MM_IEEE_ENTITY"

#ifndef MM_IEEE_ENTITY_WIDGET
#define MM_IEEE_ENTITY_WIDGET ::MM::EntityWidget
#endif

namespace MM {

	template <class EntityType>
	inline void EntityWidget(EntityType& e, entt::basic_registry<EntityType>& reg, bool dropTarget = false)
	{
		ImGui::PushID(static_cast<int>(entt::to_integral(e)));

		if (reg.valid(e)) {
			ImGui::Text("ID: %d", entt::to_integral(e));
		}
		else {
			ImGui::Text("Invalid Entity");
		}

		if (reg.valid(e)) {
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
				ImGui::SetDragDropPayload(MM_IEEE_IMGUI_PAYLOAD_TYPE_ENTITY, &e, sizeof(e));
				ImGui::Text("ID: %d", entt::to_integral(e));
				ImGui::EndDragDropSource();
			}
		}

		if (dropTarget && ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(MM_IEEE_IMGUI_PAYLOAD_TYPE_ENTITY)) {
				e = *(EntityType*)payload->Data;
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::PopID();
	}

	template <class Component, class EntityType>
	void ComponentEditorWidget([[maybe_unused]] entt::basic_registry<EntityType>& registry, [[maybe_unused]] EntityType entity) {}

	template <class TComponent, class EntityType>
	void ComponentAddAction(entt::basic_registry<EntityType>& registry, EntityType entity)
	{
		auto & ent = registry.template emplace<TComponent>(entity);
		if constexpr (std::is_base_of<maple::component::Component, TComponent >::value)
		{
			ent.setEntity(entity);
		}
	}

	template <class Component, class EntityType>
	void ComponentRemoveAction(entt::basic_registry<EntityType>& registry, EntityType entity)
	{
		registry.template remove<Component>(entity);
	}

	template <class EntityType>
	class EntityEditor {
	public:
		using Registry = entt::basic_registry<EntityType>;
		using ComponentTypeID = entt::id_type;

		std::function<void(Registry& registry, EntityType& e)> createCallback;

		std::function<void(const std::string &, Registry& registry, EntityType& e)> acceptFile;


		struct ComponentInfo {
			using Callback = std::function<void(Registry&, EntityType)>;
			using Callback2 = std::function<void(Registry&, EntityType)>;

			std::string name;
			Callback widget, create, destroy;
			bool showInEditor;
			bool hasChildren;
			Callback2 childrenDraw;
		};

		bool show_window = true;

	private:
		std::map<ComponentTypeID, ComponentInfo> component_infos;

		bool entityHasComponent(Registry& registry, EntityType& entity, ComponentTypeID type_id)
		{
			ComponentTypeID type[] = { type_id };
			return registry.runtime_view(std::cbegin(type), std::cend(type)).contains(entity);
		}

	public:

		void clear() 
		{
			component_infos.clear();
		}

		template <class Component>
		ComponentInfo& registerComponent(const ComponentInfo& component_info)
		{
			//auto index = entt::type_hash<Component>::value();
			auto index = entt::type_info<Component>::id();
			auto insert_info = component_infos.insert_or_assign(index, component_info);
			MM_IEEE_ASSERT(insert_info.second);
			return std::get<ComponentInfo>(*insert_info.first);
		}

		template <class Component>
		ComponentInfo& registerComponent(const std::string& name, typename ComponentInfo::Callback widget)
		{
			return registerComponent<Component>(ComponentInfo{
				name,
				widget,
				ComponentAddAction<Component, EntityType>,
				ComponentRemoveAction<Component, EntityType>,
				true,
				false,
				nullptr
				});
		}

		template <class Component>
		ComponentInfo& registerComponent(const std::string& name,bool showInEditor)
		{
			auto & info = registerComponent<Component>(name, ComponentEditorWidget<Component, EntityType>);
			info.showInEditor = showInEditor;
			return info;
		}

		void renderEditor(Registry& registry, EntityType& e)
		{
			ImGui::TextUnformatted("Editing:");
			ImGui::SameLine();
			
			MM_IEEE_ENTITY_WIDGET(e, registry, true);


			


			/*if (ImGui::Button("New")) {
				e = registry.create();
			}*/
			/*if (registry.valid(e)) {
				ImGui::SameLine();

				

				ImGui::Dummy({ 10, 0 }); // space destroy a bit, to not accidentally click it
				ImGui::SameLine();

				// red button
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.65f, 0.15f, 0.15f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.f, 0.2f, 0.2f, 1.f));
				if (ImGui::Button("Destroy")) {
					registry.destroy(e);
					e = entt::null;
				}
				ImGui::PopStyleColor(3);
			}*/

			ImGui::Separator();
			if (registry.valid(e)) {
				ImGui::PushID(static_cast<int>(entt::to_integral(e)));
				std::map<ComponentTypeID, ComponentInfo> has_not;

				for (auto& [component_type_id, ci] : component_infos) {
					if (!ci.hasChildren) {
						if (entityHasComponent(registry, e, component_type_id)) {
							ImGui::PushID(component_type_id);
							if (ImGui::Button("-")) {
								ci.destroy(registry, e);
								ImGui::PopID();
								continue; // early out to prevent access to deleted data
							}
							else {
								ImGui::SameLine();
							}

							if (ImGui::CollapsingHeader(ci.name.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
								ImGui::Indent(30.f);
								ImGui::PushID("Widget");
								ci.widget(registry, e);
								ImGui::PopID();
								ImGui::Unindent(30.f);
							}
							ImGui::PopID();
						}
						else {
							has_not[component_type_id] = ci;
						}
					}
					else 
					{
						ci.childrenDraw(registry, e);
					}
				}

			

				if (!has_not.empty()) {
					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
					if (ImGui::Button("+ Add Component")) {
						ImGui::OpenPopup("Add Component");
					}

					if (ImGui::BeginDragDropTarget()) {
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AssetFile")) {
							auto filePath = std::string(reinterpret_cast<const char*>(payload->Data));
							acceptFile(filePath, registry, e);
						}
						ImGui::EndDragDropTarget();
					}


					ImGui::PopStyleVar();

					ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 10);
					if (ImGui::BeginPopup("Add Component")) {
						ImGui::TextUnformatted("Available:");
						ImGui::Separator();

						for (auto& [component_type_id, ci] : has_not) {
							if (ci.showInEditor) 
							{
								ImGui::PushID(component_type_id);
								if (ImGui::Selectable(ci.name.c_str())) {
									ci.create(registry, e);
									if (createCallback) {
										createCallback(registry, e);
									}
								}
								ImGui::PopID();
							}
						}
						ImGui::EndPopup();
					}
					ImGui::PopStyleVar();
				}
				ImGui::PopID();
			}
		}

		void renderEntityList(Registry& registry, std::set<ComponentTypeID>& comp_list)
		{
			ImGui::Text("Components Filter:");
			ImGui::SameLine();
			if (ImGui::SmallButton("clear")) {
				comp_list.clear();
			}

			ImGui::Indent();

			for (const auto& [component_type_id, ci] : component_infos) {
				bool is_in_list = comp_list.count(component_type_id);
				bool active = is_in_list;

				ImGui::Checkbox(ci.name.c_str(), &active);

				if (is_in_list && !active) { // remove
					comp_list.erase(component_type_id);
				}
				else if (!is_in_list && active) { // add
					comp_list.emplace(component_type_id);
				}
			}

			ImGui::Unindent();
			ImGui::Separator();

			if (comp_list.empty()) {
				ImGui::Text("Orphans:");
				registry.orphans([&registry](auto e) {
					MM_IEEE_ENTITY_WIDGET(e, registry, false);
					});
			}
			else {
				auto view = registry.runtime_view(comp_list.begin(), comp_list.end());
				ImGui::Text("%lu Entities Matching:", view.size_hint());

				if (ImGui::BeginChild("entity list")) {
					for (auto e : view) {
						MM_IEEE_ENTITY_WIDGET(e, registry, false);
					}
				}
				ImGui::EndChild();
			}
		}

		[[deprecated("Use renderEditor() instead. And manage the window yourself.")]]
		void render(Registry& registry, EntityType& e)
		{
			if (show_window) {
				if (ImGui::Begin("Entity Editor", &show_window)) {
					renderEditor(registry, e);
				}
				ImGui::End();
			}
		}

		// displays both, editor and list
		// uses static internally, use only as a quick way to get going!
		void renderSimpleCombo(Registry& registry, EntityType& e)
		{
			if (show_window) {
				ImGui::SetNextWindowSize(ImVec2(550, 400), ImGuiCond_FirstUseEver);
				if (ImGui::Begin("Entity Editor", &show_window)) {
					if (ImGui::BeginChild("list", { 200, 0 }, true)) {
						static std::set<ComponentTypeID> comp_list;
						renderEntityList(registry, comp_list);
					}
					ImGui::EndChild();

					ImGui::SameLine();

					if (ImGui::BeginChild("editor")) {
						renderEditor(registry, e);
					}
					ImGui::EndChild();

				}
				ImGui::End();
			}
		}


		void addCreateCallback(const std::function<void(Registry& registry, EntityType& e)> & call) {
			createCallback = call;
		}
	};

}