#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include "render_pipeline.hpp"
#include "layer.hpp"

using LayerID = uint32_t;


class Renderer
{
public:
	enum
	{
		FinalTexture = 0
	};

	Renderer(uint32_t render_width, uint32_t render_height, float render_scale = 1.0f) :
		m_pipeline([&](uint32_t id) -> sf::RenderTexture& {return m_layers[id].render_texture; }),
		m_render_size(render_width, render_height),
		m_render_scale(render_scale)
	{
		addLayer();
	}

	PipeLine& getPipeline()
	{
		return m_pipeline;
	}

	void setRenderScale(float scale)
	{
		m_render_scale = scale;
	}

	LayerID addLayer(bool clear = true)
	{
		LayerInfo info(m_render_size.x, m_render_size.y, clear);
		m_layers.emplace_back(info);

		return LayerID(m_layers.size() - 1);
	}

	void clear()
	{
		for (RenderLayer& layer : m_layers) {
			layer.clear();
		}
	}

	void draw(const sf::Drawable& drawable, LayerID layer_id = 0)
	{
		RenderLayer& layer(m_layers[layer_id]);

		sf::RenderStates rs;
		rs.transform.translate(-m_focus);
		rs.transform.translate(0.5f * m_render_size.x * m_render_scale, 0.5f * m_render_size.y * m_render_scale);

		rs.transform.scale(m_render_scale, m_render_scale);

		layer.render_texture.draw(drawable, rs);
	}

	void draw(const sf::Drawable& drawable, std::vector<LayerID> layers)
	{
		for (LayerID layer_id : layers) {
			draw(drawable, layer_id);
		}
	}

	const sf::Sprite render()
	{
		m_pipeline.execute();

		m_layers.front().render_texture.display();
		sf::Sprite result(m_layers.front().render_texture.getTexture());
		result.setScale(1.0f / m_render_scale, 1.0f / m_render_scale);

		return result;
	}

	void render_in(sf::RenderTarget& target)
	{
		target.draw(render());
	}

private:
	float m_zoom;
	sf::Vector2f m_focus;

	PipeLine m_pipeline;

	float m_render_scale;
	sf::Vector2u m_render_size;
	std::vector<RenderLayer> m_layers;
};
