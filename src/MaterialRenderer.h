/** 
 * MaterialRenderer.h 
 * created 06/26/25 by frank collebrusco
 */
#ifndef MATERIAL_RENDERER_H
#define MATERIAL_RENDERER_H
#include <flgl.h>
#include <flgl/render.h>
#include <flgl/glm.h>

/** ambient, diffuse, emmisive, specular, alpha (specularity coeff) */
struct Material {
    glm::vec3 ka, kd, ke, ks;
    float alpha;
};

struct MaterialRenderer {

    static Shader shader;
    static Mesh<Vt_pn> mesh;

    static void init();
    static void sync(Camera& cam);
    static void render(glm::mat4 const& model);
    static void destroy();
};


#endif /* MATERIAL_RENDERER_H */
