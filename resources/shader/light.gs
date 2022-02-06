#version 330

uniform float windowSizeX;
uniform float windowSizeY;

uniform vec2 wallInfos[27];
uniform int numWalls_x3;

in vec4 vs_out_LightColor[1];

out vec2 simpleWallInfos[27];
out vec2 gs_out_LightPosition;
out vec4 gs_out_LightColor;
out float gs_out_lightRadius;

layout (points) in;

// The output will consist of triangle strips with four vertices each
layout (triangle_strip, max_vertices = 4) out;


#define CMPSWAP(a,b, a2, b2) if(a < b) {float tmp = a; a = b; b = tmp; vec2 tmpVec = a2; a2 = b2; b2 = tmpVec;}

float distanceSq(vec2 _a, vec2 _b)
{
	vec2 dif = _a - _b;
	return dif.x * dif.x + dif.y * dif.y;
}

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
	gs_out_LightColor = vs_out_LightColor[0];//gl_in[0].gl_FrontColor;
	
	float radius = gl_in[0].gl_TexCoord[0].x;
	gs_out_lightRadius = radius;
	radius =  radius / windowSizeX * 2.f;

	
	//calculate which line describes the walls best from this lights perspective
	for(int i = 0; i < numWalls_x3; i += 3)
	{
		if(gs_out_LightPosition.x > wallInfos[i].x && gs_out_LightPosition.x < wallInfos[i+1].x)
		{
			float y = abs(wallInfos[i].y - gs_out_LightPosition.y ) < abs(wallInfos[i+1].y - gs_out_LightPosition.y ) ?
						wallInfos[i].y : wallInfos[i+1].y;
			
			simpleWallInfos[i] = vec2(wallInfos[i].x, y);
			simpleWallInfos[i+1] = vec2(wallInfos[i+1].x, y);
		}
		else if(gs_out_LightPosition.y < wallInfos[i].y && gs_out_LightPosition.y > wallInfos[i+1].y)
		{
			float x = abs(wallInfos[i].x - gs_out_LightPosition.x ) < abs(wallInfos[i+1].x - gs_out_LightPosition.x ) ?
						wallInfos[i].x : wallInfos[i+1].x;
			
			simpleWallInfos[i] = vec2(x, wallInfos[i].y);
			simpleWallInfos[i+1] = vec2(x, wallInfos[i+1].y);
		}
		else //take the diagonal with the medium distance points
		{
			float dist[4];
			vec2 points[4];
			points[0] = wallInfos[i];
			points[1] = wallInfos[i+1];
			points[2] = vec2(wallInfos[i].x, wallInfos[i+1].y);
			points[3] = vec2(wallInfos[i+1].x, wallInfos[i].y);
			
			for(int j = 0; j < 4; ++j) dist[j] = distanceSq(points[j],gs_out_LightPosition);
			
			CMPSWAP(dist[0], dist[1], points[0], points[1]);
			CMPSWAP(dist[2], dist[3], points[2], points[3]);
			CMPSWAP(dist[1], dist[3], points[1], points[3]);
			CMPSWAP(dist[0], dist[2], points[0], points[2]);
			CMPSWAP(dist[1], dist[2], points[1], points[2]);
			
			simpleWallInfos[i] = points[1];
			simpleWallInfos[i+1] = points[2];
		}
	}
	
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