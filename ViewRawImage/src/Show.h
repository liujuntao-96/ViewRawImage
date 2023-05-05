#pragma once

#include "imgui/imgui.h"

namespace show {
	void DropFile(std::string path);
	void ShowMainWindow(bool* p_open);
	void SetStyle(ImGuiIO& io);
}