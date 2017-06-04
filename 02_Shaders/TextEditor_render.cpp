//Render related functions of TextEditor
#include "TextEditor.h"
#include "imgui\imgui.h"

void GUI::TextEditor::Render()
{
	if(states.window.p_open) SetSize();
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[options.fullscreen ? 4 : 3]);
	if(!ImGui::Begin(winnames.editor, &states.window.p_open, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
	{
		ImGui::End();
		ImGui::PopFont(); return;
	}
	ImGui::PopFont();	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
	if(strlen(text.data()) + 30 > text.size()) text.resize(text.size(), '\0');		//can be optimized
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[options.fullscreen ? 3 : 2]);
	MenuBar();		ImGui::PopFont();
	OpenPopup();	SavePopup();

	const int N = 2;
	const char * tmp_selectables[N] = { "Source Code", "CSG Editor" };
	ImVec2 tab_button_size = ImVec2(ImGui::GetContentRegionAvailWidth() / N - 5, 15);

	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[options.fullscreen ? 3 : 2]);
	for(int i = 0; i < N; ++i)
	{
		if(ImGui::Selectable(tmp_selectables[i], i == states.tab_sub_wind_id, 0, tab_button_size))
			states.tab_sub_wind_id = i;
		if(i != N - 1) ImGui::SameLine();
	}ImGui::PopFont();

	switch(states.tab_sub_wind_id)
	{
	case 0:	ShowTab1();		break;
	case 1:	ShowCSG();		break;
	default:				break;
	}
	ImGui::End();
	ImGui::PopFont();
}

void GUI::TextEditor::ShowTab1()
{
	float h = atanf((float)log.str().length() / 128.f) / 3.1415f*2.f*sizes.win_height*0.4f;
	if(h != 0) h += 75.f;
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[options.fullscreen ? 5 : 4]);
	if(ImGui::Button("BUILD", ImVec2(sizes.win_width*0.5f - 5.f, 30.f)))
		BuildShortcut();
	ImGui::SameLine(0, 10);
	if(ImGui::Button("REBUILD", ImVec2(sizes.win_width*0.5f - 5.f, 30.f)))
		RebuildCompShortcut();
	
	ImGui::PopFont();
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[options.fullscreen ? 2 : 0]);

	static ImVec4 text_bg_color = ImVec4(1, 1, 1, 0.1f);
	ImGui::PushStyleColor(ImGuiCol_FrameBg, text_bg_color);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
	ImGui::InputTextMultiline("##1", text.data(), text.size(), ImVec2(sizes.win_width, sizes.win_height - h - 30), ImGuiInputTextFlags_AllowTabInput);
	text_bg_color = (ImGui::IsItemActive() ? ImVec4(1, 1, 1, 1) : ImVec4(1, 1, 1, 0.1f));
	
	ImGui::PopStyleColor(2);	ImGui::PopFont();
	if(h > 75.1f) LogWindow(h);
}

inline float SmallInputFloat(const char *__restrict name, float &__restrict f)
{
	int length = ( abs(f) > 1 ? (int)log(abs(f)) : 1 ) + (f > 0 ? 5 : 6) + (abs(f) < 1 ? 1 : 0) + (abs(f) < 100 ? 1 : 0);
	ImGui::PushItemWidth(length * 5.2f);
	ImGui::DragFloat(name, &f, 0.1f, 0, 0, (abs(f) < 1 ? "%.2f" : abs(f) < 100 ? "%0.1f" : "%0.f"), 1.01f);
	ImGui::PopItemWidth();
	return (length + strlen(name))*7.0f;
}

void GUI::TextEditor::ShowTab2()
{
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[options.fullscreen ? 5 : 4]);
	if(ImGui::Button("GENERATE", ImVec2(sizes.win_width*0.55f - 5.f, 30.f)))
		GenerateAlgebricSurface();
	ImGui::SameLine(0, 10);
	if(ImGui::Button("BUILD", ImVec2(sizes.win_width*0.45f - 5.f, 30.f)))
		BuildShortcut();

	float width = ImGui::GetContentRegionAvailWidth();
	ImGui::PopFont();

	float line_width = 0;
	for(int i = 0; i < 20; ++i)
	{
		line_width += SmallInputFloat(algebric.labels[i], algebric.a[i]);
		if(line_width + 70 < width)	ImGui::SameLine();
		else line_width = 0;
	}

}

void GUI::TextEditor::LogWindow(float h)
{
	ImGui::Separator();
	std::string str = log.str();
	size_t endln0 = str.find('\n', 0);
	size_t endln1 = str.find('\n', endln0 + 1); //second line end
	std::string line01 = str.substr(0, endln1);

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.4f, 0.4f, 1));
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[options.fullscreen ? 3 : 1]);
	ImGui::TextWrapped(line01.c_str());
	ImGui::PopFont(); ImGui::PopStyleColor();
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[options.fullscreen ? 2 : 0]);
	if(options.log_copyable)
	{
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0.2f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.5f, 0.5f, 0.8f));
		ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0.4f, 0.4f, 0, 0.5f));
		ImGui::InputTextMultiline("##2", const_cast<char*>(str.data()), str.length(), ImVec2(sizes.win_width, h - 100.f), ImGuiInputTextFlags_ReadOnly);
		ImGui::PopStyleColor(3);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0.2f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.5f, 0.5f, 0.7f));
		ImGui::BeginChildFrame(0, ImVec2(sizes.win_width, h - 100));
		ImGui::TextUnformatted(str.c_str());
		ImGui::EndChildFrame();
		ImGui::PopStyleColor(2);
	}
	ImGui::PopFont();
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[options.fullscreen ? 3 : 2]);
	if(ImGui::SmallButton("Copy All"))
		ImGui::SetClipboardText(str.c_str());
	ImGui::SameLine(0, 20);
	if(ImGui::SmallButton("Copy First Error"))
		ImGui::SetClipboardText(line01.c_str() + endln0 + 1);
	ImGui::SameLine(0, 20);
	if(ImGui::SmallButton("Clear"))
		log.str(std::string());	//log.clear() is inherited form ios for clearing error state
	ImGui::PopFont();
}

void GUI::TextEditor::MenuBar()
{
	if(ImGui::BeginMenuBar())
	{
		if(ImGui::BeginMenu("File"))
		{
			if(ImGui::MenuItem("New", "Ctrl+N"))				NewFileShortcut();
			if(ImGui::MenuItem("Reopen", "Ctrl+O"))				Open(filenames.current);
			if(ImGui::MenuItem("Open other", "Ctrl+Shift+O"))	OpenShortcut();
			if(ImGui::BeginMenu("Open Recent"))
			{
				ImGui::MenuItem("Recents", nullptr, nullptr, false);
				for(auto & r : filenames.recents)
					if(ImGui::MenuItem(r.c_str()))
						Open(r.c_str());
				ImGui::Separator();
				ImGui::MenuItem("Examples", nullptr, nullptr, false);
				for(auto & e : filenames.examples)
					if(ImGui::MenuItem(e.c_str()))
						Open(e.c_str());
				ImGui::EndMenu();
			}
			ImGui::Separator();
			if(ImGui::MenuItem("Save", "Ctrl+S"))	SaveShortcut();
			if(ImGui::MenuItem("Save As..", "Ctrl+Shift+S")) SaveAsShortcut();
			if(ImGui::MenuItem("Close"))
				states.window.p_open = false;
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("Options"))
		{
			if(ImGui::MenuItem("Fullscreen", "F11", &options.fullscreen))	Resize(sizes.full_width, sizes.full_height);
			ImGui::MenuItem("Selectable log", nullptr, &options.log_copyable, log.str().length() != 0);
			ImGui::EndMenu();
		}

		if(ImGui::BeginMenu("Build"))
		{
			if(ImGui::MenuItem("Minimal rebuild", "Ctrl+B"))	BuildShortcut();
			if(ImGui::MenuItem("Rebuild computing shader", "Ctrl+R")) RebuildCompShortcut();
			if(ImGui::MenuItem("Rebuild visualizing shader", "Ctrl+Shift+R")) RebuildShowShortcut();
			if(ImGui::BeginMenu("Call Build Function..."))
			{
				if(ImGui::MenuItem("ReloadFragShaderConstCode()"))	ReloadFragShaderConstCode();
				if(ImGui::MenuItem("BuildComputeFragShader()"))	BuildComputeFragShader();
				if(ImGui::MenuItem("BuildComputeVertShader()"))	BuildComputeVertShader();
				if(ImGui::MenuItem("LinkComputeProgram()"))	LinkComputeProgram();
				if(ImGui::MenuItem("BuildShowFragShader()"))	BuildShowFragShader();
				if(ImGui::MenuItem("BuildShowVertShader()"))	BuildShowVertShader();
				if(ImGui::MenuItem("LinkShowProgram()"))	LinkShowProgram();
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void GUI::TextEditor::SetSize()
{
	float w0 = sqrtf((float)sizes.full_width) * 9.5f + 100.f;
	sizes.win_width = (options.fullscreen ? sizes.full_width - w0 - 50: w0);
	sizes.win_height = sizes.full_height - 50;
	ImGui::SetWindowSize(winnames.editor, ImVec2(sizes.win_width + 15, sizes.full_height));
	ImGui::SetWindowPos(winnames.editor, ImVec2(sizes.full_width - sizes.win_width - 15, 0));
}

const ImVec2 popup_button_size = ImVec2(125, 30);
const float cancel_button_offset = 335;

void GUI::TextEditor::OpenPopup()
{
	if(states.window.is_open_openpopup) ImGui::OpenPopup(winnames.open);
	if(ImGui::BeginPopupModal(winnames.open, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::TextUnformatted("Enter filename: ");
		ImGui::SameLine();
		ImGui::InputText("##0", filenames.buffer, 512);
		ImGui::Separator();
		if(ImGui::Button("Open##2", popup_button_size))
		{
			Open(filenames.buffer);
			ImGui::CloseCurrentPopup();
			states.window.is_open_openpopup = false;
		}
		ImGui::SameLine(cancel_button_offset - popup_button_size.x);
		if(ImGui::Button("Cancel##3", popup_button_size))
		{
			ImGui::CloseCurrentPopup();
			states.window.is_open_openpopup = false;
		}
		ImGui::EndPopup();
	}
}

void GUI::TextEditor::SavePopup()
{
	if(states.window.is_open_savepopup) ImGui::OpenPopup(winnames.save);
	if(ImGui::BeginPopupModal(winnames.save, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::TextUnformatted("Enter filename: ");
		ImGui::SameLine();
		ImGui::InputText("##0", filenames.buffer, 512);
		ImGui::Separator();
		if(ImGui::Button("Save##2", popup_button_size))
		{
			Save(filenames.buffer);
			ImGui::CloseCurrentPopup();
			states.window.is_open_savepopup = false;
		}
		ImGui::SameLine(cancel_button_offset - popup_button_size.x);
		if(ImGui::Button("Cancel##3", popup_button_size))
		{
			ImGui::CloseCurrentPopup();
			states.window.is_open_savepopup = false;
		}
		ImGui::EndPopup();
	}
}