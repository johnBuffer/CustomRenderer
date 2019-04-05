#include <SFML/Graphics.hpp>
#include "Renderer.hpp"
#include "dynamic_blur.hpp"

struct Bloom
{
	Bloom(uint32_t width, uint32_t height) :
		blur(width, height)
	{}

	void apply(sf::RenderTexture& bloom_texure, sf::RenderTexture& final_render)
	{
		bloom_texure.display();
		final_render.draw(sf::Sprite(blur.apply(bloom_texure.getTexture(), 3)), sf::BlendAdd);
	}

	Blur blur;
};

int main()
{
	constexpr uint32_t win_width(1600);
	constexpr uint32_t win_height(900);

	sf::RenderWindow window(sf::VideoMode(win_width, win_height), "RenderTest");

	// Create renderer
	Renderer renderer(win_width, win_height);
	renderer.setRenderScale(0.5f);

	// Add bloom layer
	Bloom bloom(win_width, win_height);
	const ID bloom_texture(renderer.addLayer());
	RenderStage bloom_stage([&](sf::RenderTexture& t1, sf::RenderTexture& t2) { bloom.apply(t1, t2); }, bloom_texture, Renderer::FinalTexture);

	renderer.getPipeline().addStage(bloom_stage);

	// Draw
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		renderer.clear();

		renderer.draw(shape, bloom_texture);
		renderer.draw(shape, Renderer::FinalTexture);

		window.draw(renderer.render());
		window.display();
	}

	return 0;
}

