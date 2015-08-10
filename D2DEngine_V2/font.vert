attribute vec2 a_pos;
attribute vec2 a_texCoords;
//attribute vec4 a_color;

uniform mat4 u_matrix;

varying vec2 v_texCoords;
//varying vec4 v_color;
void main(){
    gl_Position = u_matrix * vec4(a_pos,1.0,1.0);
    v_texCoords = a_texCoords;
//    v_color = a_color;
}