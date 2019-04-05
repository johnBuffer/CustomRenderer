#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>

using StageProcess = std::function<void(sf::RenderTexture&, sf::RenderTexture&)>;
using RenderAccessor = std::function<sf::RenderTexture&(uint32_t id)>;

// A stage in the pipeline
class RenderStage
{
public:
	RenderStage(StageProcess process, uint32_t id1, uint32_t id2) :
		m_process(process),
		m_id1(id1),
		m_id2(id2)
	{}

	void applyProcess(RenderAccessor accessor) const
	{
		m_process(accessor(m_id1), accessor(m_id2));
	}

	virtual void process(sf::RenderTexture& texture1, sf::RenderTexture& texture2) 
	{}

private:
	uint32_t m_id1;
	uint32_t m_id2;
	StageProcess m_process;
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

	void addStage(const RenderStage& stage)
	{
		m_stages.push_back(stage);
	}

	const sf::Sprite execute()
	{
		for (const RenderStage& stage : m_stages)
		{
			stage.process(m_accessor);
		}
	}

private:
	RenderAccessor m_accessor;
	std::vector<RenderStage> m_stages;
};

