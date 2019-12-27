#pragma once

#include <memory>
#include <SFML/Graphics.hpp>


struct LayerInfo
{
	LayerInfo()
		: width(0U)
		, height(0U)
		, clear(true)
	{}

	LayerInfo(uint32_t width_, uint32_t height_, bool clear_)
		: width(width_)
		, height(height_)
		, clear(clear_)
	{}
	
	uint32_t width;
	uint32_t height;
	bool clear;
};


struct RenderLayer
{
	using Ptr = std::shared_ptr<RenderLayer>;

	RenderLayer(const LayerInfo& layer_info)
		: m_render_texture_ptr(std::make_unique<sf::RenderTexture>())
		, render_texture(*m_render_texture_ptr)
		, m_info(layer_info)
	{
		render_texture.create(m_info.width, m_info.height);
	}

	void clear() {
		if (m_info.clear) {
			render_texture.clear();
		}
	}

private:
	std::unique_ptr<sf::RenderTexture> m_render_texture_ptr;
	LayerInfo m_info;

public:
	sf::RenderTexture& render_texture;
	float render_scale;
};