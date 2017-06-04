#include "DebugWindow.h"
#include "imgui\imgui.h"
#include "Uniforms.h"


void GUI::DebugWindow::ShowIterationOptions()
{
	if(turn_pause_to_false) pause = false;
	if(turn_pause_to_true) pause = true;
	turn_pause_to_false = turn_pause_to_true = false;

	ImVec2 button_size = ImVec2(ImGui::GetContentRegionAvailWidth() / 2 - 5, 30);

	ImGui::PushStyleColor(ImGuiCol_Button, (pause ? ImVec4(0, 1, 0, 0.5f) : ImVec4(1, 0, 0, 0.5f)));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (pause ? ImVec4(0, 1, 0, 0.75f) : ImVec4(1, 0, 0, 0.75f)));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (pause ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1)));
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[4]);
	if(ImGui::Button((pause ? "Resume" : "Pause"), button_size)) pause = !pause;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);
	ImGui::SameLine();
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[4]);
	if(ImGui::Button("Restart", button_size))	iternum = 0;
	ImGui::PopFont();
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[3]);
	ImGui::TextUnformatted("Iteration = "); ImGui::SameLine(); ImGui::TextColored(ImVec4(1, 0.5, 0.5, 1), "%d", iternum);
	ImGui::PopFont();
	if(pause)
	{
		button_size = ImVec2(ImGui::GetContentRegionAvailWidth() / 3 - 5, 19);
		if(ImGui::Button("Next Frame", button_size))
		{
			turn_pause_to_true = true;
			pause = false;
		}
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, (show_currentstate ? ImVec4(1, 1, 0, 0.5) : ImVec4(0, 0, 1, 0.5)));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (show_currentstate ? ImVec4(1, 1, 0, 0.75f) : ImVec4(0, 0, 1, 0.75f)));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (show_currentstate ? ImVec4(1, 1, 0, 1) : ImVec4(0, 0, 1, 1)));
		if(ImGui::Button((show_currentstate ? "Show Previous State" : "Show Current State"), button_size))
			show_currentstate = !show_currentstate;
		ImGui::PopStyleColor(3);
	}
	ImGui::PushStyleColor(ImGuiCol_Button, (original_size ? ImVec4(0, 1, 1, 0.5) : ImVec4(1, 0, 1, 0.5)));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (original_size ? ImVec4(0, 1, 1, 0.75f) : ImVec4(1, 0, 1, 0.75f)));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (original_size ? ImVec4(0, 1, 1, 1) : ImVec4(1, 0, 1, 1)));
	if(pause)
	{
		ImGui::SameLine();
		if(ImGui::Button((original_size ? "Show Original Size" : "Show Normal Size"), button_size))
			original_size = !original_size;
	}
	else
	{
		ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - (original_size ? 127 : 113));
		if(ImGui::SmallButton((original_size ? "Show Original Size" : "Show Normal Size")))
			original_size = !original_size;
	}
	ImGui::PopStyleColor(3);
}

void GUI::DebugWindow::ShowSwichOptions()
{
	float width = ImGui::GetContentRegionAvailWidth();
	int tmpi; float tmpf;
	ImGui::PushItemWidth(-25.f - width*0.2f);

	if(ImGui::CollapsingHeader("Tracing Options", nullptr, false, false))
	{
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
		ImGui::TextDisabled("Sphere-trace");
		ImGui::PopFont();

		tmpi = uniforms.algorithms.spheretrace - Uniforms::ST_1;
		ImGui::Combo("Algorithm", &tmpi, "Basic Algorithm\0Relaxed Sphere-tracing Algorithm\0Tangent Spheres Algorithm\0");
		uniforms.algorithms.spheretrace = tmpi + Uniforms::ST_1;

		ImGui::Separator();
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
		ImGui::TextDisabled("Surface normals");
		ImGui::PopFont();

		tmpi = uniforms.algorithms.norm - Uniforms::NORM_0;
		ImGui::Combo("Aproximation method", &tmpi, "Use previous (0 evaluations)\0Tetrahedron (4 evaluations)\0Simmetric difference (6 evaluations)\0");
		uniforms.algorithms.norm = tmpi + Uniforms::NORM_0;

		ImGui::DragFloat("Normal epszilon multipier", &uniforms.normal.epsz_mult, 0.01f, 0.f, 1000);
	}
	if(ImGui::CollapsingHeader("Camera & Lighting", nullptr, false, true))
	{
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
		ImGui::TextDisabled("Camera");
		ImGui::PopFont();
		bool l = false;
		glm::vec3 eye = camera.GetEye();
		l = l || ImGui::DragFloat3("Eye", &eye.x, 0.1f, 0, 0, "%.4f");
		glm::vec3 at = camera.GetAt();
		l = l || ImGui::DragFloat3("At", &at.x, 0.1f, 0, 0, "%.4f");
		glm::vec3 up = camera.GetUp();
		l = l || ImGui::DragFloat3("Up", &up.x, 0.1f, 0, 0, "%.4f");
		if(l) { camera.SetView(eye, at, up); iternum = 0; }

		float speed = camera.GetSpeed();
		ImGui::PushItemWidth(width*0.4f);
		ImGui::DragFloat("Speed", &speed, 0.1f, 0, 1000, "%.1f");
		camera.SetSpeed(speed);
		ImGui::PopItemWidth();		ImGui::SameLine();
		ImGui::DragFloat("Fow multipier", &view.fow_mult, 0.01f, 0.02f, 100, "%.2f");

		ImGui::Separator();

		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
		ImGui::TextDisabled("Light");
		ImGui::PopFont();
		static bool relative_to_camera = false;
		static glm::vec3 light_pos = glm::vec3(2);

		ImGui::DragFloat3("Position##Light", (relative_to_camera ? &light_pos.x : &uniforms.shadows.light_pos.x), 0.1f);
		if(relative_to_camera) uniforms.shadows.light_pos = eye + light_pos;

		ImGui::Checkbox("Relative to camera", &relative_to_camera);
		ImGui::SameLine();
		ImGui::DragFloat("Radius##Light", &uniforms.shadows.light_radius, 0.01f, 0.f, 10000, "%.2f");
		ImGui::PushItemWidth(width*0.4f);
		ImGui::DragFloat("Decay##Light", &uniforms.material.light_decay, 0.001f, 0, uniforms.material.light_strength, "%.3f");
		ImGui::PopItemWidth();		ImGui::SameLine();
		ImGui::DragFloat("Strength##Light", &uniforms.material.light_strength, 0.1f, 0, 10000, "%.2f");
	}
	if(ImGui::CollapsingHeader("Occulution Options", nullptr, false, true))
	{
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
		ImGui::TextDisabled("Ambient Occulution");
		ImGui::PopFont();	ImGui::SameLine(width*0.4f);
		ImGui::InputInt("Ambient##AO", &ambient.start_iternum);
		ImGui::Combo("Algorithm##AO", &ambient.algorithm, "No Ambient Occulution\0Progressive AO\0");
		ImGui::DragFloat("Radius##AO", &uniforms.ambient_occolution.radius, 0.01f, 0, 10000, "%.2f");
		ImGui::DragFloat("Strength##AO", &uniforms.ambient_occolution.strength, 0.01f, 0, 10000, "%.2f");
		ImGui::Separator();

		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
		ImGui::TextDisabled("Shadow");
		ImGui::PopFont();	ImGui::SameLine(width*0.4f);
		tmpf = (1 - uniforms.shadows.step_multipier) * 100;
		ImGui::DragFloat("Quality##Shadow", &tmpf, 0.1f, 0, 90, "%.1f%%");
		uniforms.shadows.step_multipier = 1 - tmpf*0.01f;

		tmpi = uniforms.algorithms.shadow - Uniforms::SHADOW_0;
		ImGui::Combo("Shadow Type", &tmpi,
					 "No shadow\0Hard Shadow to light\0Soft Shadow to light\0Hard Shadow from light\0Soft Shadow from light\0");
		uniforms.algorithms.shadow = tmpi + Uniforms::SHADOW_0;

	}
	if(ImGui::CollapsingHeader("Material Options", nullptr, false, false))
	{
		ImGui::DragFloat("Ambient strength", &uniforms.material.ambient, 0.01f, 0, 1, "%.2f");
		ImGui::DragFloat("Diffuse strength", &uniforms.material.diffuse, 0.01f, 0, 1, "%.2f");
		ImGui::DragFloat("Specular strength", &uniforms.material.specular, 0.01f, 0, 1, "%.2f");
		ImGui::DragFloat("Specular power", &uniforms.material.power, 1, 0, 10000, "%.1f", 1.5);
		ImGui::DragFloat("Light decay", &uniforms.material.light_decay, 0.001f, 0, uniforms.material.light_strength, "%.3f");
		ImGui::DragFloat("Light Strength", &uniforms.material.light_strength, 0.1f, 0, 10000, "%.2f");
	}
	ImGui::PopItemWidth();
}