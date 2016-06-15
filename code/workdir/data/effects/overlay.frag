uniform sampler2D 	u_texture;
uniform vec4 		u_color;

void main()
{
	vec4 a = texture2D(u_texture, gl_TexCoord[0].xy) * 0.75;
	vec4 b = gl_Color;
	
	b.rgb *= 2.5;
	
	vec4 final = a.r < 0.5
				?
					2.0 * a * b
				:
					vec4(1.0) - 2.0 * (vec4(1.0) - a) * (vec4(1.0) - b);
	
	
	//vec4 final = mix(vec4(0,0,0,1), b, a.r);

	gl_FragColor = vec4(final.rgb, a.a * b.a);
}