uniform float u_time;
uniform vec2 u_resolution;
uniform vec2 u_vresolution;

uniform sampler2D u_texture;

void main()
{
	vec2 uv = gl_TexCoord[0].xy;
	uv = vec2(
		uv.x + cos(u_time * 15.f + uv.y * 10.f) / 200.f,
		uv.y + sin(u_time * 15.f + uv.x * 10.f) / 200.f
	);
	
	vec4 c = texture2D(u_texture, uv);
	
	gl_FragColor = vec4(c.g, c.b, c.r, c.a);
}