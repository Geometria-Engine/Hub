#include "DrawCallImGuiGPU.h"
#include <GL/glew.h>
#include <glfw3.h>

int ImGui_GPU::GetActiveTexture()
{
	if(last_active_texture == -1)
		glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);

	return last_active_texture;
}

int ImGui_GPU::GetProgram()
{
	if(last_program == -1)
		glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);

	return last_program;
}

int ImGui_GPU::GetTexture()
{
	if(last_texture == -1)
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);

	return last_texture;
}

int ImGui_GPU::GetSampler(int GLVersion)
{
	if(last_sampler == -1)
	{
		if (GLVersion >= 330) 
			glGetIntegerv(GL_SAMPLER_BINDING, &last_sampler);
		else 
			last_sampler = 0;
	}

	return last_sampler;
}

int ImGui_GPU::GetArrayBuffer()
{
	if(last_array_buffer == -1)
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);

	return last_array_buffer;
}

int ImGui_GPU::GetVAObject()
{	
	if(last_vertex_array_object == -1)
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array_object);

	return last_vertex_array_object;
}

void ImGui_GPU::GetPolygonMode(int* array)
{
	if(last_polygon_mode[0] == -1)
		glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);

	array = last_polygon_mode;
}

void ImGui_GPU::GetViewport(int* array)
{
	if(last_viewport[0] == -1)
		glGetIntegerv(GL_VIEWPORT, last_viewport);

	array = last_viewport;
}

void ImGui_GPU::GetScissorBox(int* array)
{
	if(last_scissor_box[0] == -1)
		glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);

	array = last_scissor_box;
}

int ImGui_GPU::GetBlendSourceRGB()
{
	if(last_blend_src_rgb == -1)
		glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);

	return last_blend_src_rgb;
}

int ImGui_GPU::GetBlendDestRGB()
{
	if(last_blend_dst_rgb == -1)
		glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);

	return last_blend_dst_rgb;
}

int ImGui_GPU::GetBlendSourceAlpha()
{
	if(last_blend_src_alpha == -1)
		glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);

	return last_blend_src_alpha;
}

int ImGui_GPU::GetBlendDestAlpha()
{
	if(last_blend_dst_alpha == -1)
		glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);

	return last_blend_dst_alpha;
}

int ImGui_GPU::GetBlendEquationRGB()
{
	if(last_blend_equation_rgb == -1)
		glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb);

	return last_blend_equation_rgb;
}

int ImGui_GPU::GetBlendEquationAlpha()
{
	if(last_blend_equation_alpha == -1)
		glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha);

	return last_blend_equation_alpha;
}

bool ImGui_GPU::IsBlendEnabled()
{
	return true;
}