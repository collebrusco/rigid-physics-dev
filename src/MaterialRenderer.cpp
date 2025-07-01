#include "MaterialRenderer.h"
#include <flgl/logger.h>
LOG_MODULE(matrend);

Material::Material() {
    kd = ka = glm::vec3(1.f,0.f,1.f);
    ks = glm::vec3(1.f);
    ke = glm::vec3(0.f);
    alpha = 12.f;
}

/** set up a material from:
 * @param color base material color
 * @param spec how specular the object is from 0 to 1: where 0 is completely dull and 1 is completely chrome
 * @param shiny the specular coeffecient, higher is a tighter specular highlight, lower is more spread out 
 * @param emm emmissive color of the mat (optional)
 */
Material::Material(glm::vec3 color, float spec, float shiny, glm::vec3 emm) {
    assert(spec >= 0.f && spec <= 1.f && "spec is a 0 to 1 slider to go from dull to shiny");
    const float dull = 1.f - spec;
    kd = ka = color * dull;
    ks = glm::vec3(spec);
    alpha = shiny;
    ke = emm;
}


#define QUAD (0.5f)

#define POSX {1.f, 0.f, 0.f}
#define NEGX {-1.f, 0.f, 0.f}
#define POSY {0.f, 1.f, 0.f}
#define NEGY {0.f, -1.f, 0.f}
#define POSZ {0.f, 0.f, 1.f}
#define NEGZ {0.f, 0.f, -1.f}

CubeMesh& CubeMesh::init() {
    Vt_pn verts[] = {
        /* ^z   >y*/
        {{ QUAD,  QUAD, -QUAD}, POSX},
        {{ QUAD,  QUAD,  QUAD}, POSX},
        {{ QUAD, -QUAD,  QUAD}, POSX},
        {{ QUAD, -QUAD, -QUAD}, POSX},

        /* ^z   >-y*/
        {{-QUAD, -QUAD, -QUAD}, NEGX},
        {{-QUAD, -QUAD,  QUAD}, NEGX},
        {{-QUAD,  QUAD,  QUAD}, NEGX},
        {{-QUAD,  QUAD, -QUAD}, NEGX},

        /* ^z   >-x*/
        {{-QUAD,  QUAD, -QUAD}, POSY},
        {{-QUAD,  QUAD,  QUAD}, POSY},
        {{ QUAD,  QUAD,  QUAD}, POSY},
        {{ QUAD,  QUAD, -QUAD}, POSY},

        /* ^-z   >-x*/
        {{ QUAD, -QUAD, -QUAD}, NEGY},
        {{ QUAD, -QUAD,  QUAD}, NEGY},
        {{-QUAD, -QUAD,  QUAD}, NEGY},
        {{-QUAD, -QUAD, -QUAD}, NEGY},

        /* ^y   >-x*/
        {{ QUAD, -QUAD,  QUAD}, POSZ},
        {{ QUAD,  QUAD,  QUAD}, POSZ},
        {{-QUAD,  QUAD,  QUAD}, POSZ},
        {{-QUAD, -QUAD,  QUAD}, POSZ},

        /* ^y   >x*/
        {{-QUAD, -QUAD, -QUAD}, NEGZ},
        {{-QUAD,  QUAD, -QUAD}, NEGZ},
        {{ QUAD,  QUAD, -QUAD}, NEGZ},
        {{ QUAD, -QUAD, -QUAD}, NEGZ},
    };
    #define QUAD_ELEMS(start) (start)+0,(start)+2,(start)+1,  (start)+0,(start)+3,(start)+2
    uint32_t elems[] = {
        QUAD_ELEMS(0),
        QUAD_ELEMS(4),
        QUAD_ELEMS(8),
        QUAD_ELEMS(12),
        QUAD_ELEMS(16),
        QUAD_ELEMS(20),
    };
    m = Mesh<Vt_pn>::from_arrays(24, verts, 36, elems);
    return *this;
}

void CubeMesh::destroy() {
    m.destroy();
}

Mesh<Vt_pn>& CubeMesh::mesh() {
    return m;
}




SphereMesh& SphereMesh::init(size_t N) {
    const float PI = 3.14159265359f;
    const float dPhi = PI / N;
    const float dTheta = 2.0f * PI / N;

    std::vector<Vt_pn> verts;
    std::vector<uint32_t> elems;

    for (int i = 0; i <= N; ++i) {
        float phi = i * dPhi;
        for (int j = 0; j <= N; ++j) {
            float theta = j * dTheta;

            float x = sinf(phi) * cosf(theta);
            float y = cosf(phi);
            float z = sinf(phi) * sinf(theta);

            glm::vec3 pos = {x, y, z};
            pos = glm::normalize(pos);
            verts.push_back({pos/2.f, pos});
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int curr = i * (N + 1) + j;
            int next = (i + 1) * (N + 1) + j;

            elems.push_back(curr);
            elems.push_back(next);
            elems.push_back(curr + 1);

            elems.push_back(curr + 1);
            elems.push_back(next);
            elems.push_back(next + 1);
        }
    }

    m = Mesh<Vt_pn>::from_vectors(verts, elems);
    return *this;
}


void SphereMesh::destroy() {
    m.destroy();
}

Mesh<Vt_pn>& SphereMesh::mesh() {
    return m;
}



Shader MaterialRenderer::shader;
Mesh<Vt_pn> MaterialRenderer::mesh;

void MaterialRenderer::init() {
    shader = Shader::from_source("matV", "matF");
    
    mesh = SphereMesh().init().mesh();
}

void MaterialRenderer::destroy() {
    shader.destroy();
    mesh.destroy();
}

void MaterialRenderer::sync(Camera &cam, float amb) {
    shader.uViewProj(cam.view(), cam.proj());
    shader.uVec3("ucampos", cam.readPos());
    shader.uFloat("uAmb", amb);
}

void MaterialRenderer::sync(Material const &material) {
    shader.uVec3("uka", material.ka);
    shader.uVec3("ukd", material.kd);
    shader.uVec3("uke", material.ke);
    shader.uVec3("uks", material.ks);
    shader.uFloat("ualpha", material.alpha);
}

void MaterialRenderer::render(glm::mat4 const &model) {
    shader.bind();
    shader.uMat4("uModel", model);
    mesh.bind();
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    gl.draw_mesh(mesh);
    mesh.unbind();
    shader.unbind();
}
