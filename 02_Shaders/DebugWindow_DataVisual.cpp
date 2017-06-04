//Data Visualization related functions of Debug Window
#include "DebugWindow.h"
#include "imgui\imgui.h"
#include "GPUState.h"
#include "Uniforms.h"
#include "FileIO.h"

void ShowImage(const char* name, const Image2D &img, float scale, float m, ImVec4 multipiers = ImVec4(1,1,1,1))
{
	GLuint tex_id = img.get(); GLint tex_w = img.GetW(); GLint tex_h = img.GetH();
	ImGui::Separator();
	ImGui::TextUnformatted(name); ImGui::SameLine(); ImGui::Text("%dx%d", tex_w, tex_h);

	float width = glm::max(ImGui::GetContentRegionAvailWidth(), 1.f);
	float screen_w = width, screen_h = width*tex_h / tex_w;
	ImGui::Image((ImTextureID)tex_id, ImVec2(screen_w, screen_h), ImVec2(0, m), ImVec2(m, 0), multipiers);
	if(ImGui::IsItemHovered())
	{
		ImVec2 tex_screen_pos = ImGui::GetCursorScreenPos();
		ImVec2 mouse_screen_pos = ImGui::GetMousePos();
		ImVec2 uv_mid;
		uv_mid.x = (mouse_screen_pos.x - tex_screen_pos.x) / screen_w;
		uv_mid.y = (tex_screen_pos.y - mouse_screen_pos.y) / screen_h;

		ImVec2 uv0 = ImVec2((uv_mid.x - scale / tex_w)*m, (uv_mid.y + scale / tex_h)*m);
		ImVec2 uv1 = ImVec2((uv_mid.x + scale / tex_w)*m, (uv_mid.y - scale / tex_h)*m);
		ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(1.f, 0.9f, 0.8f, 0.3f));
		ImGui::BeginTooltip();
		ImGui::Image((ImTextureID)tex_id, ImVec2(256, 256), uv0, uv1, multipiers);
		ImGui::EndTooltip();
		ImGui::PopStyleColor();
	}
}

void GUI::DebugWindow::ShowGPUState(int i, const GPUState & gpu, float m)
{
	ImGui::PushID(i);
	float width = ImGui::GetContentRegionAvailWidth();
	ImGui::Checkbox("Orig size", &stateview.b[i]);
	ImGui::PushItemWidth(-275);
	ImGui::SameLine(); ImGui::DragFloat("Magnifier", &stateview.scale[i], 0.1f, 1.f, 256.f);
	ImGui::SameLine(); ImGui::Checkbox("Color", stateview.cnd[i] + 0);
	ImGui::SameLine(); ImGui::Checkbox("Normal", stateview.cnd[i] + 1);
	ImGui::SameLine(); ImGui::Checkbox("Depth", stateview.cnd[i] + 2);

	if(stateview.b[i]) m = 1.0;
	if(stateview.cnd[i][0])	ShowImage("Color image", gpu.linear_2, stateview.scale[i], m,  ImVec4(1,1,1, 10));
	if(stateview.cnd[i][1])	ShowImage("Normal image", gpu.linear_1, stateview.scale[i], m, ImVec4(1,1,1, 10));
	if(stateview.cnd[i][2])	ShowImage("Depth image", gpu.nearest_0, stateview.scale[i], m, ImVec4(0.02, 2, 0.02, 10));
	ImGui::PopID();
}

void GUI::DebugWindow::ColorMappingSettings(int i)
{
	ImGui::PushID(2 + i);
	ImGui::SameLine(); ImGui::RadioButton("Depth", &view.samplers[i], 0);
	ImGui::SameLine(); ImGui::RadioButton("Normal", &view.samplers[i], 1);
	ImGui::SameLine(); ImGui::RadioButton("Color", &view.samplers[i], 2);
	ImGui::PushItemWidth(-80);
	ImGui::Combo("Color mode", &view.detail.color_mode[i], "Identity\0""3 Channels\0Single Channel\0Advanced\0");
	switch(view.detail.color_mode[i])
	{
	case 0: view.color_multipiers[i] = glm::mat4(1); view.color_offsets[i] = glm::vec4(0);	break;
	case 1: view.color_multipiers[i] = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0);
		view.color_offsets[i] = glm::vec4(0, 0, 0, 1);										break;
	case 2:
	{
		ImGui::TextUnformatted("Choose a ch");
		ImGui::SameLine(); ImGui::RadioButton("R", &view.detail.channel_id[i], 0);
		ImGui::SameLine(); ImGui::RadioButton("G", &view.detail.channel_id[i], 1);
		ImGui::SameLine(); ImGui::RadioButton("B", &view.detail.channel_id[i], 2);
		ImGui::SameLine(); ImGui::RadioButton("A", &view.detail.channel_id[i], 3);

		float width = ImGui::GetContentRegionAvailWidth() / 2.5f - 10.f;
		ImGui::PushItemWidth(width);
		ImGui::DragFloat("Mult", &view.detail.ch_multipiers[i], 0.005f);
		ImGui::PushItemWidth(width);
		ImGui::SameLine();
		ImGui::DragFloat("Offset", &view.detail.ch_offsets[i], 0.005f);

		view.color_multipiers[i] = glm::mat4(0);
		view.color_multipiers[i][view.detail.channel_id[i]] = glm::vec4(view.detail.ch_multipiers[i]);
		view.color_offsets[i] = glm::vec4(view.detail.ch_offsets[i]);
	}
	break;
	case 3:
	{
		glm::mat4 &M = view.detail.channel_map[i];
		for(int r = 0; r < 4; ++r)
		{
			ImGui::PushID(r); ImGui::Text("Channel %d", r);
			ImGui::SameLine();	M[r][0] = (float)(M[r][0] != (float)ImGui::RadioButton("##1", M[r][0] == 1.f));
			ImGui::SameLine();	M[r][1] = (float)(M[r][1] != (float)ImGui::RadioButton("##2", M[r][1] == 1.f));
			ImGui::SameLine();	M[r][2] = (float)(M[r][2] != (float)ImGui::RadioButton("##3", M[r][2] == 1.f));
			ImGui::SameLine();	M[r][3] = (float)(M[r][3] != (float)ImGui::RadioButton("##4", M[r][3] == 1.f));
			ImGui::PopID();
		}
		//glm::vec4 diag = view.detail.multipiers[i].
		glm::vec4 &d = view.detail.multipiers_diag[i];
		ImGui::DragFloat4("Mults", &d.x, 0.005f);
		ImGui::DragFloat4("Offsets", &view.detail.offsets[i].x, 0.005f);

		view.color_offsets[i] = M * view.detail.offsets[i];
		glm::mat4 temp = glm::mat4(d.x, 0, 0, 0, 0, d.y, 0, 0, 0, 0, d.z, 0, 0, 0, 0, d.w);
		view.color_multipiers[i] = M * temp;
	}
	break;
	default:break;
	}
	ImGui::PopID();
}

void GUI::DebugWindow::UpdateHistogramCH(int ch)
{	//Create histogram and statistical data for channel ch
	stats.histograms[ch].assign(stats.hist_Ns[ch], 0.f);
	float multipier = (float) stats.hist_Ns[ch] / (stats.max[ch] - stats.min[ch]);
	int maxind = (int) stats.hist_Ns[ch] - 1;
	for(const auto& pix : stats.pixeldata)
		++stats.histograms[ch][std::min<int>((pix[ch] - stats.min[ch]) * multipier, maxind)];
	float normalizer = 1.f / (float)stats.pixeldata.size();
	//normalize histogram and generate latex from histograms
	auto &hs = stats.histograms[ch];		auto &lh = stats.latex_hist[ch];
	lh = "\\begin{tikzpicture}\n\\begin{axis}[ybar interval, ymax = 1, ymin = 0]\n\\addplot coordinates { ";
	for(int j = 0; j < hs.size(); ++j)
	{
		hs[j] *= normalizer;	//normalizing
		lh += "(" + std::to_string(j / multipier + stats.min[ch]) + ", " + std::to_string(hs[j]) + ") ";
	}
	lh += "}; \n\\end{axis}\n\\end{tikzpicture}\n%" + std::to_string(stats.min[ch]) + " && "
		   + std::to_string(stats.max[ch]) + " && " + std::to_string(stats.avg[ch]) + " && " + std::to_string(stats.sd[ch]);
}

void GUI::DebugWindow::DispHistDataCH(int channel)
{
	ImGui::PushID(channel);
	float width = ImGui::GetContentRegionAvailWidth();
	ImGui::TextDisabled("Channel %d", channel); ImGui::SameLine();
	ImGui::PushItemWidth(60);
	if(ImGui::InputInt("##i", &stats.hist_Ns[channel]))
		UpdateHistogramCH(channel);
	if(stats.histograms[channel].size() > 0)
	{
		ImGui::SameLine(0, 15);
		if(ImGui::SmallButton("Copy LaTeX"))
			ImGui::SetClipboardText(stats.latex_hist[channel].data());
		ImGui::PlotHistogram("##h", stats.histograms[channel].data(), stats.histograms[channel].size(),
							 0, nullptr, FLT_MAX, FLT_MAX, ImVec2(width, sizes.win_width*0.33));
		ImGui::Text("range = %0.2f - %0.2f, avg = %0.2f, sd = %0.2f", stats.min[channel], stats.max[channel], stats.avg[channel], stats.sd[channel]);
		if(ImGui::BeginChild(channel, ImVec2(width, 92), true, ImGuiWindowFlags_HorizontalScrollbar))
		{
			ImGui::TextUnformatted(stats.latex_hist[channel].data(), &stats.latex_hist[channel].back());
		}
		ImGui::EndChild();
	}
	ImGui::PopID();
}

void GUI::DebugWindow::ShowVisualisationOptions()
{
	if(ImGui::CollapsingHeader("GPU States", nullptr, true, true))
	{
		ImGui::TextUnformatted("Current GPU state");
		ShowGPUState(0, *gpu_states.source_state, uniforms.resolution_multipiers.camera.x);
		ImGui::Separator();
		ImGui::TextUnformatted("Previous GPU state");
		ShowGPUState(1, *gpu_states.target_state, uniforms.resolution_multipiers.texture.x);
	}
	if(ImGui::CollapsingHeader("Fullscreen Visualization", nullptr, true, true))
	{
		int tmp = view.type;
		ImGui::PushItemWidth(-80);
		ImGui::Combo("View type", &tmp, "Standard\0Adjustable\0Quadrants\0");
		view.type = tmp;
		if(view.type != 0)
		{
			if(view.type == 2) { ImGui::Separator(); ImGui::Columns(2, "First row", true); }
			ImGui::TextUnformatted((view.type == 1 ? "Channel" : "1st"));
			ColorMappingSettings(0);
			if(view.type == 2)
			{
				ImGui::NextColumn();
				ImGui::TextUnformatted("2nd");	ColorMappingSettings(1);
				ImGui::Columns(1, "Separator only", false);
				ImGui::Separator();
				ImGui::Columns(2, "Second row", true);
				ImGui::TextUnformatted("3rd");	ColorMappingSettings(2);
				ImGui::NextColumn();
				ImGui::TextUnformatted("4th");	ColorMappingSettings(3);
				ImGui::Columns(1, nullptr, false);	ImGui::Separator();
			}
			glUniform1iv(0, 4, view.samplers);
			glUniformMatrix4fv(9, 4, false, &view.color_multipiers[0][0][0]);
			glUniform4fv(5, 4, &view.color_offsets[0][0]);
		}
	}
	if(ImGui::CollapsingHeader("Histograms", nullptr, true, false))
	{
		static int texind = 0;
		float width = ImGui::GetContentRegionAvailWidth();
		ImGui::PushItemWidth(width*0.55 + 50);
		ImGui::Combo("Texture", &texind, "nearest 0\0linear 1\0linear 2\0linear 3\0");
		ImGui::SameLine(); const int H_N = 30;
		if(ImGui::Button("Load"))
		{
			GLuint texid; GLsizei w, h;
			const GPUState *gpus = gpu_states.source_state;
			switch(texind)
			{
			case 0: texid = gpu_states.source_state->nearest_0.get(); w = gpus->nearest_0.GetW(); h = gpus->nearest_0.GetH();  break;
			case 1: texid = gpu_states.source_state->linear_1.get(); w = gpus->linear_1.GetW(); h = gpus->linear_1.GetH();  break;
			case 2: texid = gpu_states.source_state->linear_2.get(); w = gpus->linear_2.GetW(); h = gpus->linear_2.GetH();  break;
			case 3: texid = gpu_states.source_state->linear_3.get(); w = gpus->linear_3.GetW(); h = gpus->linear_3.GetH();  break;
			}
			stats.pixeldata.resize(w*h);
			glBindTexture(GL_TEXTURE_2D, texid);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, stats.pixeldata.data());

			stats.min = stats.pixeldata[0]; stats.max = stats.pixeldata[0];
			for(const auto& pix : stats.pixeldata)
			{
				stats.min = glm::min(stats.min, pix);	stats.max = glm::max(stats.max, pix);
				stats.avg += pix;						stats.sd += pix * pix;
			}
			float normalizer = 1.f / (float)stats.pixeldata.size();
			stats.avg *= normalizer;		stats.sd= glm::sqrt(stats.sd * normalizer - stats.avg * stats.avg);
			for(int i = 0; i < 4; ++i) UpdateHistogramCH(i);
		}
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
		ImGui::Columns(2, "1", true);		DispHistDataCH(0);
		ImGui::NextColumn();			DispHistDataCH(1);
		ImGui::Columns(1, 0, false);	ImGui::Separator();
		ImGui::Columns(2, "2", true);	DispHistDataCH(2);
		ImGui::NextColumn();			DispHistDataCH(3);
		ImGui::Columns(1, 0, false);	ImGui::PopFont();

		static char filename[256] = "R:/stats.txt";
		ImGui::PushItemWidth(width*0.55 + 50);
		ImGui::InputText("Filename", filename, 256, ImGuiInputTextFlags_AutoSelectAll);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		if(ImGui::Button("Save"))
			FileIO::SaveUnformattedData(filename, (char*)&stats.pixeldata[0], stats.pixeldata.size()*sizeof(glm::vec4));
	}
	if(ImGui::CollapsingHeader("Iteration times", nullptr, true, true))
	{
		ImGui::TextUnformatted("Optimize Rendertimes:"); ImGui::SameLine();
		if(ImGui::RadioButton("On", times.optimize)) times.optimize = true;
		ImGui::SameLine();
		if(ImGui::RadioButton("Off", !times.optimize)) times.optimize = false;

		ImGui::TextUnformatted("Target framerate:"); ImGui::SameLine();
		if(ImGui::RadioButton("60fps", 1 * times.consts.fps60ms == times.consts.target)) times.consts.target = 1 * times.consts.fps60ms;
		ImGui::SameLine();
		if(ImGui::RadioButton("30fps", 2 * times.consts.fps60ms == times.consts.target)) times.consts.target = 2 * times.consts.fps60ms;
		ImGui::SameLine();
		if(ImGui::RadioButton("20fps", 3 * times.consts.fps60ms == times.consts.target)) times.consts.target = 3 * times.consts.fps60ms;
		ImGui::SameLine();
		if(ImGui::RadioButton("15fps", 4 * times.consts.fps60ms == times.consts.target)) times.consts.target = 4 * times.consts.fps60ms;
		ImGui::SameLine();
		if(ImGui::RadioButton("12fps", 5 * times.consts.fps60ms == times.consts.target)) times.consts.target = 5 * times.consts.fps60ms;
		ImGui::SameLine();
		if(ImGui::RadioButton("10fps", 6 * times.consts.fps60ms == times.consts.target)) times.consts.target = 6 * times.consts.fps60ms;

		ImGui::TextDisabled("Computation time: %0.1f ms", times.sum_of_updates);
		ImGui::TextDisabled("Show & UI time: %0.1f ms", times.render);
		ImGui::TextDisabled("Total GPU time: %0.1f ms", times.total);
		ImGui::TextDisabled("Total CPU time: %0.1f ms", times.cpu_measured_time);
		ImGui::TextDisabled("Average FPS: %0.1f ms", 1000.0 / times.cpu_measured_time);
		ImGui::TextDisabled("Number of  updates: %d", times.num_of_updates);
		float width = ImGui::GetContentRegionAvailWidth();
		
		ImGui::PlotHistogram("##UPDATES",times.update.data(),times.update.size()/8, 0,nullptr,0,times.consts.target,ImVec2(width,10*sqrtf(width)));
		
		if(ImGui::BeginChild("Time prediction vector", ImVec2(0, 260)))
		{
			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
			ImGui::Separator();
			for(int i = 0; i < times.consts.N - 1; ++i)
				ImGui::TextDisabled("%d itaration time : %0.1f ms", i + 1, times.update[i]);
			ImGui::TextDisabled("Above %d iteration: %0.1f ms", times.consts.N, times.update[times.consts.N - 1]);
			ImGui::Separator();
			ImGui::PopFont();
		}
		ImGui::EndChild();
	}
}