#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inMixed1;
layout(location = 1) in vec3 inMixed2;

layout(location = 0) out vec4 fragColor;

void main() {
    vec3 position = vec3(inMixed1.x, inMixed1.z, inMixed2.y);
    vec4 color = vec4(inMixed1.y, inMixed2.x, inMixed2.z, 1.0f);

    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(position, 1.0);
    fragColor = color;
}