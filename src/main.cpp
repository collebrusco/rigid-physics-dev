#include <flgl.h>
#include <flgl/render.h>
#include <flgl/logger.h>
LOG_MODULE(main);

#include "MaterialRenderer.h"

MaterialRenderer renderer;
SkyboxRenderer skyrend;
MouseMovePerspectiveCamera cam;


Material mat;


int main() {
	gl.init();
	window.create("hello window", 1280, 720);
	
	renderer.init();
	skyrend.init();
	cam.update(1.f/80.f);
	cam.enable_look();

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

		bool change = false;
		if (window.keyboard[GLFW_KEY_LEFT_BRACKET].pressed) {
			change = true;
			mat = Material(glm::vec3(0.4f, 0.8f, 0.4f), glm::max(0.f, mat.ks.x - 0.025f), mat.alpha);
		}
		if (window.keyboard[GLFW_KEY_RIGHT_BRACKET].pressed) {
			change = true;
			mat = Material(glm::vec3(0.4f, 0.8f, 0.4f), glm::min(1.f, mat.ks.x + 0.025f), mat.alpha);			
		}
		if (window.keyboard[GLFW_KEY_UP].pressed) {
			change = true;
			mat.alpha += 0.5f;
		}
		if (window.keyboard[GLFW_KEY_DOWN].pressed) {
			change = true;
			mat.alpha = glm::max(1.f, mat.alpha - 0.5f);
		}
		if (change) renderer.sync(mat);

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
