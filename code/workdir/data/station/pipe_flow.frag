uniform sampler2D u_texture;
uniform sampler2D u_mask;
uniform float u_time;

#define PI 3.14159265358979323

#define sq(_x) ((_x) * (_x))

void main()
{
	vec2 uv = gl_TexCoord[0].xy;
	vec4 frag = texture2D(u_texture, uv);
	
	vec2 maskuv = uv + vec2(
		cos(u_time + uv.y * 2.0) * 0.04,
		cos(u_time + uv.y * 10.0 + uv.x * 2.0) * -0.04 + mod(u_time / 20.0, 1.0));
	vec4 mask = texture2D(u_mask, maskuv);
	mask.rgb = mix(vec3(0.4, 0.4, 0.4), mask.rgb, 0.3);
	
	float mx = uv.x - 0.5;// + cos(u_time + uv.y * 50.0 + sin(uv.y * 20.0) * 15.0) * 0.15;
	float mi = 1.0 / (mx * mx);
	
	//float ei = 1.0 / sq((uv.y - 0.95) / 3.0) + 1.0 / sq((uv.y - 0.01) / 3.0);
	
	float i = (cos(u_time * 2.0 + uv.y * 130.0) * 0.5 + 0.5) * 0.5 + 0.6
				 + mi / (1200.0 - cos(u_time + uv.y * 150.0) * 440.0 - 440.0);
				// + max(0.0, ei / 300.0 - 0.5);
	
	float fb = dot(frag.rgb, vec3(0.2126, 0.7152, 0.0722));
	float ss = smoothstep(0.2, 0.5, fb);
	
	frag.rgb *= mix(1.0, i, ss);
	//frag.rgb *= mix(1.0, mask.rgb, ss);
	
	//float mb = dot(frag.rgb, vec3(0.2126, 0.7152, 0.0722));
	
	vec4 b = frag;
	vec4 a = mask;
	
	float brightness = 1.0-dot(a.rgb, vec3(0.2126, 0.7152, 0.0722));
	
	vec4 final = a.r < 0.5
				?
					2.0 * a * b
				:
					vec4(1.0) - 2.0 * (vec4(1.0) - a) * (vec4(1.0) - b);
	
	gl_FragColor = vec4(mix(frag.rgb, final.rgb, ss), frag.a);
}