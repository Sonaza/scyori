uniform sampler2D u_texture;
uniform float u_progress;

uniform float u_hover;

void main()
{
	vec2 uv = gl_TexCoord[0].xy;
	vec4 frag = texture2D(u_texture, uv);
	
	float pass = uv.x > u_progress || frag.r < 0.7 ? 1.0 : 0.0;
	vec3 final = mix(vec3(219 / 285.0, 155 / 285.0, 98 / 285.0), frag.rgb, pass);
	
	pass = frag.r > 0.7 && u_hover > 0.9 ? 1.0 : 0.0;
	final = mix(final, final * 1.15, pass);
	
	gl_FragColor = vec4(final, frag.a * gl_Color.a);
}