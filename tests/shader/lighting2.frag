#define N_MAX_LIGHTS 16
#define DARKNESS_LEVEL 0.9
#define MIX_LIGHT(n) \
	if (n_lights > n) { \
		vec4 c = light_influence(n, light_color[n]); \
		color = mix(color, c, c.a); \
	}
		
uniform sampler2D tex;
uniform int n_lights;
uniform vec2 light_pos[N_MAX_LIGHTS];
uniform vec3 light_color[N_MAX_LIGHTS];
uniform float light_radius[N_MAX_LIGHTS];
uniform float light_intensity[N_MAX_LIGHTS];

/* Returns the color and the intensity of a single light on 
 * this fragment.
 */
vec4 light_influence(int idx, vec3 lcol) {
	vec2 l = light_pos[idx];
	vec2 c = gl_FragCoord.xy;

	float dist = (l.x - c.x)*(l.x - c.x) + (l.y - c.y)*(l.y - c.y);
	float alpha = light_intensity[idx] * (1000.0/(1.0+dist));//(1.0 - dist / light_radius[idx] / light_radius[idx]);
	alpha = max(alpha, 0.0);

	return vec4(lcol.xyz, alpha);
}

/* Calculate the sum of light contributions for this fragment 
 * and return it.
 */
vec4 calc_light_contrib() {
	int i = 0;
	vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 c;

	// This is needed since GLSL doesn't support indexing with variables
	MIX_LIGHT(0)
	MIX_LIGHT(1)
	MIX_LIGHT(2)
	MIX_LIGHT(3)
	MIX_LIGHT(4)
	MIX_LIGHT(5)
	MIX_LIGHT(6)
	MIX_LIGHT(7)
	MIX_LIGHT(8)
	MIX_LIGHT(9)
	MIX_LIGHT(10)
	MIX_LIGHT(11)
	MIX_LIGHT(12)
	MIX_LIGHT(13)
	MIX_LIGHT(14)
	MIX_LIGHT(15)

	return color;
}

void main() {
	vec4 color = texture2D(tex, gl_TexCoord[0].st);

	// Add the initial darkness
	vec4 darkness = vec4(0.01, 0.01, 0.01, DARKNESS_LEVEL);
	color = mix(color, darkness, darkness.a);

	// Add the lights contribution
	vec4 light_contrib = calc_light_contrib();
	//color = mix(color, light_contrib, light_contrib.a);
	color += light_contrib;

	// Blend the colors together
	gl_FragColor = color;
}
