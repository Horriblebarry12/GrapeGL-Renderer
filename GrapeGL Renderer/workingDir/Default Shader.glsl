#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 mvp;
out vec3 theFunny;
void main()
{
	theFunny = aPos;
	gl_Position = mvp * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;
in vec3 theFunny;

void main()
{
	FragColor = vec4(theFunny, 1.0f);
}
