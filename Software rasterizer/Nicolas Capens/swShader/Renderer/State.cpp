#include "State.hpp"

#include "Error.hpp"

#include <string.h>
#include <malloc.h>

namespace swShader
{
	State::State()
	{
		shaderFile = 0;
		pipelineState = 0;
		for(int i = 0; i < 16; i++) samplerState[i] = 0;
		stamp = 0;
	}

	State::State(const State &state)
	{
		shaderFile = strdup(state.shaderFile);
		pipelineState = state.pipelineState;
		for(int i = 0; i < 16; i++) samplerState[i] = state.samplerState[i];
		updateStamp();
	}

	State &State::operator=(const State &state)
	{
		free(shaderFile);
		shaderFile = strdup(state.shaderFile);
		pipelineState = state.pipelineState;
		for(int i = 0; i < 16; i++) samplerState[i] = state.samplerState[i];
		updateStamp();

		return *this;
	}

	State::~State()
	{
		free(shaderFile);
		shaderFile = 0;
	}

	bool State::operator==(const State &state) const
	{
		if(stamp != state.stamp) return false;
		if(!state.shaderFile) return false;
		if(strcmp(shaderFile, state.shaderFile) != 0) return false;
		if(pipelineState != state.pipelineState) return false;

		for(int i = 0; i < 16; i++)
		{
			if(samplerState[i] != state.samplerState[i]) return false;
		}

		return true;
	}

	bool State::operator!=(const State &state) const
	{
		return !(*this == state);
	}

	void State::setShaderFile(const char *shaderFile)
	{
		free(this->shaderFile);
		this->shaderFile = strdup(shaderFile);
	}

	void State::setPipelineState(int state)
	{
		if(pipelineState != state)
		{
			pipelineState = state;
			updateStamp();
		}
	}

	void State::setSamplerState(int stage, int state)
	{
		if(stage < 0 || stage >= 16) throw Error("Sampler index out of range");

		if(samplerState[stage] != state)
		{
			samplerState[stage] = state;
			updateStamp();
		}
	}

	void State::updateStamp()
	{
	//	stamp = pipelineState;
	//	for(int i = 0; i < 16; i++) stamp += samplerState[i];
	}
}