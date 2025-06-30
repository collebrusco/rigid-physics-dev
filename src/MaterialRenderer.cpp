#include "MaterialRenderer.h"

Shader MaterialRenderer::shader;
Mesh<Vt_pn> MaterialRenderer::mesh;

#define QUAD (0.5f)

#define POSX {1.f, 0.f, 0.f}
#define NEGX {-1.f, 0.f, 0.f}
#define POSY {0.f, 1.f, 0.f}
#define NEGY {0.f, -1.f, 0.f}
#define POSZ {0.f, 0.f, 1.f}
#define NEGZ {0.f, 0.f, -1.f}

void MaterialRenderer::init() {
    shader = Shader::from_source("matV", "matF");
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
    mesh = Mesh<Vt_pn>::from_arrays(24, verts, 36, elems);
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
