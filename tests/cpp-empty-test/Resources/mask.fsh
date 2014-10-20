#ifdef GL_ES								
precision lowp float;						
#endif										
											
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
 
uniform sampler2D u_texture;
uniform sampler2D u_mask;
 
void main() {
    vec4 mainColor = texture2D(u_texture, v_texCoord);
    vec4 maskColor = texture2D(u_mask, v_texCoord);
    vec4 srcColor = vec4(mainColor.r * maskColor.a,
                         mainColor.g * maskColor.a,
                         mainColor.b * maskColor.a,
                         mainColor.a * maskColor.a);
     
    gl_FragColor = srcColor;
}