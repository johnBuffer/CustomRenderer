#include <SFML/Graphics.hpp>
#include "Renderer.hpp"
#include "dynamic_blur.hpp"

class BloomStage : public RenderStage
{
public:
	using BloomStagePtr = std::shared_ptr<RenderStage>;

	BloomStage(ID tex1, ID tex2, uint32_t width, uint32_t height) :
		RenderStage(tex1, tex2),
		m_blur(width, height)
	{}

	void process(sf::RenderTexture& bloom_texure, sf::RenderTexture& final_render) const
	{
		bloom_texure.display();
		final_render.draw(sf::Sprite(m_blur.apply(bloom_texure.getTexture(), 4)), sf::BlendAdd);
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
	constexpr uint32_t win_width(800);
	constexpr uint32_t win_height(450);

	sf::RenderWindow window(sf::VideoMode(win_width, win_height), "Custom Renderer", sf::Style::Default);

	// Create renderer
	Renderer renderer(win_width, win_height);

	// Add bloom layer
	const ID bloom_texture(renderer.addLayer());
	renderer.getPipeline().addStage(BloomStage::create(bloom_texture, Renderer::FinalTexture, win_width, win_height));

	// Draw
	const float r(100.0f);
	sf::CircleShape shape(r);
	shape.setFillColor(sf::Color::Green);
	shape.setOrigin(r, r);

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

