#version 410 core

in vec3 iPos;
in vec3 iNorm;

out vec4 outcolor;

uniform vec3 uka, ukd, uke, uks;
uniform float ualpha;
uniform float uAmb;
uniform vec3 ucampos;

void main() {
    const vec3 ldir = normalize(vec3(-1, -6, -2));

    const vec3 l = -ldir;
    vec3 r = reflect(ldir, iNorm);
    vec3 v = normalize(ucampos - iPos);

    vec3 amb = uka * uAmb;
    vec3 emm = uke;
    vec3 diff = ukd * max(0., dot(l, iNorm));
    vec3 spec = uks * pow(max(dot(v,r), 0), ualpha);

    vec3 sum = min(vec3(1.f), amb + diff + spec + emm);

    outcolor = vec4(sum, 1.);
}

