#include "TextEditor.h"
#include <map>
void GUI::TextEditor::Resize(float w, float h)
{
	sizes.full_width = w; sizes.full_height = h;
	SetSize();
}

GUI::TextEditor::TextEditor() : show(log), compute(log), text(1,' ')
{
	for(int i = 0; i < 20; ++i)		//init map
		algebric.detail.names_to_i[algebric.names[i]] = i;
}

std::string GUI::TextEditor::GenerateExpr(const std::string &expr)
{
	std::stringstream ret;
	auto &A = algebric;
	for(std::string::const_iterator l = expr.begin(); l != expr.end(); ++l)
	{
		switch(*l)
		{
		case 'a':	//TODO: delete zeros altogether
		{
			std::string aijk(l, l + 4);
			float val = A.a[A.detail.names_to_i[aijk]];
			ret << abs(val);	//abszolút érték kell!
			l += 3;	//++l => l+=4 ;)
		} break;
		case 'p': case 'v': ret << *l << '.'; break; //add dots
		case '+': ret << " + "; break;	//readabality
		default: ret << *l;	break;
		}
	}
	return ret.str();
}

void GUI::TextEditor::GenerateAlgebricSurface()
{
	std::string str = "";
	str += "vec3 color(in vec3 p, in vec3 v)\n";
	str += "{\n";
	{
		str += "\start_time return vec3(1,1,1);\n";
	}
	str += "}\n\n";
	str += "float sdf(in vec3 p, in vec3 v)\n";
	str += "{\n";
	{
		str += "\start_time float f = " + GenerateExpr(algebric.formulas_3.fp) + ";\n";
		str += "\start_time vec3 df;\n";
		str += "\start_time df.x = " + GenerateExpr(algebric.formulas_3.dxfp) + ";\n";
		str += "\start_time df.y = " + GenerateExpr(algebric.formulas_3.dyfp) + ";\n";
		str += "\start_time df.z = " + GenerateExpr(algebric.formulas_3.dzfp) + ";\n";
		//str += "\start_time float ldf = length(df);\n";
		//str += "\start_time float h = f / ldf;\n";
		//str += "\start_time float lip = " + GenerateExpr(algebric.formulas_3.lip) + ";\n";
		str += "\start_time return f / length(df);\n";
	}
	str += "}\n";

	text.assign(str.begin(), str.end());
	text.resize(text.size() + 10);
	strcpy_s(filenames.current, "Shaders/SDF/generated.frag");
}