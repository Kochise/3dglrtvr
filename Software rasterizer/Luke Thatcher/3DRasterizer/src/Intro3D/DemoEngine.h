
#pragma once

#include "AppEngine.h"
#include "vector.h"
#include "LightStructs.h"
#include "Material.h"
#include <sstream>

enum FilteringType
{
	Point,
	Bilinear
};

#define NUM_TESTS 18
#define MAX_TEST_TIME 15.0

class DemoEngine : public AppEngine
{
public:
	DemoEngine(HWND hWnd, const InputManager& inputManager, UINT numWorkerThreads);
	virtual ~DemoEngine(void);
	bool LoadFailed(void) const;

protected:
	void Update(double totalTime, double elapsedTime);
	void Draw(double totalTime, double elapsedTime);
	void DrawHUD(Gdiplus::Graphics& graphics);

private:
	// Drawing Helper Method
	void DrawMarvinAndPlane(const Matrix4x4& marvinWorld, const Matrix4x4& planeWorld, VertexShaderCallback vertexShader, PixelShaderCallback pixelShader, bool usePerVertexNormals = true, int numVector4Slots = 0);
	
	DepthBuffer* shadowMapDepthBuffer;

	// Models
	Model* marvin;
	Model* plane;
	Model* sphere;

	// Textures
	Texture2D* marvinTex;
	Texture2D* checkerBlue;

	// Object Materials
	Material marvinMaterial;
	Material planeMaterial;
	Material sphereMaterial;

	// Demo State
	double fps;
	double frameTimer;
	int frames;
	bool loadFailed;
	bool useFreeCamera;
	Camera camera;
	FilteringType filterType;
	const InputManager& inputManager;
	UINT testIndex;
	double testTimer;
	bool testTimerEnabled;

	// Current lights
	vector<SpotLight, AlignAlloc<SpotLight, 16>> spotLights;
	vector<PointLight, AlignAlloc<PointLight, 16>> pointLights;
	vector<DirectionalLight, AlignAlloc<DirectionalLight, 16>> directionalLights;

	// Number of lights
	UINT numSpotLights;
	UINT numPointLights;
	UINT numDirectionalLights;

	// Vertex and Pixel Shader parameters
	Matrix4x4 world;
	Matrix4x4 view;
	Matrix4x4 projection;
	Matrix4x4 wvp;

	Matrix4x4 lightWVP;

	Vector3 camPos;
	Vector3 camLookAt;
	Material* currentMaterial;
	Vector4 ambientLight;

	// Vertex Shaders
	void VS_Simple(const Vertex& input, VS_Output& output) const;
	void VS_GouraudDiffuse(const Vertex& input, VS_Output& output) const;
	void VS_SimpleTextured(const Vertex& input, VS_Output& output) const;
	void VS_GouraudDiffuseTextured(const Vertex& input, VS_Output& output) const;
	void VS_PhongTexturedCommon(const Vertex& input, VS_Output& output) const;
	void VS_PhongShadowedSpotLight(const Vertex& input, VS_Output& output) const;

	// Pixel Shaders
	void PS_White(const VS_Output& input, Vector4& output) const;
	void PS_Slot0Color(const VS_Output& input, Vector4& output) const;
	void PS_SimpleTexturedPoint(const VS_Output& input, Vector4& output) const;
	void PS_SimpleTexturedBilinear(const VS_Output& input, Vector4& output) const;
	void PS_PhongCommon(const VS_Output& input, Vector4& output) const;
	void PS_PhongShadowedSpotLight(const VS_Output& input, Vector4& output) const;
};