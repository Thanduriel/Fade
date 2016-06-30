#version 330

uniform float windowSizeX;
uniform float windowSizeY;

out vec2 gs_out_LightPosition;
out vec4 gs_out_LightColor;
out float gs_out_lightRadius;

layout (points) in;

// The output will consist of triangle strips with four vertices each
layout (triangle_strip, max_vertices = 4) out;

vec2 toWindowSpace(vec2 _pos)
{
	_pos = (_pos + vec2(1.f, 1.f)) * 0.5f;
	_pos.x *= windowSizeX;
	_pos.y *= windowSizeY;
	
	return _pos;
}

void main()
{
	float ratio = windowSizeX / windowSizeY;
	
    // Retrieve the passed vertex position
    vec2 pos = gl_in[0].gl_Position.xy;
	gs_out_LightPosition = toWindowSpace(pos);
	gs_out_LightColor = gl_in[0].gl_FrontColor;
	
	float radius = gl_in[0].gl_TexCoord[0].x;
	gs_out_lightRadius = radius;
	radius =  radius / windowSizeX * 2.f;

    // Bottom left vertex
    gl_Position = vec4(-radius, -radius, 0.f, 1.f); gl_Position.y *= ratio;
	gl_Position += vec4(pos, 0.f, 0.f);
    EmitVertex();
		
	gl_Position = vec4(radius, -radius, 0.f, 1.f); gl_Position.y *= ratio;
	gl_Position += vec4(pos, 0.f, 0.f);
    EmitVertex();

	gl_Position = vec4(-radius, radius, 0.f, 1.f); gl_Position.y *= ratio;
	gl_Position += vec4(pos, 0.f, 0.f);
    EmitVertex();

	gl_Position = vec4(radius, radius, 0.f, 1.f); gl_Position.y *= ratio;
	gl_Position += vec4(pos, 0.f, 0.f);
    EmitVertex();
		
    // And finalize the primitive
    EndPrimitive();
}