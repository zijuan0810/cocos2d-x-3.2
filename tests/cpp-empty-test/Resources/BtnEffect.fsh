#ifdef GL_ES								
precision lowp float;						
#endif										
											
varying vec4 v_fragmentColor;				
varying vec2 v_texCoord;					
uniform sampler2D CC_Texture0;	
											
void main()									
{											
	vec4 basecolor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);			
	float df = CC_SinTime.w;
	if(df < 0.0)
		df = -df;
	gl_FragColor = basecolor +  basecolor * df * 0.4;
}
