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
    Material();
    Material(glm::vec3 color, float spec, float shiny = 12.f, glm::vec3 emm = glm::vec3(0.f));
};

struct CubeMesh {
    CubeMesh& init();
    void destroy();
    Mesh<Vt_pn>& mesh();
private:
    Mesh<Vt_pn> m;
};


struct SphereMesh {
    SphereMesh& init(size_t N = 128);
    void destroy();
    Mesh<Vt_pn>& mesh();
private:
    Mesh<Vt_pn> m;
};


struct MaterialRenderer {

    static Shader shader;
    static Mesh<Vt_pn> mesh;

    static void init();
    static void sync(Camera& cam, float amb = 0.08);
    static void sync(Material const& material);
    static void render(glm::mat4 const& model);
    static void destroy();
};


#endif /* MATERIAL_RENDERER_H */
