#include <flgl.h>
#include <flgl/render.h>
#include <flgl/logger.h>
LOG_MODULE(main);

#include "MaterialRenderer.h"

MaterialRenderer renderer;
MousePerspectiveCamera cam;

int main() {
	gl.init();
	window.create("hello window", 1280, 720);
	
	renderer.init();
	cam.update();

	glm::vec3 rot(0.f);

	while (!window.should_close()) {
		static bool look = false;
		if (window.keyboard[GLFW_KEY_K].pressed) {
			if (look)
				{cam.disable_look(); look = !look;}
			else
				{cam.enable_look(); look = !look;}
		}
		if (window.keyboard[GLFW_KEY_W].down) {
			cam.getPos() += 0.08f * cam.readLook();
		}
		if (window.keyboard[GLFW_KEY_A].down) {
			cam.getPos() -= 0.08f * glm::cross(cam.readLook(), cam.readUp());
		}
		if (window.keyboard[GLFW_KEY_S].down) {
			cam.getPos() -= 0.08f * cam.readLook();
		}
		if (window.keyboard[GLFW_KEY_D].down) {
			cam.getPos() += 0.08f * glm::cross(cam.readLook(), cam.readUp());
		}

		gl.enable_depth_test();
		gl.clear();

		renderer.sync(cam);

		rot.y += 0.1f;
		rot.z -= 0.04f;


		renderer.render(genModelMat3d(glm::vec3(0.f), rot, glm::vec3(1.f)));

		cam.update();
		window.update();
	}

	gl.destroy();
	
	return 0;
}
