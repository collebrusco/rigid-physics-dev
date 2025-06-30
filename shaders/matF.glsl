#version 410 core

in vec3 iPos;
in vec3 iNorm;

out vec4 outcolor;

uniform vec3 uka, ukd, uke, uks;
uniform float ualpha;

void main() {
    const vec3 ldir = normalize(vec3(-1, -6, -1));

    const vec3 l = -ldir;
    vec3 r = reflect(l, iNorm);

    vec3 base = vec3(1.) * max(0., dot(l, iNorm));

    base += vec3(0.11);

    base = min(vec3(1.f), base);

    outcolor = vec4(base, 1.);
}

