#include <cstdlib>
#include <iostream>
#include <regex>
#include <SDKDDKVer.h>
#include <afxdlgs.h>

#include "Show.h"
#include "APPContext.h"
#include "util/Pixdesc.h"
#include "imGuiExt/imgui_memory_editor.h"
#include "ConvertFile.h"

namespace show {
	struct ShowContext {
		bool show_main_menu_bar;
		bool show_set_image_window;
		bool show_convert_window;
		bool show_dockspace;
		bool show_image_window;
		bool show_set_image_view_window;
		bool show_image_data_window;
	};

	static ShowContext ctx;

	static void ShowMainMenuBar();
	static void ShowMenuFile();

	static void HelpMarker(const char* desc)
	{
		ImGui::TextDisabled("(help)");
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort) && ImGui::BeginTooltip())
		{
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	static void ShowMainMenuBar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Window"))
			{
				if (ImGui::MenuItem("open set image window"))
				{
					ctx.show_set_image_window = true;
				}
				if (ImGui::MenuItem("open convert window"))
				{
					ctx.show_convert_window = true;
				}

				if (ImGui::MenuItem("open view image window"))
				{
					ctx.show_set_image_view_window = true;
				}

				if (ImGui::MenuItem("open viewing image data window"))
				{
					ctx.show_image_data_window = true;
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Layout issues"))
			{
				ImGui::MenuItem("Each window is independent!Users can select the window name with the mouse and drag the window to layout according to their preferences.");
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				ImGui::MenuItem("Supported functions: Display and play raw images, and also view the memory content of the images.");
				ImGui::MenuItem("The button that is not easy to understand will have a help icon (help) behind it, and the introduction will be automatically displayed when the mouse is placed.");
				ImGui::MenuItem("If you find any bugs or need to add image formats and features, please contact the author.");
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Author"))
			{
				ImGui::MenuItem("Author:Liujuntao");
				ImGui::MenuItem("Email:juntao.liu@metax-tech.com  15136357701@163.com");
				ImGui::MenuItem("If you have any other format or function requirements, please contact me.");
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Version"))
			{
				ImGui::MenuItem("Version:1.0.4");

				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}

	static void ShowDockSpace()
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	}

	static bool AutoSetImageFormat(const std::string& filename)
	{

		for (int i = 0; i < PIX_FMT_NB; i++)
		{
			const PixFmtDescriptor* desc = pix_fmt_desc_get((enum PixelFormat)i);

			std::regex regexStr(desc->name);
			std::smatch matchResult;

			if (std::regex_search(filename, matchResult, regexStr)) {
				ctx::SetFormat((enum PixelFormat)i);
				return true;
			}

			std::string name(desc->name);
			std::transform(name.begin(), name.end(), name.begin(), tolower);
			regexStr = name;

			if (std::regex_search(filename, matchResult, regexStr)) {
				ctx::SetFormat((enum PixelFormat)i);
				return true;
			}
		}
		return false;
	}

	static bool AutoSetImageWH(const std::string& filename)
	{
		std::regex regexStr("(\\d+)[xX_-](\\d+)");
		std::smatch matchResult;

		if (std::regex_search(filename, matchResult, regexStr)) {
			ctx::SetWidth(stoi(matchResult[1]));
			ctx::SetHeight(stoi(matchResult[2]));
			return true;
		}
		regexStr = "1080[pP]";
		if (std::regex_search(filename, matchResult, regexStr)) {
			ctx::SetWidth(1920);
			ctx::SetHeight(1080);
			return true;
		}

		regexStr = "720[pP]";
		if (std::regex_search(filename, matchResult, regexStr)) {
			ctx::SetWidth(1280);
			ctx::SetHeight(720);
			return true;
		}
		return false;
	}

	static void ShowSetImageWindow()
	{

		ImGui::Begin("set image param", &ctx.show_set_image_window, ImGuiWindowFlags_HorizontalScrollbar);

		ImGui::BeginDisabled(ctx::GetPlayFlag() && *ctx::GetPlayModePtr());
		if (ctx::GetPlayFlag() && *ctx::GetPlayModePtr()) {
			ImGui::Text("Do not allow changes during playback.");
		}
		if (ImGui::Button("open file")) {
			CFileDialog dlg(TRUE);
			if (dlg.DoModal() != IDOK)
			{
				AfxMessageBox(_T("打开图像失败！"));
			}
			else
			{
				std::string path(CT2A(dlg.GetPathName().GetString()));
				std::string fileName(CT2A(dlg.GetFileName().GetString()));
				ctx::OpenFile(path);
				if (AutoSetImageWH(fileName) && AutoSetImageFormat(fileName))
				{
					ctx.show_image_window = true;
				}
				ctx::SetUpDateTexFlag(true);

			}
		}
		ImGui::SameLine();
		HelpMarker("If the path is in Chinese, the following path will display???, Because no Chinese font has been added, it does not affect normal use.");
		ImGui::SameLine();
		ImGui::Text(ctx::GetFilePath().c_str());

		ImGui::SetNextItemWidth(100);
		if (ImGui::InputInt("width", ctx::GetWidthPtr()))
		{
			ctx::SetUpDateTexFlag(true);
			ctx.show_image_window = true;
		}
		ImGui::SameLine();
		HelpMarker("Less than or equal to zero is invalid.");
		ImGui::SetNextItemWidth(100);
		if (ImGui::InputInt("height", ctx::GetHeightPtr()))
		{
			ctx::SetUpDateTexFlag(true);
			ctx.show_image_window = true;
		}
		ImGui::SameLine();
		HelpMarker("Less than or equal to zero is invalid.");

		const char* formatNameArray[PIX_FMT_NB];
		for (int i = 0; i < PIX_FMT_NB; i++)
		{
			const PixFmtDescriptor* desc = pix_fmt_desc_get((enum PixelFormat)i);
			formatNameArray[i] = desc->name;
		}
		ImGui::SetNextItemWidth(150);
		if (ImGui::Combo("format", ctx::GetFormatPtr(), formatNameArray, IM_ARRAYSIZE(formatNameArray)))
		{
			ctx::SetUpDateTexFlag(true);
			ctx.show_image_window = true;
		}

		const char* tiledNameArray[TILED_NB];
		for (int i = 0; i < TILED_NB; i++)
		{
			const TiledDescriptor* desc = tiled_desc_get((enum Tiled)i);
			tiledNameArray[i] = desc->name;
		}
		ImGui::SetNextItemWidth(150);
		if (ImGui::Combo("tiled", ctx::GetTiledPtr(), tiledNameArray, IM_ARRAYSIZE(tiledNameArray))) {
			ctx::SetUpDateTexFlag(true);
			ctx.show_image_window = true;
		}
		ImGui::SameLine();
		HelpMarker("Please select (TILED_LINEAR) if you do not understand.");
		ImGui::Begin("convert image", &ctx.show_convert_window, ImGuiWindowFlags_HorizontalScrollbar);
		const char* targetFormatNameArray[PIX_FMT_NB];
		for (int i = 0; i < PIX_FMT_NB; i++)
		{
			const PixFmtDescriptor* desc = pix_fmt_desc_get((enum PixelFormat)i);
			targetFormatNameArray[i] = desc->name;
		}
		ImGui::SetNextItemWidth(150);
		static int targetFormat = 0;
		ImGui::Combo("target format", &targetFormat, targetFormatNameArray, IM_ARRAYSIZE(targetFormatNameArray));

		const char* targetTiledNameArray[TILED_NB];
		for (int i = 0; i < TILED_NB; i++)
		{
			const TiledDescriptor* desc = tiled_desc_get((enum Tiled)i);
			targetTiledNameArray[i] = desc->name;
		}
		ImGui::SetNextItemWidth(150);
		static int targetTiled = 0;
		ImGui::Combo("target tiled", &targetTiled, targetTiledNameArray, IM_ARRAYSIZE(targetTiledNameArray));
		if (ImGui::Button("convert and save file")) {
			CFileDialog dlg(false);
			if (dlg.DoModal() != IDOK)
			{
				AfxMessageBox(_T("保存文件失败！"));
			}
			else
			{
				std::string path(CT2A(dlg.GetPathName().GetString()));

				convertFile::FileConvert::FileParam src = { .path = ctx::GetFilePath(),
					.w = ctx::GetWidth(), .h = ctx::GetHeight(),
					.format = (enum PixelFormat)*ctx::GetFormatPtr(),.tiled = (enum Tiled)*ctx::GetTiledPtr() };
				convertFile::FileConvert::FileParam dst = { .path = path ,.w = ctx::GetWidth(), .h = ctx::GetHeight(),
				.format = (enum PixelFormat)targetFormat,.tiled = (enum Tiled)targetTiled };
				convertFile::FileConvert fileC(src, dst);
				fileC.Convert();
				AfxMessageBox(_T("转换完成"));
			}
		}
		ImGui::End();
		ImGui::EndDisabled();
		ImGui::End();

	}

	static void ShowSetImageViewWindow()
	{
		ImGui::Begin("view param", &ctx.show_set_image_view_window, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::Checkbox("adaptive window", ctx::GetAutoResizeFlagPtr());
		ImGui::SameLine();
		HelpMarker("Enable: Auto zoom based on image window.\nDisable: Display according to the actual aspect ratio, and you can zoom in or out according to the \"Zoom\" button below.");
		if (!(*ctx::GetAutoResizeFlagPtr())) {
			ImGui::SetNextItemWidth(200);
			if (ImGui::DragInt("zoom", ctx::GetScalingPtr(), 2.0F, 1, 500, "%d%%", ImGuiSliderFlags_AlwaysClamp))
				ctx::SetUpDateTexFlag(true);
			ImGui::SameLine();
			HelpMarker("Zoom the image.\nYou can double-click to input, or drag left or right after selecting with the mouse.");
		}
		ImGui::Separator();
		ImGui::Checkbox("enable play mode", ctx::GetPlayModePtr());
		ImGui::SameLine();
		HelpMarker("If the file is a continuous image, it can be played after being enabled.");
		if (*ctx::GetPlayModePtr()) {
			ImGui::SetNextItemWidth(200);
			ImGui::DragInt("FPS", ctx::GetFPSPtr(), 0.5F, 1, 30, "%d/s", ImGuiSliderFlags_AlwaysClamp);
			ImGui::SameLine();
			HelpMarker("It is not necessarily possible to truly reach the set value.\nYou can double-click to input, or drag left or right after selecting with the mouse.");

			if (*ctx::GetPlayModePtr() && ctx::GetPlayFlag()) {
				ImGuiIO& io = ImGui::GetIO(); (void)io;
				float Framerate = (float)*ctx::GetFPSPtr();
				if (Framerate > io.Framerate)
					Framerate = io.Framerate;
				ImGui::Text("Total Frames:,Real frame rate: %.3f ms/frame (%.1f FPS)", ctx::GetFrameTotal(), 1000.0f / Framerate, Framerate);
			}

			if (ImGui::Button("play"))
				ctx::SetPlayFlag(true);
			ImGui::SameLine();
			if (ImGui::Button("pause"))
				ctx::SetPlayFlag(false);

			ImGui::SetNextItemWidth(200);
			if (ImGui::SliderInt("progress bar", ctx::GetProgressBarPtr(), 0, 100, "%d%%"))
			{
				ctx::SetProgressBarFlag(true);
				ctx::SetPlayFlag(true);
			}
			ImGui::SameLine();
			HelpMarker("Set and display playback progress.\nClick or select with the mouse and drag.");
		}
		ImGui::End();

	}

	static void ShowImageWindow()
	{
		if (ctx::GetUpDateTexFlag() || ctx::GetPlayFlag())
			ctx::UpDateTexture();

		ImGui::Begin("image", &ctx.show_image_window,
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_HorizontalScrollbar);
		if (*ctx::GetAutoResizeFlagPtr())
			ImGui::Image((ImTextureID)ctx::GetTextureID(), ImGui::GetContentRegionAvail());
		else
			ImGui::Image((ImTextureID)ctx::GetTextureID(), ImVec2((float)ctx::GetWidth() * (*ctx::GetScalingPtr()) / (float)100, (float)ctx::GetHeight() * (*ctx::GetScalingPtr()) / (float)100));
		ImGui::End();
	}

	static void ShowImageDataWindow()
	{
		static MemoryEditor mem_edit;
		mem_edit.DrawWindow("image memory data", ctx::GetMemBuffer(), ctx::GetBufferSize(), &ctx.show_image_data_window);
	}

	void DropFile(std::string path)
	{
		ctx::OpenFile(path);
		if (AutoSetImageWH(path) && AutoSetImageFormat(path))
		{
			ctx.show_set_image_window = true;
			ctx.show_image_window = true;
		}
		ctx::SetUpDateTexFlag(true);
	}

	void SetStyle(ImGuiIO& io)
	{
		ImGui::StyleColorsLight();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			style.FrameRounding = 6.0f;
			style.FrameBorderSize = 1.0f;
		}

	}

	void ShowMainWindow(bool* p_open)
	{
		IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context.");
		static bool init = false;
		if (!init) {
			ctx.show_main_menu_bar = true;
			ctx.show_set_image_window = true;
			ctx.show_dockspace = true;
			ctx.show_set_image_view_window = true;
			ctx.show_convert_window = true;
			ctx.show_image_window = true;
			ctx.show_image_data_window = true;
			init = true;
		}

		if (ctx.show_main_menu_bar)             ShowMainMenuBar();
		if (ctx.show_dockspace)                 ShowDockSpace();
		if (ctx.show_set_image_window)          ShowSetImageWindow();
		if (ctx.show_set_image_view_window)     ShowSetImageViewWindow();
		if (ctx.show_image_window)              ShowImageWindow();
		if (ctx.show_image_data_window)			ShowImageDataWindow();
	}
}