uniform sampler2D 	u_texture;
uniform vec2 		u_resolution;
uniform float		u_flicker;

uniform sampler2D	u_grid;

void main()
{
	vec2 uv = gl_TexCoord[0].xy;
	
	vec2 pos = (gl_FragCoord.xy / u_resolution.xy) - vec2(0.5);
	pos.x *= u_resolution.x / u_resolution.y;
	
	float len = length(pos);
	//float vignette = smoothstep(1.4 + u_flicker / 15.f, 0.4, len);
	float vignette = smoothstep(0.5, 1.0, len) * (1.1 + u_flicker / 10.0) + 1.04;
	
	//float saturate = smoothstep(0.5, 0.25, len) * 0.5 + 1.0;
	
	float scanline = cos(gl_FragCoord.y / u_resolution.y * 1100.0) * (0.04 + smoothstep(0.8, 0.3, len) / 20.0) + 1.0;

	//scanline = mix(scanline, 1.0, 1.0 - (sin(uv.y * 3.1415926535) * 0.5 + 0.5));

	// float pix = min(20.0, 1.0 + len * 25.0);
	// float pix = 8.0;
	// vec2 fc = floor(gl_FragCoord / pix) * pix;
	// uv = fc / u_resolution;
	
	vec4 fragment = texture2D(u_texture, uv);
	
	vec4 gray = vec4(vec3(dot(fragment.rgb, vec3(0.299, 0.587, 0.114))), fragment.a);
	
	vec4 final = mix(fragment, gray, len / 1.75);
	
	vec2 hguv = vec2(gl_FragCoord.xy / u_resolution.xy);// - vec2(0.5);
	float gridvalue = 1.0-texture2D(u_grid, hguv).a * smoothstep(0.65, 1.1, len);
	
	gridvalue = mix(gridvalue, 1.0, 0.5);
	
	vec3 hexgrid = vec3(gridvalue) * vec3(0.25, 0.4, 0.1);

	gl_FragColor = vec4(final.rgb * scanline * vignette * gridvalue, final.a);
}