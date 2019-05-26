#version 330 core
layout (location = 0) in float t;

uniform int n;
uniform vec3 p[];

int fac(int x) {
    int res = 1;
    for(int k = 1; k <= x; k++) {
        res = res * k;
    }
    return res;
}

float B(int i) {
    return fac(n-1) / (fac(i) * fac(n-1-i)) * pow(t, i) * pow((1-t), n-1-i);
}

void main()
{
    vec3 qt = vec3(0.0f, 0.0f, 0.0f);
  
    
    if (n == 1) {
        qt = p[0];
    } else if (n == 2) {
        qt = B(0) * p[0] + B(1) * p[1];
    } else if (n == 3) {
        qt = B(0) * p[0] + B(1) * p[1] + B(2) * p[2];
    } else if (n == 4) {
        qt = B(0) * p[0] + B(1) * p[1] + B(2) * p[2] + B(3) * p[3];
    } else if (n == 5) {
        qt = B(0) * p[0] + B(1) * p[1] + B(2) * p[2] + B(3) * p[3] + B(4) * p[4];
    } else if (n == 6) {
        qt = B(0) * p[0] + B(1) * p[1] + B(2) * p[2] + B(3) * p[3] + B(4) * p[4] + B(5) * p[5];
    } else if (n == 7) {
         qt = B(0) * p[0] + B(1) * p[1] + B(2) * p[2] + B(3) * p[3] + B(4) * p[4] + B(5) * p[5] + B(6) * p[6];
    }

    gl_Position = vec4(qt.x, qt.y, qt.z, 1.0);
}