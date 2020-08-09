uniform float lineWidth;
uniform vec3 lineColor;

noperspective in vec3 EdgeDistance;
out vec4 outColor;
in vec4 ZPosition;

vec3 rainbowColor(float f) {
  float r, g, b;

  float a = (1.0 - f) * 4;
  float X = floor(a);
  float Y = floor(255 * (a - X));

  if (X == 0) {
    r = 255.0f;
    g = Y;
    b = 0.0f;
  } else if (X == 1) {
    r = 255.0f-Y;
    g = 255;
    b = 0.0f;
  } else if (X == 2) {
    r = 0.0f;
    g = 255;
    b = Y;
  } else if (X == 3) {
    r = 0.0f;
    g = 255-Y;
    b = 255.0f;
  } else if (X == 4) {
    r = 0.0f;
    g = 0.0f;
    b = 255.0f;
  }

  return vec3(clamp(r / 255, 0.0f, 1.0f), clamp(g / 255, 0.0f, 1.0f),
              clamp(b / 255, 0.0f, 1.0f));
}

float edgeMix() {
  // Find the smallest distance
  float d = min(min(EdgeDistance.x, EdgeDistance.y), EdgeDistance.z);

  if (d < lineWidth - 1) {
    return 1.0;
  } else if (d > lineWidth + 1) {
    return 0.0;
  } else {
    float x = d - (lineWidth - 1);
    return exp2(-2.0 * (x * x));
  }
}

void main() {
  float mixVal = edgeMix();
  outColor = vec4(mix(rainbowColor(ZPosition.z), lineColor, mixVal), 1.0f);
}