#define N_MAX_LIGHTS 16
#define STARTING_ALPHA 0.0

uniform sampler2D tex;
uniform int n_lights;
uniform vec2 lights[N_MAX_LIGHTS];
uniform float light_intensities[N_MAX_LIGHTS];

float light_influence(int idx) {
	vec2 l = lights[idx], c = gl_FragCoord.xy;
	float dist = (l.x - c.x)*(l.x - c.x) + (l.y - c.y)*(l.y - c.y);
	return light_intensities[idx] * 10000.0 / dist;
}

float calc_color() {
	int i;
	float alpha = STARTING_ALPHA;
	for (i = 0; i < n_lights; ++i) {
		alpha += light_influence(i);	
	}
	return alpha;
}

void main() {
	vec4 color = texture2D(tex, gl_TexCoord[0].st);
	color.a = calc_color();
	gl_FragColor = color;
}
