/** 
 * MaterialRenderer.h 
 * created 06/26/25 by frank collebrusco
 */
#ifndef MATERIAL_RENDERER_H
#define MATERIAL_RENDERER_H
#include <flgl.h>
#include <flgl/tools.h>
#include <flgl/glm.h>

/** ambient, diffuse, emmisive, specular, alpha (specularity coeff) */
struct Material {
    glm::vec3 ka, kd, ke, ks;
    float alpha;
};

struct MaterialRenderer {

    static Shader shader;

    void init();
    void sync(Camera& cam);
    // void prepare
};


#endif /* MATERIAL_RENDERER_H */
