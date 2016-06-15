uniform sampler2D u_texture;
uniform float u_progress;
uniform int u_fluxbar;
uniform float u_time;


// Description : Array and textureless GLSL 2D/3D/4D simplex
//               noise functions.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
//

vec3 mod289(vec3 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec4 mod289(vec4 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec4 permute(vec4 x) { return mod289(((x*34.0)+1.0)*x); }
vec4 taylorInvSqrt(vec4 r){ return 1.79284291400159 - 0.85373472095314 * r; }

float snoise(vec3 v)
{
	const vec2  C = vec2(1.0/6.0, 1.0/3.0);
	const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

	// First corner
	vec3 i  = floor(v + dot(v, C.yyy));
	vec3 x0 = v - i + dot(i, C.xxx);

	// Other corners
	vec3 g = step(x0.yzx, x0.xyz);
	vec3 l = 1.0 - g;
	vec3 i1 = min(g.xyz, l.zxy);
	vec3 i2 = max(g.xyz, l.zxy);
	vec3 x1 = x0 - i1 + C.xxx;
	vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
	vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y

	// Permutations
	i = mod289(i);
	vec4 p = permute( permute( permute( i.z + vec4(0.0, i1.z, i2.z, 1.0)) + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

	// Gradients: 7x7 points over a square, mapped onto an octahedron.
	// The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
	float n_ = 0.142857142857; // 1.0/7.0
	vec3  ns = n_ * D.wyz - D.xzx;
	vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)

	vec4 x_ = floor(j * ns.z);
	vec4 y_ = floor(j - 7.0 * x_);    // mod(j,N)

	vec4 x = x_ *ns.x + ns.yyyy;
	vec4 y = y_ *ns.x + ns.yyyy;

	vec4 h = 1.0 - abs(x) - abs(y);
	vec4 b0 = vec4(x.xy, y.xy);
	vec4 b1 = vec4(x.zw, y.zw);

	vec4 s0 = floor(b0) * 2.0 + 1.0;
	vec4 s1 = floor(b1) * 2.0 + 1.0;
	vec4 sh = -step(h, vec4(0.0));

	vec4 a0 = b0.xzyw + s0.xzyw * sh.xxyy;
	vec4 a1 = b1.xzyw + s1.xzyw * sh.zzww;

	vec3 p0 = vec3(a0.xy, h.x);
	vec3 p1 = vec3(a0.zw, h.y);
	vec3 p2 = vec3(a1.xy, h.z);
	vec3 p3 = vec3(a1.zw, h.w);

	//Normalise gradients
	vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));

	p0 *= norm.x;
	p1 *= norm.y;
	p2 *= norm.z;
	p3 *= norm.w;

	// Mix final noise value
	vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
	m = m * m;

	return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), dot(p2,x2), dot(p3,x3)));
}

///////////////////////////////////////////////////////////

void main()
{
	vec4 color = gl_Color * 1.15;
	
	vec2 uv = gl_TexCoord[0].xy;
	vec4 frag = texture2D(u_texture, uv);
	
	float xlen = length(vec2(uv.x, 0.0) - vec2(0.5, 0.0));
	float ylen = length(vec2(uv.y, 0.0) - vec2(0.5, 0.0));
	ylen = (0.5-ylen) * 0.2 + 1.0;
	
	float threshold = u_progress * u_progress * 0.22 + 0.05;
	
	float ss = smoothstep(u_progress-threshold, u_progress * ylen, xlen);
	float invss = 1.0-ss;
	
	frag.rgb *= invss * 0.7;
	
	float my = (uv.y - 0.5) * 2.0;
	float mi = 1.0 / (my * my);
	
	float mx = (uv.x - 0.5) * 15.0;
	mx = 1.0 / (mx * mx) * abs(1.0 - uv.y - 0.5);
	//mx = clamp(mx, 0.0, 1.0);
	
	frag.rgb += snoise(vec3(uv.x * 10.0 + u_time * 0.1, uv.y * 2.0, u_time * 0.3)) * 0.25 * (invss * 0.8 + 0.2);
	
	//frag.rgb *= (mx * 0.2) + 0.7;
	
	float xl = (uv.x - 0.5) / 10.0;
	//xl = 1.0 / (xl * xl);
	
	float metas = (mi / (350.0 + cos(u_time * (uv.x > 0.5 ? 1.0 : -1.0) + xl * 15.0) * 270.0) * invss);
	
	//////////////////
	
	float wx = max(u_progress / 0.475, 0.9) - 0.9;
	float warn = (wx * wx) * 500.0;
	frag.rgb *= cos(u_time * 15.0) * 0.4 * warn * invss + warn * invss * 0.4 + 1.0;
	
	//////////////////
	
	frag.rgb += metas * 0.8;
	frag.rgb *= 0.85;
	frag.rgb += color.rgb * 0.06;
	frag.rgb = min(frag.rgb, 1.0);
	
	float brightness = dot(frag.rgb, vec3(0.2126, 0.7152, 0.0722));
	
	vec4 final = brightness < 0.5
				?
					2.0 * frag * color
				:
					vec4(1.0) - 2.0 * (vec4(1.0) - frag) * (vec4(1.0) - color);
	
	float brig = mix(dot(final.rgb, vec3(0.2126, 0.7152, 0.0722)) * 0.6 + 0.4, 1.0, ss);
	
	gl_FragColor = vec4(final.rgb, frag.a * brig);
}