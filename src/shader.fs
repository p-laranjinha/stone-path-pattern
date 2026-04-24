#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord; // Actually the pixel coords here
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables
const int N = 10;
uniform vec2[N] points;

uniform float edgeBlur = 1.0;
uniform float smoothness = 0.0;

// SDF from Inigo Quilez : https://www.shadertoy.com/view/wdBXRW
float dot2(in vec2 v) {
  return dot(v, v);
}
float cross2d(in vec2 v0, in vec2 v1) {
  return v0.x * v1.y - v0.y * v1.x;
}

float sdPolygon(in vec2 p, in vec2[N] v)
{
  int num = v.length();
  float d = dot(p - v[0], p - v[0]);
  float s = 1.0;
  for (int i = 0, j = num - 1; i < num; j = i, i++)
  {
    // distance
    vec2 e = v[j] - v[i];
    vec2 w = p - v[i];
    vec2 b = w - e * clamp(dot(w, e) / dot(e, e), 0.0, 1.0);
    d = min(d, dot(b, b));

    // winding number from http://geomalgorithms.com/a03-_inclusion.html
    bvec3 cond = bvec3(p.y >= v[i].y,
        p.y < v[j].y,
        e.x * w.y > e.y * w.x);
    if (all(cond) || all(not(cond))) s = -s;
  }

  return s * sqrt(d);
}

void main()
{
  vec4 texelColor = texture(texture0, fragTexCoord) * colDiffuse * fragColor;

  float d = sdPolygon(fragTexCoord, points);

  //texelColor = vec4(1.0, 0.0, 1.0, 1.0);
  float inside = smoothstep(-edgeBlur * 0.5, edgeBlur * 0.5, -d + smoothness);
  finalColor = vec4(texelColor.xyz, inside);
}
