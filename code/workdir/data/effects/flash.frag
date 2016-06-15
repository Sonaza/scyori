uniform sampler2D u_texture;

void main()
{
	vec4 tex = texture2D(u_texture, gl_TexCoord[0].xy);
	
	vec3 mixed = mix(tex.rgb, vec3(1.0), min(1.0, gl_Color.r * 2.8));
	
	gl_FragColor = vec4(mixed, tex.a);
}