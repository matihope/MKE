#include "MKE/Camera.hpp"
#include "MKE/Event.hpp"
#include "MKE/Game.hpp"
#include "MKE/Input.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/Nodes/3d/CubeShape.hpp"
#include "MKE/ResPath.hpp"
#include "MKE/ResourceManager.hpp"
#include "MKE/Shader.hpp"
#include "MKE/WorldEntity.hpp"
#include "glad/glad.h"

class Cube: public mk::CubeShape {
public:
	void onReady(mk::Game&) override { setOrigin(mk::math::Vector3f{ 0.5f }); }

	void onPhysicsUpdate(mk::Game&, float dt) override { rotate(mk::math::Vector3f{ dt * 0.2f }); }

	void onDraw(mk::RenderTarget& target, mk::DrawContext context, const mk::Game& game)
		const override {
		context.shader = shader;

		mk::CubeShape::onDraw(target, context, game);
	}

	const mk::Shader* shader = nullptr;
};

class LightSource: public mk::CubeShape {
public:
	void onReady(mk::Game&) override { setOrigin(mk::math::Vector3f{ -4.5f, 0.5f, 0.5f }); }

	void onPhysicsUpdate(mk::Game&, float dt) override {
		rotate({ 0.f, dt, 0.f });
		shader->setVector3f("lightPos", getGlobalTransform() ^ mk::math::Vector3f(0.5f));
	}

	void onDraw(mk::RenderTarget& target, mk::DrawContext context, const mk::Game& game)
		const override {
		context.shader = light_shader;
		mk::CubeShape::onDraw(target, context, game);
	}

	const mk::Shader* light_shader = nullptr;
	const mk::Shader* shader       = nullptr;
};

class LightScene: public mk::WorldEntity3D {
public:
	void onReady(mk::Game& game) override {
		// Camera
		cam = addChild<mk::Camera3D>(game);
		cam->setPosition(mk::math::Vector3f{ 5.f });
		cam->lookAt({ 0.f });

		// Objects
		cube         = addChild<Cube>(game);
		cube->shader = &shader;
		// cube->setScale({ 0.6, 0.9, 0.4 });

		light               = addChild<LightSource>(game);
		light->light_shader = &light_shader;
		light->shader       = &shader;
		light->setScale({ 0.2f });

		// Shaders
		setupShaders(game);
	}

	void setupShaders(mk::Game& game) {
		// light properties
		shader.setVector3f("light.ambient", { 0.2f, 0.2f, 0.2f });
		shader.setVector3f("light.diffuse", { 0.5f, 0.5f, 0.5f });
		shader.setVector3f("light.specular", { 1.0f, 1.0f, 1.0f });

		shader.setI32("material.diffuse", 1);
		mk::Texture::bindAt(game.resources().getTexture("container2.png"), 1);

		shader.setI32("material.specular", 2);
		mk::Texture::bindAt(game.resources().getTexture("container2_specular.png"), 2);

		shader.setFloat("material.shininess", 32.0f);
	}

	void onUpdate(mk::Game&, float) override { shader.setVector3f("camPos", cam->getPosition()); }

	void onEvent(mk::Game& game, const mk::Event& event) override {
		if (auto ev = event.get<mk::Event::KeyPressed>(); ev) {
			if (ev->key == mk::input::KEY::R)
				if (!shader.tryLoad(mk::ResPath("vert.glsl"), mk::ResPath("frag.glsl")))
					std::cerr << "Shader recompilation has failed...\n";
				else
					setupShaders(game);
			else if (ev->key == mk::input::KEY::SPACE)
				setPaused(!isPaused());
		}
	}

	Cube* cube = nullptr;

	mk::Camera3D* cam   = nullptr;
	LightSource*  light = nullptr;

	mk::Shader shader       = { mk::ResPath("vert.glsl"), mk::ResPath("frag.glsl") };
	mk::Shader light_shader = { mk::ResPath("light_vert.glsl"), mk::ResPath("light_frag.glsl") };

	const mk::Texture* cube_texture;
};

int main() {
	mk::Game game("settings.json");
	game.addScene<LightScene>();
	game.run();
}
