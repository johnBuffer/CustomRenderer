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
		: m_render_texture(std::make_unique<sf::RenderTexture>())
		, m_info(layer_info)
	{
		m_render_texture->create(m_info.width, m_info.height);
	}

	void clear() {
		if (m_info.clear) {
			m_render_texture->clear();
		}
	}

	sf::RenderTexture& getTexture()
	{
		return *m_render_texture;
	}

private:
	std::shared_ptr<sf::RenderTexture> m_render_texture;
	LayerInfo m_info;

public:
	float render_scale;
};