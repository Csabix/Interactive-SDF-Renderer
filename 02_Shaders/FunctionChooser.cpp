#include "FunctionChooser.h"
#include "imgui\imgui.h"

void GUI::FunctionChooser::ConstantFunctionChooser()
{
	ImGui::DragFloat(" = f(x)", &constant_value, 0.01f);
}

void GUI::FunctionChooser::HermiteFunctionChooser()
{
	float width;
	ImGui::Checkbox("Relative derivatives", &relative_derivatives);
	ImGui::Columns(2, "Input");

	width = glm::max(ImGui::GetContentRegionAvailWidth(), 1.f);
	ImGui::TextUnformatted(""); ImGui::SameLine(width / 2 - 30); ImGui::TextUnformatted("First Point");
	ImGui::PushItemWidth(-10);
	ImGui::DragFloat("x##1", &p0.x, 0.1f);
	ImGui::PushItemWidth(-10);
	ImGui::DragFloat("y##1", &p0.y, 0.1f);
	ImGui::PushItemWidth(-40);
	ImGui::DragFloat("dy/dx##1", &d_in.x, 0.01f);

	ImGui::NextColumn();

	width = glm::max(ImGui::GetContentRegionAvailWidth(), 1.f);
	ImGui::TextUnformatted(""); ImGui::SameLine(width / 2 - 30); ImGui::TextUnformatted("Second Point");
	ImGui::PushItemWidth(-10);
	ImGui::DragFloat("x##2", &p1.x, 0.1f);
	ImGui::PushItemWidth(-10);
	ImGui::DragFloat("y##2", &p1.y, 0.1f);
	ImGui::PushItemWidth(-40);
	ImGui::DragFloat("dy/dx##2", &d_in.y, 0.01f);

	ImGui::Columns(1);

	d_out = d_in * (relative_derivatives ? (p1.y - p0.y) / (p1.x - p0.x) : 1.f);
	hermite3_coeffs = FUN::hermite3_coeffs(p0, p1, d_out.x, d_out.y);
}

void GUI::FunctionChooser::ChangeFunction()
{
	if(ImGui::CollapsingHeader(header_name))
	{
		float width0 = glm::max(ImGui::GetContentRegionAvailWidth(), 1.f), width;

		static const int N = 50;	//no runtime reallocation
		static float plot[N + 1];	//only one vector
		float min = p0.y, max = p0.y;
		for(int i = 0; i <= N; ++i)
		{
			float f = function(i / (float)N * (p1.x - p0.x) + p0.x);
			plot[i] = f;
			min = glm::min(f, min);
			max = glm::max(f, max);
		}

		ImGui::PushID(header_name);
		const char * items[] = { "Cubic hermite", "Constant" };

		ImGui::Combo("Function type", &current_funtion_type, items, 2);
		switch(current_funtion_type)
		{
		case 0:	HermiteFunctionChooser();	break;
		case 1:	ConstantFunctionChooser();	break;
		default:break;
		}

		ImGui::Separator();
		ImGui::Columns(2, "Output");

		width = glm::max(ImGui::GetContentRegionAvailWidth(), 1.f);
		ImGui::PlotLines("", plot, N + 1, 0, nullptr, min, max, ImVec2(width, 4 * sqrt(width0)));

		ImGui::NextColumn();
		ImGui::SetColumnOffset(-1, width0 - 175);

		width = glm::max(ImGui::GetContentRegionAvailWidth(), 1.f);


		ImGui::BeginChild(header_name, ImVec2(width, 4 * sqrt(width0)));
		ImGui::Checkbox("Integers only", &integers_only);
		if(!integers_only)
			for(int i = 0; i <= N; i += 4)
				ImGui::Text("x = %.2f, y = %.2f", i / (float)N * (p1.x - p0.x) + p0.x, plot[i]);
		else
			if(p0.x < p1.x)
				for(int x = (int)p0.x; x != (int)p1.x + 1; ++x)
					ImGui::Text("x = %d, y = %.2f", x, function((float)x));
			else
				for(int x = (int)p0.x; x != (int)p1.x - 1; --x)
					ImGui::Text("x = %d, y = %.2f", x, function((float)x));
		ImGui::EndChild();

		ImGui::Columns(1);
		ImGui::PopID();
	}
}

std::array<float, 4> FUN::hermite3_coeffs(glm::vec2 p0, glm::vec2 p1, float d0, float d1)
{
	float a = p0.y;
	float b = d0;

	float dx = p1.x - p0.x;
	float v = (p1.y - p0.y) / dx;

	float c = (v - b) / dx;
	float u = (d1 - v) / dx;
	float d = (u - c) / dx;
	float x[2] = { p0.x, p1.x };

	std::array<float, 4> ret;		//from maple...
	ret[0] = (-d*x[1] + c)*x[0] * x[0] - b*x[0] + a;
	ret[1] = c * x[0] * x[0] + (2 * d*x[1] - 2 * c)*x[0] + b;
	ret[2] = (-2 * x[0] - x[1])*d + c;
	ret[3] = d;
	return ret;
}
