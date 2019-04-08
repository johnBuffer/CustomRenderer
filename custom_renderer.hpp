#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include "render_pipeline.hpp"

using ID = uint32_t;

struct RenderLayer
{
	RenderLayer() :
		m_render_texture_ptr(std::make_unique<sf::RenderTexture>()),
		render_texture(*m_render_texture_ptr)
	{
		std::cout << "New RenderLayer created" << std::endl;
	}

private:
	std::unique_ptr<sf::RenderTexture> m_render_texture_ptr;

public:
	sf::RenderTexture& render_texture;
	float render_scale;
};

class Renderer
{
public:
	enum
	{
		FinalTexture = 0
	};

	Renderer(uint32_t render_width, uint32_t render_height) :
		m_pipeline([&](uint32_t id) -> sf::RenderTexture& {return m_layers[id].render_texture; }),
		m_render_size(render_width, render_height),
		m_render_scale(1.0f)
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

	ID addLayer()
	{
		m_layers.emplace_back();
		m_layers.back().render_texture.create(m_render_size.x, m_render_size.y);

		return ID(m_layers.size() - 1);
	}

	void clear()
	{
		for (RenderLayer& layer : m_layers)
		{
			layer.render_texture.clear();
		}
	}

	void draw(const sf::Drawable& drawable, ID layer_id = 0)
	{
		RenderLayer& layer(m_layers[layer_id]);

		sf::RenderStates rs;
		rs.transform.translate(-m_focus);
		rs.transform.translate(0.5f * m_render_size.x * m_render_scale, 0.5f * m_render_size.y * m_render_scale);

		rs.transform.scale(m_render_scale, m_render_scale);

		layer.render_texture.draw(drawable, rs);
	}

	const sf::Sprite render()
	{
		m_pipeline.execute();

		m_layers.front().render_texture.display();
		sf::Sprite result(m_layers.front().render_texture.getTexture());
		result.setScale(1.0f / m_render_scale, 1.0f / m_render_scale);

		return result;
	}

private:
	float m_zoom;
	sf::Vector2f m_focus;

	PipeLine m_pipeline;

	float m_render_scale;
	sf::Vector2u m_render_size;
	std::vector<RenderLayer> m_layers;
};
