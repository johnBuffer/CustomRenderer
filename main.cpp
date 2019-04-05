#include <SFML/Graphics.hpp>
#include "Renderer.hpp"
#include "dynamic_blur.hpp"

class BloomStage : public RenderStage
{
public:
	BloomStage(ID tex1, ID tex2, uint32_t width, uint32_t height) :
		RenderStage(tex1, tex2),
		m_blur(width, height)
	{}

	void process(sf::RenderTexture& bloom_texure, sf::RenderTexture& final_render) const
	{
		bloom_texure.display();
		final_render.draw(sf::Sprite(m_blur.apply(bloom_texure.getTexture(), 4)), sf::BlendAdd);
	}

private:
	mutable Blur m_blur;
};

using BloomStagePtr = std::shared_ptr<RenderStage>;

int main()
{
	constexpr uint32_t win_width(1600);
	constexpr uint32_t win_height(900);

	sf::RenderWindow window(sf::VideoMode(win_width, win_height), "RenderTest");

	// Create renderer
	Renderer renderer(win_width, win_height);
	renderer.setRenderScale(0.5f);

	// Add bloom layer
	const ID bloom_texture(renderer.addLayer());
	BloomStagePtr bloom_stage(std::make_shared<BloomStage>(bloom_texture, Renderer::FinalTexture, win_width, win_height));
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

