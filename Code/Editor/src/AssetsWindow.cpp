//////////////////////////////////////////////////////////////////////////////
// This file is part of the Maple Engine                              		//
//////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#	include "windows.h"
#endif
#include "AssetsWindow.h"
#include "Others/Console.h"

#include "Editor.h"
#include "Others/StringUtils.h"
#include "Engine/Quad2D.h"
#include "ImGui/ImGuiHelpers.h"
#include "RHI/Texture.h"
#include "Loaders/Loader.h"
#include "Window/NativeWindow.h"
#include "FileSystem/MeshResource.h"

#include <IconsMaterialDesignIcons.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <algorithm>
#include <cmath>
#include <filesystem>


namespace maple
{
#ifdef _WIN32
	std::string AssetsWindow::delimiter = "\\";
#else
	std::string AssetsWindow::delimiter = "/";
#endif

	AssetsWindow::AssetsWindow()
	{
		baseDirPath = ".";
		readDirectory(baseDirPath, &baseProjectDir);
		basePathLen                 = baseDirPath.length();
		currentDir                  = &baseProjectDir;
		baseProjectDir.absolutePath = baseDirPath;
		lastDir                     = currentDir;
		rootDir                     = currentDir;
	}

	auto AssetsWindow::onImGui() -> void
	{
		auto &editor = static_cast<Editor &>(*Application::get());
		ImGui::Begin(STATIC_NAME);
		{
			ImGui::Columns(2, "AB", true);

			ImGui::BeginChild("##folders_common");
			{
				renderNavigationBar();

				{
					ImGui::BeginChild("##folders");
					{
						for (auto &dir : rootDir->children)
							drawFolder(dir.second);
					}
					ImGui::EndChild();
				}

				if (ImGui::IsMouseDown(1))
				{
					ImGui::OpenPopup("window");
				}
			}

			ImGui::EndChild();

			if (ImGui::BeginDragDropTarget())
			{
				auto data = ImGui::AcceptDragDropPayload("selectable", ImGuiDragDropFlags_AcceptNoDrawDefaultRect);
				if (data)
				{
					std::string file = (char *) data->Data;
					if (this->move(file, movePath))
					{
						LOGV("Moved File: " + file + " to " + movePath);
						//currentDir = readDirectory(currentDirPath);
					}
					isDragging = false;
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::NextColumn();

			ImGui::BeginChild("##directory_structure");
			{
				{
					ImGui::BeginChild("##directory_navigation_bar", ImVec2(ImGui::GetColumnWidth(), 50));
					if (isInListView)
					{
						if (ImGui::Button(ICON_MDI_VIEW_GRID))
						{
							isInListView = !isInListView;
						}
						ImGui::SameLine();
					}
					else
					{
						if (ImGui::Button(ICON_MDI_VIEW_LIST))
						{
							isInListView = !isInListView;
						}
						ImGui::SameLine();
					}

					ImGui::TextUnformatted(ICON_MDI_MAGNIFY);

					ImGui::SameLine();

					filter.Draw("##Filter", ImGui::GetContentRegionAvail().x - ImGui::GetStyle().IndentSpacing);

					ImGui::EndChild();
				}

				{
					ImGui::BeginChild("##Scrolling");

					if (ImGui::IsMouseClicked(1))
					{
						ImGui::OpenPopup("AssetsWindow::PopupWindow");
					}
					popupWindow();

					int shownIndex = 0;

					float xAvail    = ImGui::GetContentRegionAvail().x;
					gridItemsPerRow = (int32_t) std::floor(xAvail / (95.0f * Application::get()->getWindow()->getScale()));
					gridItemsPerRow = std::max(1, gridItemsPerRow);

					bool clickAny   = false;
					bool isClickDir = false;
					for (auto &dir : currentDir->children)
					{
						if (!showHiddenFiles && StringUtils::isHiddenFile(dir.first))
						{
							continue;
						}

						if (filter.IsActive())
						{
							if (!filter.PassFilter(dir.first.c_str()))
							{
								continue;
							}
						}

						bool doubleClicked = drawFile(dir.second.get(), !dir.second->isFile, shownIndex, !isInListView, clickAny, isClickDir);

						if (doubleClicked && dir.second->isFile)
						{
							editor.setEditingResource(dir.second->absolutePath);
						}

						if (doubleClicked)
							break;
						shownIndex++;
					}

					if (ImGui::IsMouseClicked(0) && !clickAny && ImGui::IsWindowFocused())
					{
						editor.setSelected("");
					}

					ImGui::EndChild();
				}

				ImGui::EndChild();
			}

			if (ImGui::BeginDragDropTarget())
			{
				auto data = ImGui::AcceptDragDropPayload("selectable", ImGuiDragDropFlags_AcceptNoDrawDefaultRect);
				if (data)
				{
					std::string a = (char *) data->Data;
					if (this->move(a, movePath))
					{
						LOGI("Moved File: " + a + " to " + movePath);
					}
					isDragging = false;
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::End();
		}
	}

	auto AssetsWindow::drawFile(FileInfo *file, bool folder, int32_t shownIndex, bool gridView, bool &clickAny, bool &isClickDir) -> bool
	{
		bool  doubleClicked = false;
		auto &editor        = static_cast<Editor &>(*Application::get());

		if (gridView)
		{
			ImGui::BeginGroup();
			auto icon      = editor.getIcon(file->type);
			bool flipImage = true;        //opengl is true

			bool click = false;

			auto &uv = icon->getTexCoords();

			bool flipY = false;

			ImVec4 bgColor = {0, 0, 0, 0};

			if (editor.getSelectResource() == file->absolutePath)
			{
				bgColor = {1., 0.3f, 0.4f, 1.0f};
			}

			auto img = icon && icon->getTexture() ? icon->getTexture()->getHandle() : nullptr;

			if (ImGui::ImageButtonNoBg(img,
			                           {80 * editor.getWindow()->getScale(), 80 * editor.getWindow()->getScale()},
			                           ImVec2(uv[3].x, uv[1].y),
			                           ImVec2(uv[1].x, uv[3].y), -1, bgColor))
			{
				click = false;
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0) && file->isFile)
			{
				editor.setSelected(file->absolutePath);
				clickAny = true;
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			{
				doubleClicked = true;
			}

			auto &fname = file->fileName;
			auto  name  = stripExtras(fname);

			ImGui::TextWrapped("%s", name.c_str());
			ImGui::EndGroup();

			if ((shownIndex + 1) % gridItemsPerRow != 0)
				ImGui::SameLine();
		}
		else
		{
			ImGui::TextUnformatted(folder ? ICON_MDI_FOLDER : editor.getIconFontIcon(file->absolutePath));
			ImGui::SameLine();

			try {
				if (StringUtils::endWith(file->fileName, "obj"))
				{
					if (ImGui::TreeNode(file->fileName.c_str()))
					{
						std::vector < std::shared_ptr<IResource> > out;
						Loader::load(file->absolutePath, out);

						for (auto& res : out)
						{
							if(res->getResourceType() == FileType::Model)
							{
								auto meshes = std::static_pointer_cast<MeshResource>(res);

								for (auto mesh : meshes->getMeshes())
								{
									if (ImGui::Selectable(mesh.first.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
									{
										if (ImGui::IsMouseDoubleClicked(0))
										{
											doubleClicked = true;
										}
									}
								}
							}
						}
						ImGui::TreePop();
					}
				}
				else
				{
					if (ImGui::Selectable(file->fileName.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
					{
						if (ImGui::IsMouseDoubleClicked(0))
						{
							doubleClicked = true;
						}
					}
				}
			}
			catch (...) {}


		}

		if (doubleClicked)
		{
			if (folder)
			{
				lastDir    = file;
				currentDir = file;
			}

			else
			{
				//LOGW("Open File {0} did not implementation {1}", currentDir[dirIndex].absolutePath, __FUNCTION__);
				editor.openFileInPreview(file->absolutePath);
			}
		}

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			ImGui::TextUnformatted(editor.getIconFontIcon(file->absolutePath));
			ImGui::SameLine();
			ImGui::TextUnformatted(file->fileName.c_str());
			size_t size = sizeof(const char *) + strlen(file->absolutePath.c_str());
			ImGui::SetDragDropPayload("AssetFile", file->absolutePath.c_str(), size);
			isDragging = true;
			ImGui::EndDragDropSource();
		}

		return doubleClicked;
	}

	auto AssetsWindow::drawFolder(const std::shared_ptr<FileInfo> &dirInfo) -> void
	{
		ImGuiTreeNodeFlags nodeFlags = ((dirInfo.get() == currentDir) ? ImGuiTreeNodeFlags_Selected : 0);
		nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

		const ImColor TreeLineColor = ImColor(128, 128, 128, 128);
		const float   SmallOffsetX  = 6.0f;
		ImDrawList *  drawList      = ImGui::GetWindowDrawList();

		if (!dirInfo->isFile)
		{
			bool containsFolder = false;

			for (auto &file : dirInfo->children)
			{
				if (!file.second->isFile)
				{
					containsFolder = true;
					break;
				}
			}
			if (!containsFolder)
				nodeFlags |= ImGuiTreeNodeFlags_Leaf;

			static std::string folderIcon = ICON_MDI_FOLDER " ";

			bool isOpen = ImGui::TreeNodeEx((folderIcon + dirInfo->fileName).c_str(), nodeFlags);

			ImVec2 verticalLineStart = ImGui::GetCursorScreenPos();

			if (ImGui::IsItemClicked())
			{
				lastDir    = dirInfo.get();
				currentDir = dirInfo.get();
			}

			if (isOpen && containsFolder)
			{
				verticalLineStart.x += SmallOffsetX;        //to nicely line up with the arrow symbol
				ImVec2 verticalLineEnd = verticalLineStart;

				for (auto &folder : dirInfo->children)
				{
					if (!folder.second->isFile)
					{
						float HorizontalTreeLineSize = 16.0f;        //chosen arbitrarily
						auto  currentPos             = ImGui::GetCursorScreenPos();

						ImGui::Indent(10.0f);

						/*	auto dirDataTemp = readDirectory(dirData[i].absolutePath.c_str());

						bool containsFolderTemp = false;
						for (auto& file : dirDataTemp)
						{
							if (!file.isFile)
							{
								containsFolderTemp = true;
								break;
							}
						}
						if (containsFolderTemp)
							HorizontalTreeLineSize *= 0.5f;*/
						drawFolder(folder.second);

						const ImRect childRect(currentPos, ImVec2{currentPos.x, currentPos.y + ImGui::GetFontSize()});

						const float midpoint = (childRect.Min.y + childRect.Max.y) / 2.0f;
						drawList->AddLine(ImVec2(verticalLineStart.x, midpoint), ImVec2(verticalLineStart.x + HorizontalTreeLineSize, midpoint), TreeLineColor);
						verticalLineEnd.y = midpoint;

						ImGui::Unindent(10.0f);
					}
				}

				drawList->AddLine(verticalLineStart, verticalLineEnd, TreeLineColor);

				ImGui::TreePop();
			}

			if (isOpen && !containsFolder)
				ImGui::TreePop();
		}

		if (isDragging && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
		{
			movePath = dirInfo->absolutePath.c_str();
		}
	}

	auto AssetsWindow::renderNavigationBar() -> void
	{
		ImGui::BeginChild("##directory_navigation_bar", ImVec2(ImGui::GetColumnWidth(), 50));
		{
			if (ImGui::Button(ICON_MDI_ARROW_LEFT))
			{
				if (currentDir->parent != nullptr)
				{
					lastDir    = currentDir;
					currentDir = currentDir->parent;
				}
			}
			ImGui::SameLine();
			if (ImGui::Button(ICON_MDI_ARROW_RIGHT))
			{
				if (lastDir != nullptr)
					currentDir = lastDir;
			}
			ImGui::SameLine();

			ImGui::TextUnformatted(currentDir->absolutePath.c_str());

			ImGui::SameLine();
		}

		ImGui::EndChild();
	}

	auto AssetsWindow::renderBottom() -> void
	{
		ImGui::BeginChild("##nav", ImVec2(ImGui::GetColumnWidth() - 12, 23));
		{
			ImGui::EndChild();
		}
	}

	auto AssetsWindow::getDirectories(const std::string &path) -> void
	{
		directoryCount = 0;
		size_t start;
		size_t end = 0;

		while ((start = path.find_first_not_of(delimiter.c_str(), end)) != std::string::npos)
		{
			end                         = path.find(delimiter.c_str(), start);
			directories[directoryCount] = path.substr(start, end - start);
			directoryCount++;
		}
	}

	auto AssetsWindow::popupWindow() -> void
	{
		if (ImGui::BeginPopupContextWindow("AssetsWindow::PopupWindow"))
		{
			if (ImGui::Selectable("Refresh current folder"))
			{
				currentDir->children.clear();
				readDirectory(currentDir->absolutePath, currentDir);
			}

			if (ImGui::Selectable("Add Material"))
			{
				File::create(currentDir->absolutePath + "/default.material");
			}

			ImGui::EndPopup();
		}
	}

	auto AssetsWindow::parseFilename(const std::string &str, const char delim, std::vector<std::string> &out) -> std::string
	{
		size_t start;
		size_t end = 0;

		while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
		{
			end = str.find(delim, start);
			out.push_back(str.substr(start, end - start));
		}

		return out[out.size() - 1];
	}

	auto AssetsWindow::parseFiletype(const std::string &filename) -> std::string
	{
		size_t                   start;
		size_t                   end = 0;
		std::vector<std::string> out;

		while ((start = filename.find_first_not_of(".", end)) != std::string::npos)
		{
			end = filename.find(".", start);
			out.push_back(filename.substr(start, end - start));
		}

		return out[out.size() - 1];
	}

	auto AssetsWindow::readDirectory(const std::string &path, FileInfo *parent) -> void
	{
		for (const auto &entry : std::filesystem::directory_iterator(path))
		{
			bool isDir = std::filesystem::is_directory(entry);

			std::vector<std::string> test;
			const char del  = *delimiter.c_str();

			std::string dirData;
			try
			{
				dirData = parseFilename(entry.path().string(), del, test);
			}
			catch (...)
			{
				LOGW("{0} : catch an error", __FUNCTION__);
				continue;
			}
				
			auto fileExt = parseFiletype(dirData);

			auto fileInfo          = std::make_shared<FileInfo>();
			fileInfo->fileName     = dirData;
			fileInfo->fileType     = fileExt;
			fileInfo->absolutePath = entry.path().string();
			fileInfo->isFile       = !isDir;
			fileInfo->parent       = parent;
			if (isDir)
			{
				fileInfo->type = FileType::Folder;
				readDirectory(entry.path().string(), fileInfo.get());
			}
			else
			{
				fileInfo->type = File::getFileType(dirData);
			}
			parent->children[dirData] = fileInfo;
		}
	}

	auto AssetsWindow::getParentPath(const std::string &path) -> std::string
	{
		auto p = std::filesystem::path(path);
		return p.parent_path().string();
	}

	auto AssetsWindow::searchFiles(const std::string &query) -> std::vector<std::string>
	{
		return std::vector<std::string>();
	}

	auto AssetsWindow::move(const std::string &filePath, const std::string &movePath) -> bool
	{
		return false;
	}

	auto AssetsWindow::stripExtras(const std::string &filename) -> std::string
	{
		if (filename.length() > 11)
		{
			auto cutFilename = filename.substr(0, 8) + "...";
			return cutFilename;
		}

		std::string file;

		for (int32_t i = 0; i < (12 - filename.length()) / 2; i++)
		{
			file += " ";
		}

		return file + filename;
	}

};        // namespace maple
