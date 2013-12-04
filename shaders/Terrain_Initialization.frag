#version 400


out vec4 out_flux;			// f_l, f_r, f_t, f_b
out vec4 out_terrainAttr;
out vec4 out_velocity;

void main (void)
{
	out_flux = vec4(0,0,0,0);
	out_terrainAttr = vec4(0,0,0,0);
	out_velocity = vec4(0,0,0,0);
}