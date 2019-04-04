#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

using ID = uint32_t;

struct RenderLayer
{
	ID identifier;
	sf::RenderTexture render_texture;
};

class Renderer
{
public:
	Renderer(uint32_t render_width, uint32_t render_height) :
		m_render_size(render_width, render_height)
	{
		addLayer();
	}

	ID addLayer()
	{
		m_layers.emplace_back();
		m_layers.back().render_texture.create(m_render_size.x, m_render_size.y);

		return m_layers.size() - 1;
	}

	void draw(sf::Drawable& drawable, ID layer_id = 0)
	{
		RenderLayer& layer(m_layers[layer_id]);

		layer.render_texture.draw(drawable);
	}

	const sf::Texture& render()
	{

	}

private:

	sf::Vector2u m_render_size;
	std::vector<RenderLayer> m_layers;
};
