#ifndef State_hpp
#define State_hpp

namespace swShader
{
	class State
	{
	public:
		State();
		State(const State &state);

		State &operator=(const State &state);

		~State();

		void setShaderFile(const char *shaderFile);
		void setPipelineState(int state);
		void setSamplerState(int stage, int state);

		bool operator==(const State &state) const;
		bool operator!=(const State &state) const;

	private:
		void updateStamp();

		char *shaderFile;
		int pipelineState;
		int samplerState[16];
		int stamp;
	};
}

#endif   // State_hpp