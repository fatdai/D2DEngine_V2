
attribute vec2 a_pos;
attribute vec2 a_texCoords;
uniform mat4 u_matrix;

varying vec2 v_texCoords;

void main(){
    gl_Position = u_matrix * vec4(a_pos,1.0,1.0);
    v_texCoords = a_texCoords;
}