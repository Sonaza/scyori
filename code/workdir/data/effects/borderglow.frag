uniform sampler2D u_texture;
uniform sampler2D u_mask;
uniform float u_time;

void main()
{
	vec4 frag = texture2D(u_texture, gl_TexCoord[0].xy);
	
	vec2 maskuv = vec2(gl_TexCoord[0].x + cos(mod(u_time * 0.5f, 3.1415926535)) * 0.5, gl_TexCoord[0].y);
	vec4 mask = texture2D(u_mask, maskuv);
	
	gl_FragColor = vec4(frag.rgb * mask.rgb, 1.0);
}