#include <flgl.h>
#include <flgl/render.h>
#include <flgl/logger.h>
LOG_MODULE(main);

#include "MaterialRenderer.h"

MaterialRenderer renderer;
SkyboxRenderer skyrend;
MouseMovePerspectiveCamera cam;


int main() {
	gl.init();
	window.create("hello window", 1280, 720);
	
	renderer.init();
	skyrend.init();
	cam.update(1.f/80.f);
	cam.enable_look();

	Material mat;
	mat.alpha = 8.;
	mat.ka = mat.kd = glm::vec3(0.4f, 0.8f, 0.4f);
	mat.ks = glm::vec3(0.7);
	mat.ke = glm::vec3(0.f);

	renderer.sync(mat);

	glm::vec3 rot(0.f);

	while (!window.should_close()) {
		if (window.keyboard[GLFW_KEY_ESCAPE].down) window.close();
		static bool look = true;
		if (window.keyboard[GLFW_KEY_E].pressed) {
			if (look)
				{cam.disable_look(); look = !look;}
			else
				{cam.enable_look(); look = !look;}
		}

		gl.enable_depth_test();
		gl.clear();

		skyrend.sync(cam);
		skyrend.render();


		renderer.sync(cam);

		rot.y += 0.1f;
		rot.z -= 0.4f;


		renderer.render(genModelMat3d(glm::vec3(0.f), rot, glm::vec3(1.f)));

		cam.update(1.f/80.f);
		window.update();
	}

	gl.destroy();
	
	return 0;
}
