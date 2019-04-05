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

	const StageProcess& getProcess() const
	{
		return m_process;
	}

	const uint32_t getID1() const
	{
		return m_id1;
	}

	const uint32_t getID2() const
	{
		return m_id2;
	}

private:
	uint32_t m_id1;
	uint32_t m_id2;
	StageProcess m_process;
};

// An array on stages that will be applied sequentialy
class PipeLine
{
public:
	PipeLine();

	const sf::Sprite execute()
	{
		for (const RenderStage& stage : m_stages)
		{
			const uint32_t id1(stage.getID1());
			const uint32_t id2(stage.getID2());
			const StageProcess& process(stage.getProcess());


		}
	}

private:
	uint32_t m_final_texture_id;
	RenderAccessor m_accessor;
	std::vector<RenderStage> m_stages;
	
};
