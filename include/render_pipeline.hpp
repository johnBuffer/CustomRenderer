#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include <iostream>

using RenderAccessor = std::function<sf::RenderTexture&(uint32_t id)>;

// A stage in the pipeline
class RenderStage
{
public:
	using Ptr = std::shared_ptr<RenderStage>;

	RenderStage(uint32_t id1, uint32_t id2) :
		m_id1(id1),
		m_id2(id2)
	{}

	void exec(RenderAccessor accessor) const
	{
		process(accessor(m_id1), accessor(m_id2));
	}

	virtual void process(sf::RenderTexture&, sf::RenderTexture&) const {};

private:
	uint32_t m_id1;
	uint32_t m_id2;
};


// An array on stages that will be applied sequentialy
class PipeLine
{
public:
	PipeLine() = default;

	PipeLine(const RenderAccessor& accessor) :
		m_accessor(accessor)
	{}

	void setAccessor(RenderAccessor accessor)
	{
		m_accessor = accessor;
	}

	void addStage(const RenderStage::Ptr stage)
	{
		m_stages.push_back(stage);
	}

	const void execute()
	{
		for (const RenderStage::Ptr stage : m_stages) {
			stage->exec(m_accessor);
		} 
	}

private:
	RenderAccessor m_accessor;
	std::vector<RenderStage::Ptr> m_stages;
};

