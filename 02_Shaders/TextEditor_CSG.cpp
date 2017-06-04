#include "TextEditor.h"
#include "imgui\imgui.h"

void GUI::TextEditor::DispCSG_Primitive(GUI::TextEditor::CSG_Primitive & prim)
{
	ImGui::Indent();
	ImGui::Combo("Primitive type", (int*)&prim.type, "Plane\0Plane XY\0Plane XZ\0PlaneYZ\0Sphere\0Cylinder\0Torus\0Box\0");
	switch(prim.type)
	{
	case PLANE:	if(ImGui::DragFloat3("Normal", &prim.normal.x))
						prim.normal = glm::normalize(prim.normal);
	case PLANE_XY: case PLANE_YZ: case PLANE_XZ : break;
	case TORUS:
		ImGui::DragFloat("r", &prim.r);
	case SPHERE: case CYLINDER:
		ImGui::DragFloat("R", &prim.R);
		break;
	case BOX:	ImGui::TextDisabled("Not implemented jet"); break;
	default: ImGui::TextDisabled("Unknown primitive"); break;
	}
	ImGui::Unindent();
}

void GUI::TextEditor::DispCSG_Edge(GUI::TextEditor::CSG_Edge & edge)
{
	ImGui::Combo("Transformation", (int*)&edge.type, "Identity\0Transpose\0Rotate\0Affin\0Projective\0");
	switch(edge.type)
	{
	case IDENTITY: break; case TRANSPOSE: ImGui::DragFloat3("Delta", &edge.transpose.x); break;
	case ROTATE: case AFFIN: case PROJECTIVE: ImGui::TextDisabled("Not implemented jet"); break;
	default: ImGui::TextDisabled("Unknown transformation type"); break;
	}
	ImGui::Indent();
	DispCSG_Node(*edge.node);
	ImGui::Unindent();
}


void GUI::TextEditor::DispCSG_Node(GUI::TextEditor::CSG_Node & node)
{
	float width = ImGui::GetContentRegionAvailWidth();
	ImGui::Combo("Node type", (int*)&node.type, "Primitive\0Union\0Intersection\0Differance\0Soft Union\0Soft Intersection\0Soft Differance\0");
	switch(node.type)
	{
	case PRIMITIVE:	DispCSG_Primitive(node.primitive); node.edges.clear(); break;
	case SOFT_UNION: case SOFT_INTERSECTION: case SOFT_DIFFERANCE:
		ImGui::DragFloat("Radius", &node.soft_ness);
	case UNION: case INTERSECTION: case DIFFERANCE:
		if(node.edges.size() < 2) node.edges.resize(2);
		for(int i = 0; i < node.edges.size(); ++i)
		{
			std::string label = "Operand " + std::to_string(i);
			if(ImGui::TreeNode(label.data()))
			{
				label = "Delete node ##" + std::to_string(i); ImGui::SameLine(width-80);
				if(ImGui::SmallButton(label.data())) node.edges.erase(node.edges.begin() + i);
				else DispCSG_Edge(node.edges[i]);
				ImGui::TreePop();
			}
			else
			{
				label = "Delete node ##" + std::to_string(i); ImGui::SameLine(width - 80);
				if(ImGui::SmallButton(label.data())) node.edges.erase(node.edges.begin() + i);
			}
		}
		if(ImGui::Button("Add another")) { node.edges.push_back(CSG_Edge()); }
		break;
	default: ImGui::TextDisabled("Unknown node type"); break;
	}
}

void GUI::TextEditor::ShowCSG()
{
	//ImGui::Columns(2);
	//ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth()*0.6);
	DispCSG_Node(csg.root);
	//ImGui::Columns(2);
}

GUI::TextEditor::CSG_Edge::CSG_Edge() : node(new CSG_Node()){}

GUI::TextEditor::CSG_Edge::~CSG_Edge(){	delete node;}

GUI::TextEditor::CSG_Edge::CSG_Edge(CSG_Edge & edge)
{
	node = new CSG_Node();
	std::swap(edge.node, node);
	transpose = edge.transpose;
	rotate = edge.rotate;
	projective = edge.projective;
}

GUI::TextEditor::CSG_Edge& GUI::TextEditor::CSG_Edge::operator = (CSG_Edge & edge)
{
	std::swap(edge.node, node);
	transpose = edge.transpose;
	rotate = edge.rotate;
	projective = edge.projective;
	return *this;
}