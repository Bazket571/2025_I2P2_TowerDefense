#version 330 core

attribute vec4 al_pos;
attribute vec4 al_color;
attribute vec2 al_texcoord;
uniform mat4 al_projview_matrix;
uniform bool al_use_tex_matrix;
uniform mat4 al_tex_matrix;

uniform mat4 lightView;
uniform mat4 ortho;

varying vec4 varying_color;
varying vec2 varying_texcoord;
void main()
{
  varying_color = al_color;
  gl_Position = ortho * lightView * al_pos;
}
