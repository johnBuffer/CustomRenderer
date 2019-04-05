#include <SFML/Graphics.hpp>
#include "Renderer.hpp"

int main()
{
	constexpr uint32_t win_width(1600);
	constexpr uint32_t win_height(900);

	sf::RenderWindow window(sf::VideoMode(win_width, win_height), "RenderTest");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	Renderer renderer(win_width, win_height);
	renderer.setRenderScale(0.5f);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		renderer.draw(shape);

		window.clear();
		window.draw(renderer.render());
		window.display();
	}

	return 0;
}