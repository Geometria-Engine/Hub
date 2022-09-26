#ifndef DRAWCALL_IMGUI_GPU
#define DRAWCALL_IMGUI_GPU

#include <vector>
#include <array>

struct ImGui_GPU
{
	int last_active_texture = -1;

	int last_program = -1;
	int last_texture = -1;
	int last_sampler = -1;
	int last_array_buffer = -1;
	int last_vertex_array_object = -1;

	int last_polygon_mode[2] = {-1};
	int last_viewport[4] = {-1};
	int last_scissor_box[4] = {-1};

	int last_blend_src_rgb;
	int last_blend_dst_rgb;
	int last_blend_src_alpha;
	int last_blend_dst_alpha;
	int last_blend_equation_rgb;
	int last_blend_equation_alpha;

	bool IsBlendEnabled();

	int GetActiveTexture();

	int GetProgram();
	int GetTexture();
	int GetSampler(int GLVersion);
	int GetArrayBuffer();
	int GetVAObject();

	void GetPolygonMode(int* array);
	void GetViewport(int* array);
	void GetScissorBox(int* array);

	int GetBlendSourceRGB();
	int GetBlendDestRGB();
	int GetBlendSourceAlpha();
	int GetBlendDestAlpha();
	int GetBlendEquationRGB();
	int GetBlendEquationAlpha();
};

#endif