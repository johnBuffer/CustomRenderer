#include <SFML/Graphics.hpp>
#include "custom_renderer.hpp"
#include "dynamic_blur.hpp"
#include <iostream>

class BloomStage : public RenderStage
{
public:
	using BloomStagePtr = std::shared_ptr<RenderStage>;

	BloomStage(ID tex1, ID tex2, uint32_t width, uint32_t height) :
		RenderStage(tex1, tex2),
		m_blur(width, height, 0.5f)
	{
	}

	void process(sf::RenderTexture& bloom_texure, sf::RenderTexture& final_render) const override
	{
		bloom_texure.display();
		final_render.draw(m_blur.apply(bloom_texure.getTexture(), 4), sf::BlendAdd);
	}

	static BloomStagePtr create(ID tex1, ID tex2, uint32_t width, uint32_t height)
	{
		return std::make_shared<BloomStage>(tex1, tex2, width, height);
	}

private:
	mutable Blur m_blur;
};

int main()
{
	constexpr uint32_t win_width(1600);
	constexpr uint32_t win_height(900);

	sf::RenderWindow window(sf::VideoMode(win_width, win_height), "Custom Renderer", sf::Style::Default);

	// Create renderer
	Renderer renderer(win_width, win_height);
	renderer.setRenderScale(1.0f);

	// Add bloom layer
	const ID bloom_layer(renderer.addLayer());
	renderer.getPipeline().addStage(BloomStage::create(bloom_layer, Renderer::FinalTexture, win_width, win_height));

	// Draw
	const float r(100.0f);
	sf::CircleShape shape(r);
	shape.setFillColor(sf::Color::Green);
	shape.setOrigin(r, r);

	float time(0.0f);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		time += 0.0002f;
		shape.setPosition(800.0f * cos(time), 400.0f * sin(time));

		renderer.clear();

		renderer.draw(shape, { Renderer::FinalTexture, bloom_layer });
		renderer.render_in(window);

		window.display();
	}

	return 0;
}

