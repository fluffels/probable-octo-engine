#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;

/* Parameters. */
uniform float amplitude = 10.0f;
uniform float time = 0.0f;
uniform float length = 200.0f;
uniform float speed = 2.0f;

uniform vec2 dir1 = normalize(vec2(0.092f, 0.819f));
uniform vec2 dir2 = normalize(vec2(0.983f, 0.690f));
uniform vec2 dir3 = normalize(vec2(0.833f, 0.142f));
uniform vec2 dir4 = normalize(vec2(0.77f, 0.410f));
uniform vec2 dir5 = normalize(vec2(0.927f, 0.927f));

/* Physical quantities. */
float A = amplitude;
float w = (2 * 3.14f) / length;
float phi = speed * w;
float Q = 1 / (w * A);

out vec3 normal;
out vec3 view_dir;

float waveFunc(vec2 dir, vec2 coords)
{
   return A * sin(dot(dir, coords) * w + time * phi);
}

float waveFuncDdx(vec2 dir, vec2 coords)
{
   return w * dir.x * A * cos(dot(dir, coords) * w + time * phi);
}

float waveFuncDdz(vec2 dir, vec2 coords)
{
   return w * dir.y * A * cos(dot(dir, coords) * w + time * phi);
}

vec3 normalFunc(vec2 dir, vec2 coords, float att)
{
   vec3 n;

   n.x = -1 * waveFuncDdx(dir, coords);
   n.y = 1;
   n.z = -1 * waveFuncDdz(dir, coords);

   return normalize(n);
}

vec3 gerstNormalFunc(vec2 dir, vec2 coords, float att)
{
   float d = waveFunc(dir, coords);
   vec2 P = vec2(coords.x + d, coords.y + d);

   float wa = w * A;
   float s = sin(w * dot(dir, P) + phi * time);
   float c = cos(w * dot(dir, P) + phi * time);

   vec3 n;
   n.x = -1 * dir.x * wa * c;
   n.y = Q * wa * s;
   n.z = -1 * dir.y * wa * c;

   return n;
}

void main()
{
   for (int i = 0; i < gl_in.length(); i++)
   {
      vec3 position = gl_in[i].gl_Position.xyz;

      vec4 worldP = world * vec4(position, 1.0f);

      float accum = 0.0f;
      accum += waveFunc(dir1, worldP.xz) * cos(time);
      accum += waveFunc(dir2, worldP.xz) * cos(time);
      accum += waveFunc(dir3, worldP.xz) * cos(time);
      accum += waveFunc(dir4, worldP.xz) * cos(time);
      accum += waveFunc(dir5, worldP.xz) * cos(time);
      accum /= 5;

      position.x += accum * Q;
      position.y = accum;
      position.z += accum * Q;

      gl_Position = projection * view * world * vec4(position, 1.0f);

      vec3 n = vec3(0, 0, 0);
      n += gerstNormalFunc(dir1, worldP.xz, time);
      n += gerstNormalFunc(dir2, worldP.xz, time);
      n += gerstNormalFunc(dir3, worldP.xz, time);
      n += gerstNormalFunc(dir4, worldP.xz, time);
      n += gerstNormalFunc(dir5, worldP.xz, time);

      normal = normalize(n);

      vec4 viewer = inverse(view) * vec4(0, 0, 0, 1);
      view_dir = normalize(viewer.xyz - position);

      EmitVertex();
   }

   EndPrimitive();
}
