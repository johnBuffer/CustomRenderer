#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <memory>
#include <iostream>
#include "render_pipeline.hpp"
#include "layer.hpp"

using LayerID = uint32_t;


struct Drawable : sf::Drawable
{
	std::vector<std::string> layers;
};



class Renderer
{
public:
	Renderer(uint32_t render_width, uint32_t render_height, float render_scale = 1.0f) :
		m_pipeline([&](const std::string& name) -> sf::RenderTexture& {return m_layers[name].getTexture(); }),
		m_render_size(render_width, render_height),
		m_render_scale(render_scale)
	{
		addLayer("final");
	}

	PipeLine& getPipeline()
	{
		return m_pipeline;
	}

	void setRenderScale(float scale)
	{
		m_render_scale = scale;
	}

	LayerID addLayer(const std::string& name, bool clear = true)
	{
		LayerInfo info(m_render_size.x, m_render_size.y, clear);
		m_layers[name] = RenderLayer(info);

		return LayerID(m_layers.size() - 1);
	}

	void clear()
	{
		for (auto& layer : m_layers) {
			layer.second.clear();
		}
	}

	void draw(Drawable& drawable)
	{
		for (const std::string& name : drawable.layers) {
			drawInLayer(drawable, name);
		}
	}

	void setFocus(const sf::Vector2f& coord)
	{
		m_focus = coord;
	}

	const sf::Sprite render()
	{
		m_pipeline.execute();
		RenderLayer& final_layer = m_layers["final"];
		final_layer.getTexture().display();

		sf::Sprite result(final_layer.getTexture().getTexture());
		result.setScale(1.0f / m_render_scale, 1.0f / m_render_scale);

		return result;
	}

	void render_in(sf::RenderTarget& target)
	{
		target.draw(render());
		clear();
	}

private:
	float m_zoom;
	sf::Vector2f m_focus;

	PipeLine m_pipeline;

	float m_render_scale;
	sf::Vector2u m_render_size;
	std::map<const std::string, RenderLayer> m_layers;

	void drawInLayer(const sf::Drawable& drawable, const std::string& layer_name)
	{
		RenderLayer& layer(m_layers[layer_name]);
		sf::RenderStates rs;
		rs.transform.translate(-m_focus);
		rs.transform.translate(0.5f * m_render_size.x * m_render_scale, 0.5f * m_render_size.y * m_render_scale);
		rs.transform.scale(m_render_scale, m_render_scale);
		layer.getTexture().draw(drawable, rs);
	}
};
