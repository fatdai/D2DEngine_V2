
varying vec2 v_texCoords;

uniform sampler2D u_sampler2D;
uniform vec4 u_color;

void main(){
    gl_FragColor = u_color * texture2D(u_sampler2D,v_texCoords);
}