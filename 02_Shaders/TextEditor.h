#pragma once
#include "FileIO.h"
#include "ShaderProg.h"
#include <SDL_keycode.h>
#include <sstream>
#include <set>

namespace GUI
{
	class TextEditor
	{
	public:
		TextEditor();
		void Open(const char * file);
		void Save(const char * file);
		void Render();
		void Resize(float w, float h);

		ShaderProg compute;
		ShaderProg show;
		
		void KeyboardDown(SDL_Keycode key);
		void KeyboardUp(SDL_Keycode key);

		inline bool isProgramValid() const { return states.is_valid_program; }
		inline void resetProrgamValidity() { states.is_valid_program = true; }
		inline bool isProgramValid_AND_Reset(){	bool ret = states.is_valid_program;	states.is_valid_program = true;	return ret;	}
		inline std::ostream& Log() { return log; }

		void BuildShortcut();
		void RebuildCompShortcut();
		void RebuildShowShortcut();

	public: // private
		std::vector<char> text;
		std::stringstream log;
		struct
		{
			std::vector<std::string> examples = { "Shaders/SDF/gombok.frag","Shaders/SDF/aprajafalva.frag" ,
				"Shaders/SDF/hoember.frag", "Shaders/SDF/metasdf.frag", "Shaders/SDF/mandelbulb.frag", "Shaders/SDF/csg.frag" };
			std::set<std::string> recents;
			char current[512];
			char buffer[512];
		} filenames;
		struct
		{
			float full_width = 350;	float full_height = 450;
			float win_width = 350;	float win_height = 450;
		} sizes;
		struct
		{
			bool fullscreen = false;
			bool log_copyable = false;
		} options;
		struct
		{
			char editor[256];
			const char* open = "Open File";
			const char* save = "Save File";
		} winnames;
		struct
		{
			struct
			{
				bool p_open = true;
				bool is_open_openpopup = false;
				bool is_open_savepopup = false;
			} window;
			struct
			{
				bool ctrl = false;
				bool shift = false;
				bool alt = false;
			} key;
			bool is_valid_program = true;
			int tab_sub_wind_id = 0;
		} states;

		struct
		{
			std::string begin;
			std::string end;
		} source_codes;

		struct
		{
			//order is defined below
			float a[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
			const char * labels[20] = { " + ", "*x + ", "*y + " , "*z + ",
				"*x^2 + ", "*y^2 + " , "*z^2 + ", "*x*y + ", "x*z + ", "y*z + ",
				"*x^3 + ", "*y^3 + ", "*z^3 + ", "*x*y*z + ",
				"*x^2*y + ", "*x^2*z + " , "*x*y^2 + " , "*x*z^2 + " , "*y^2*z + " , "*y*z^2" };
			const char *names[20] = { "a000", "a100", "a010", "a001",
				"a200", "a020", "a002", "a110", "a101", "a011",
				"a300", "a030", "a003", "a111",
				"a210", "a201", "a120", "a102", "a021", "a012" };
			struct
			{
				const std::string lip = "(a001+(3*a003*pz+2*a002)*pz+(2*a012*pz+a021*py+a011)*py+(2*a102*pz+a111*py+a201*px+a101)*px)*vz+(a010+(a012*pz+a011)*pz+(2*a021*pz+3*a030*py+2*a020)*py+(a111*pz+2*a120*py+a210*px+a110)*px)*vy+(a100+(a102*pz+a101)*pz+(a111*pz+a120*py+a110)*py+(2*a201*pz+2*a210*py+3*a300*px+2*a200)*px)*vx+((3*a003*pz+a012*py+a102*px+a002)*vz*vz+((2*a012*pz+2*a021*py+a111*px+a011)*vz+(a021*pz+3*a030*py+a120*px+a020)*vy)*vy+((2*a102*pz+a111*py+2*a201*px+a101)*vz+(a111*pz+2*a120*py+2*a210*px+a110)*vy+(a201*pz+a210*py+3*a300*px+a200)*vx)*vx+(a003*vz*vz*vz+(a012*vz*vz+(a021*vz+a030*vy)*vy)*vy+(a102*vz*vz+(a111*vz+a120*vy)*vy+(a201*vz+a210*vy+a300*vx)*vx)*vx)*h)*h";
				const std::string fp = "a000+(a001+(a003*pz+a002)*pz)*pz+(a010+(a012*pz+a011)*pz+(a021*pz+a030*py+a020)*py)*py+(a100+(a102*pz+a101)*pz+(a111*pz+a120*py+a110)*py+(a201*pz+a210*py+a300*px+a200)*px)*px";
				const std::string dxfp = "a100+(a102*pz+a101)*pz+(a111*pz+a120*py+a110)*py+(2*a201*pz+2*a210*py+3*a300*px+2*a200)*px";
				const std::string dyfp = "a010+(a012*pz+a011)*pz+(2*a021*pz+3*a030*py+2*a020)*py+(a111*pz+2*a120*py+a210*px+a110)*px";
				const std::string dzfp = "a001+(3*a003*pz+2*a002)*pz+(2*a012*pz+a021*py+a011)*py+(2*a102*pz+a111*py+a201*px+a101)*px";
			} formulas_3;
			struct
			{
				std::map<std::string, int> names_to_i;
			} detail;
		} algebric;

		struct CSG_Node;
		enum CSG_EDGE_T
		{
			ID = 0,				IDENTITY = 0,
			TRANSPOSE = 1,		ROTATE = 2,
			AFFIN = 3,			PROJECTIVE = 4,
		};
		struct CSG_Edge
		{
			CSG_Node *node;	CSG_EDGE_T type = ID;
			glm::vec3 transpose;
			glm::mat3 rotate;
			glm::mat4 projective;
			CSG_Edge();			~CSG_Edge();
			CSG_Edge(CSG_Edge & edge);
			GUI::TextEditor::CSG_Edge & operator=(CSG_Edge & edge);
		};

		enum CSG_PRIMITIVE_T
		{
			PLANE = 0,		PLANE_XY = 1,	PLANE_XZ = 2,	PLANE_YZ = 3,
			SPHERE = 4,		CYLINDER = 5,	TORUS = 6,		BOX = 7,
		};
		struct CSG_Primitive
		{
			CSG_PRIMITIVE_T type = SPHERE;
			float R, r;
			float a, b, c;
			glm::vec3 normal;
		};

		enum CSG_NODE_T
		{
			PRIMITIVE = 0,
			UNION = 1, INTERSECTION = 2, DIFFERANCE = 3,
			SOFT_UNION = 4, SOFT_INTERSECTION = 5, SOFT_DIFFERANCE = 6,
		};
		struct CSG_Node
		{
			CSG_NODE_T type = PRIMITIVE;
			std::vector<CSG_Edge> edges;
			struct{	float soft_ness; };
			CSG_Primitive primitive;		//the leaf data
		};

		struct
		{
			CSG_Node root;
		} csg;
	
		bool BuildComputeFragShader();
		bool ReloadFragShaderConstCode();
		bool BuildComputeVertShader();
		bool LinkComputeProgram();

		bool BuildShowFragShader();
		bool BuildShowVertShader();
		bool LinkShowProgram();

		void OpenPopup();
		void SavePopup();
		void MenuBar();
		void LogWindow(float);
		void SetSize();

		void SaveShortcut();
		void SaveAsShortcut();
		void OpenShortcut();
		void NewFileShortcut();

		void ShowTab1();
		void ShowTab2();
		void DispCSG_Primitive(GUI::TextEditor::CSG_Primitive & prim);
		void DispCSG_Edge(GUI::TextEditor::CSG_Edge & edge);
		void DispCSG_Node(GUI::TextEditor::CSG_Node & node);
		void ShowCSG();
		void GenerateAlgebricSurface();
		std::string GenerateExpr(const std::string &);
	};
}