
#include "stdafx.h"
#include "DemoEngine.h"
#include "MD2Loader.h"
#include <sstream>
#include <iomanip>

#define _USE_MATH_DEFINES
#include <math.h>

DemoEngine::DemoEngine(HWND hWnd, const InputManager& inputManager, UINT numWorkerThreads)
	: AppEngine(hWnd, numWorkerThreads), inputManager(inputManager)
{
	this->filterType = Point;
	this->frameTimer = 0.0;
	this->frames = 0;
	this->fps = 0.0;
	this->testIndex = 0;
	this->testTimer = 0;
	this->numSpotLights = this->numPointLights = this->numDirectionalLights = 0;
	this->useFreeCamera = false;
	this->marvin = NULL;
	this->plane = NULL;
	this->sphere = NULL;
	this->marvinTex = NULL;
	this->checkerBlue = NULL;
	this->testTimerEnabled = true;

	this->loadFailed = false;

	// Load Models
	Md2Header header;
	if (!MD2Loader::LoadModel("marvin.md2", 0, &marvin, &header))
	{
		MessageBox(hWnd, L"Error Loading Model File \"marvin.md2\". Ensure this file can be found within the application's working directory.", L"Model Error", MB_OK);
		this->loadFailed = true;
		return;
	}
	if (!Texture2D::FromPCX("marvin.pcx", &header, &marvinTex))
	{
		MessageBox(hWnd, L"Error Loading Texture File \"marvin.pcx\". Ensure this file can be found within the application's working directory.", L"Texture Error", MB_OK);
		this->loadFailed = true;
		return;
	}
	if (!Texture2D::FromBitmap("checkerBlue.bmp", &checkerBlue))
	{
		MessageBox(hWnd, L"Error Loading Texture File \"checkerBlue.bmp\". Ensure this file can be found within the application's working directory.", L"Texture Error", MB_OK);
		this->loadFailed = true;
		return;
	}

	plane = Model::CreatePlane(10, 10, 10, 10, Vector2(10, 10));
	sphere = Model::CreateSphere(32, 16, 1.0f);

	this->marvinMaterial = Material(
		Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		55.0f,
		marvinTex);
	this->planeMaterial = Material(
		Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		55.0f,
		checkerBlue);
	this->sphereMaterial = Material(
		Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		70.0f,
		checkerBlue);

	this->shadowMapDepthBuffer = new DepthBuffer(512, 512);
}
DemoEngine::~DemoEngine(void)
{
	if (marvin)
	{
		delete marvin;
		marvin = NULL;
	}
	if (plane)
	{
		delete plane;
		plane = NULL;
	}
	if (sphere)
	{
		delete sphere;
		sphere = NULL;
	}
	if (marvinTex)
	{
		delete marvinTex;
		marvinTex = NULL;
	}
	if (checkerBlue)
	{
		delete checkerBlue;
		checkerBlue = NULL;
	}
	if (shadowMapDepthBuffer)
	{
		delete shadowMapDepthBuffer;
		shadowMapDepthBuffer = NULL;
	}
}
bool DemoEngine::LoadFailed(void) const
{
	return this->loadFailed;
}

void DemoEngine::Update(double totalTime, double elapsedTime)
{
	projection = Matrix4x4::CreatePerspectiveFieldOfView(
		(float)M_PI / 4.0f,
		((float)this->backBufferTarget->GetWidth()) / ((float)this->backBufferTarget->GetHeight()),
		1.0f,
		500.0f);

	if (inputManager.HasKeyPressed(0x50) || inputManager.HasButtonPressed(XINPUT_GAMEPAD_START)) // P
	{
		testTimerEnabled = !testTimerEnabled;
	}
	if (inputManager.HasKeyPressed(0x25) || inputManager.HasButtonPressed(XINPUT_GAMEPAD_DPAD_LEFT)) // Left Arrow
	{
		if (testIndex == 0)
			testIndex = NUM_TESTS - 1;
		else
			testIndex--;

		testTimer = 0;
	}
	if (inputManager.HasKeyPressed(0x27) || inputManager.HasButtonPressed(XINPUT_GAMEPAD_DPAD_RIGHT)) // Right Arrow
	{
		testIndex++;
		testIndex %= NUM_TESTS;
		testTimer = 0;
	}

	if (testTimerEnabled)
	{
		testTimer += elapsedTime;
		if (testTimer >= MAX_TEST_TIME)
		{
			testTimer -= MAX_TEST_TIME;
			testIndex++;
			testIndex %= NUM_TESTS;
		}
	}


	// Update camera settings
	switch (testIndex)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 9:
	case 10:
	case 13:
	case 14:
		camPos =
			Matrix4x4::CreateRotationY(((float)totalTime) * 0.25f) *
			Matrix4x4::CreateTranslation(Vector3(0.0f, 40.0f, 0.0f)) *
			Vector3(0.0f, 0.0f, 80.0f);
		camLookAt = Vector3(0.0f, 0.0f, 0.0f);
		break;
	case 7:
		camPos = Vector3(0.0f, 10.0f, 70.0f);
		camLookAt = Vector3(0.0f, -5.0f, 0.0f);
		break;
	case 8:
	case 11:
	case 12:
	case 15:
	case 16:
	case 17:
		camPos =
			Matrix4x4::CreateRotationY(1.0f) *
			Matrix4x4::CreateTranslation(Vector3(0.0f, 40.0f, 0.0f)) *
			Vector3(0.0f, 0.0f, 80.0f);
		camLookAt = Vector3(0.0f, 0.0f, 0.0f);
		break;
	}

	if (useFreeCamera)
	{
		camera.Update(this->inputManager, (float)elapsedTime);
		view = camera.GetView();
		camPos = camera.GetPosition();
		camLookAt = camPos + camera.GetForward();
	}
	else
	{
		view = Matrix4x4::CreateLookAt(camPos, camLookAt, Vector3(0.0f, 1.0f, 0.0f));
	}
	if (inputManager.HasKeyPressed(0x43) || inputManager.HasButtonPressed(XINPUT_GAMEPAD_Y)) // C
	{
		if (useFreeCamera)
		{
			useFreeCamera = false;
		}
		else
		{
			useFreeCamera = true;
			camera.SetPosition(camPos);
			camera.SetRotation(Matrix4x4::GetRotationFromMatrix(view));
		}
	}

	// Calculate frames per second
	frames++;
	frameTimer += elapsedTime;
	if (frameTimer >= 1.0)
	{
		fps = (double)frames / frameTimer;
		frameTimer = 0;
		frames = 0;
	}
}
void DemoEngine::Draw(double totalTime, double elapsedTime)
{
	// Clear the back buffer to black
	backBufferTarget->Clear(0x00000000);

	// Clear the depth buffer to 1.0 (maximum depth)
	backBufferDepth->Clear(1.0f);

	// Set the targets to the rasterizer
	if (testIndex != 16)
	{
		// Skip this for testIndex 16, as this test renders to a shadow map first
		rasterizer->SetTargets(backBufferTarget, backBufferDepth);
	}

	numSpotLights = 0;
	numPointLights = 0;
	numDirectionalLights = 0;
	ambientLight = Vector4();

	directionalLights.clear();
	spotLights.clear();
	pointLights.clear();

	Matrix4x4 marvinWorld = Matrix4x4::CreateTranslation(0, 24, 0) * Matrix4x4::CreateScale(0.5f);
	Matrix4x4 planeWorld;

	switch(testIndex)
	{
	case 0: // White Wireframe, no culling
		this->rasterizer->SetFillMode(Wireframe);
		this->rasterizer->SetCullMode(CullNone);
		this->filterType = Point;
		DrawMarvinAndPlane(marvinWorld, planeWorld, &DemoEngine::VS_Simple, &DemoEngine::PS_White);
		break;

	case 1: // White Wireframe, with culling
		this->rasterizer->SetFillMode(Wireframe);
		this->rasterizer->SetCullMode(CullCounterClockwise);
		this->filterType = Point;
		DrawMarvinAndPlane(marvinWorld, planeWorld, &DemoEngine::VS_Simple, &DemoEngine::PS_White);
		break;

	case 2: // Solid white fill
		this->rasterizer->SetFillMode(Solid);
		this->rasterizer->SetCullMode(CullCounterClockwise);
		this->filterType = Point;
		DrawMarvinAndPlane(marvinWorld, planeWorld, &DemoEngine::VS_Simple, &DemoEngine::PS_White);
		break;

	case 3: // Flat shading, 1 directional light (use per face normals)
	case 4: // Gouraud Diffuse Only, 1 directional light (use per vertex normals)
		this->rasterizer->SetFillMode(Solid);
		this->rasterizer->SetCullMode(CullCounterClockwise);
		this->filterType = Point;

		// Setup the directional light
		directionalLights.clear();
		directionalLights.push_back(DirectionalLight(
			Vector3::Normalize(Vector3(-5, -5, -5)),
			Vector4(1.0f, 0.5f, 0.0f, 1.0f),
			Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
		numDirectionalLights = 1;
		
		DrawMarvinAndPlane(marvinWorld, planeWorld, &DemoEngine::VS_GouraudDiffuse, &DemoEngine::PS_Slot0Color, testIndex != 3, 1);
		break;

	case 5: // Simple Texturing (Point)
		this->rasterizer->SetFillMode(Solid);
		this->rasterizer->SetCullMode(CullCounterClockwise);
		this->filterType = Point;
		DrawMarvinAndPlane(marvinWorld, planeWorld, &DemoEngine::VS_SimpleTextured, &DemoEngine::PS_SimpleTexturedPoint, true, 1);
		break;

	case 6: // Simple Texturing (Bilinear)
		this->rasterizer->SetFillMode(Solid);
		this->rasterizer->SetCullMode(CullCounterClockwise);
		this->filterType = Bilinear;
		DrawMarvinAndPlane(marvinWorld, planeWorld, &DemoEngine::VS_SimpleTextured, &DemoEngine::PS_SimpleTexturedBilinear, true, 1);
		break;

	case 7: // Texture Sampling Comparison
		{
			this->rasterizer->SetFillMode(Solid);
			this->rasterizer->SetCullMode(CullCounterClockwise);
			this->filterType = Bilinear;
		
			const vector<Vertex, AlignAlloc<Vertex, 16>> marvinVerts = marvin->GetFrames()[0].GetVertices();
			const vector<Face, AlignAlloc<Face, 16>> marvinFaces = marvin->GetFrames()[0].GetFaces();

			currentMaterial = &marvinMaterial;
			world = Matrix4x4::CreateRotationY(((float)totalTime) * 0.5f) * Matrix4x4::CreateTranslation(-20.0f, 0.0f, 0.0f);
			wvp = world * view * projection;
			DrawModel(true, marvinVerts, marvinFaces, true, 1, &DemoEngine::VS_SimpleTextured, &DemoEngine::PS_SimpleTexturedPoint);

			world = Matrix4x4::CreateRotationY(((float)totalTime) * 0.5f) * Matrix4x4::CreateTranslation(20.0f, 0.0f, 0.0f);
			wvp = world * view * projection;
			DrawModel(true, marvinVerts, marvinFaces, true, 1, &DemoEngine::VS_SimpleTextured, &DemoEngine::PS_SimpleTexturedBilinear);
		}
		break;

	case 8: // Single Orbiting Point Light
	case 9: // 5 Phong Point Lights
	case 10: // 5 Phong Point Lights - Lit Wireframe
		{
			this->rasterizer->SetFillMode(testIndex == 10 ? Wireframe : Solid);
			this->rasterizer->SetCullMode(CullCounterClockwise);
			this->filterType = Bilinear;

			numPointLights = testIndex == 8 ? 1 : 5;

			Matrix4x4 lightRot = Matrix4x4::CreateRotationY((float)totalTime);

			pointLights.push_back(PointLight(
				lightRot * Vector3(0.0f, 2.5f, 15.0f),
				Vector4(2.0f, 2.0f, 2.0f, 1.0f),
				Vector4(1.0f, 1.0f, 1.0f, 1.0f),
				0.1f,
				0.05f,
				0.0f));

			if (testIndex != 8)
			{
				float offset = (float)sin(totalTime) * 10.0f + 20.0f;

				pointLights.push_back(PointLight(
				Vector3(offset, 10.0f, offset),
				Vector4(5.0f, 0.0f, 0.0f, 1.0f),
				Vector4(1.0f, 1.0f, 1.0f, 1.0f),
				0.0f,
				0.05f,
				0.0f));

			pointLights.push_back(PointLight(
				Vector3(-offset, 10.0f, -offset),
				Vector4(2.0f, 2.0f, 0.0f, 1.0f),
				Vector4(1.0f, 1.0f, 1.0f, 1.0f),
				0.0f,
				0.05f,
				0.0f));

			pointLights.push_back(PointLight(
				Vector3(-offset, 10.0f, offset),
				Vector4(0.0f, 2.0f, 0.0f, 1.0f),
				Vector4(1.0f, 1.0f, 1.0f, 1.0f),
				0.0f,
				0.05f,
				0.0f));

			pointLights.push_back(PointLight(
				Vector3(offset, 10.0f, -offset),
				Vector4(2.0f, 0.0f, 2.0f, 1.0f),
				Vector4(1.0f, 1.0f, 1.0f, 1.0f),
				0.0f,
				0.05f,
				0.0f));
			}

			DrawMarvinAndPlane(marvinWorld, planeWorld, &DemoEngine::VS_PhongTexturedCommon, &DemoEngine::PS_PhongCommon, true, 3);
		}
		break;
	case 11: // 1 Phong Spot Light
	case 12: // 1 Phong Spot Light + Ambient
	case 13: // 1 Phong Spot Light + 1 Directional Light + Ambient
	case 14: // 1 Phong Spot Light + 1 Directional Light + 1 Point Light + Ambient
	case 15: // Case 14 with Additional Models
		{
			this->rasterizer->SetFillMode(Solid);
			this->rasterizer->SetCullMode(CullCounterClockwise);
			this->filterType = Bilinear;

			numSpotLights = 1;

			Matrix4x4 spotLightRot = Matrix4x4::CreateRotationY((float)totalTime);
			Vector3 pos = spotLightRot * Vector3(0.0f, 20.0f, 50.0f); // 0 50 80
			Vector3 lookAt(0.0f, 0.0f, 0.0f);
			Vector3 dir = Vector3::Normalize(lookAt - pos);
	
			spotLights.push_back(SpotLight(
				Vector4(1.0f, 1.0f, 1.0f, 1.0f),// Diffuse Colour
				Vector4(1.0f, 1.0f, 1.0f, 1.0f),// Specular Colour
				pos,							// Light Position
				dir,							// Light Direction
				((float)M_PI / 7.0f),			// Inner Cone Angle (0 -> PI)
				((float)M_PI / 4.0f),			// Outer Cone Angle (theta -> PI)
				1.5f,							// Falloff
				0.0f,							// Constant Attenuation
				0.0f,							// Linear Attenuation
				0.0001f));						// Quadratic Attenuation

			if (testIndex > 11)
				ambientLight = Vector4(0.1f);

			if (testIndex > 12)
			{
				numDirectionalLights = 1;
				directionalLights.push_back(DirectionalLight(
					Vector3::Normalize(Vector3(-5, -5, -5)),
					Vector4(0.5f, 0.5f, 0.5f, 1.0f),
					Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
			}

			if (testIndex > 13)
			{
				float offset = ((float)sin(totalTime) * 30.0f);

				numPointLights = 1;
				pointLights.push_back(PointLight(
					Vector3(offset, 10.0f, offset),
					Vector4(1.0f, 0.0f, 2.0f, 1.0f),
					Vector4(1.0f, 1.0f, 1.0f, 1.0f),
					0.0f,
					0.05f,
					0.0f));
			}

			DrawMarvinAndPlane(marvinWorld, planeWorld, &DemoEngine::VS_PhongTexturedCommon, &DemoEngine::PS_PhongCommon, true, 3);

			if (testIndex > 14)
			{
				// Draw some spheres
				const ModelFrame& sphereFrame = sphere->GetFrames()[0];
				const vector<Vertex, AlignAlloc<Vertex, 16>> sphereVerts = sphereFrame.GetVertices();
				const vector<Face, AlignAlloc<Face, 16>> sphereFaces = sphereFrame.GetFaces();

				for (UINT i = 0; i < 10; i++)
				{
					float t = ((float)i) / 10.0f;
					Matrix4x4 sphereRot = Matrix4x4::CreateRotationY((t * 2.0f * ((float)M_PI)) + (float)totalTime * 0.25f);
					Vector3 spherePos = sphereRot * Vector3(0.0f, (float)sin(totalTime + t * 5.0f) + 4.0f, 15.0f);

					world = Matrix4x4::CreateScale(2.0f + (float)cos(totalTime + t * 7.0f) * 0.48f) * Matrix4x4::CreateRotationZ((float)totalTime) * Matrix4x4::CreateTranslation(spherePos);
					wvp = world * view * projection;
					currentMaterial = &sphereMaterial;

					DrawModel(true, sphereVerts, sphereFaces, true, 3, &DemoEngine::VS_PhongTexturedCommon, &DemoEngine::PS_PhongCommon);
				}
			}
		}
		break;
	case 16: // Single Shadow Mapped Phong Spot Light
	case 17: // Single Shadow Mapped Phong Spot Light, Ambient, Animation
		{
			const vector<ModelFrame>& marvinFrames = marvin->GetFrames();

			int frameIndex = 0;
			if (testIndex == 17)
			{
				// If animation is enabled, get the current frame index based on time
				frameIndex = (int)fmod((totalTime * 10.0), marvinFrames.size());

				// Also enable the ambient light
				ambientLight = Vector4(0.1f);
			}

			const vector<Vertex, AlignAlloc<Vertex, 16>> marvinVerts = marvinFrames[frameIndex].GetVertices();
			const vector<Face, AlignAlloc<Face, 16>> marvinFaces = marvinFrames[frameIndex].GetFaces();

			const vector<Vertex, AlignAlloc<Vertex, 16>> planeVerts = plane->GetFrames()[0].GetVertices();
			const vector<Face, AlignAlloc<Face, 16>> planeFaces = plane->GetFrames()[0].GetFaces();

			// Clear the shadow map depth buffer
			shadowMapDepthBuffer->Clear(1.0f);

			// Set render target to the shadow map depth buffer
			this->rasterizer->SetTargets(NULL, shadowMapDepthBuffer);
			this->rasterizer->SetFillMode(Solid);
			this->rasterizer->SetCullMode(CullCounterClockwise);
			this->filterType = Bilinear;

			// Setup light view and projection matrices
			Matrix4x4 spotLightRot = Matrix4x4::CreateRotationY((float)totalTime * 0.25f);
			Vector3 pos = spotLightRot * Vector3(0.0f, 20.0f, 50.0f); // 0 50 80
			Vector3 lookAt(0.0f, 0.0f, 0.0f);
			Vector3 dir = Vector3::Normalize(lookAt - pos);

			Matrix4x4 lightView = Matrix4x4::CreateLookAt(pos, lookAt, Vector3(0.0f, 1.0f, 0.0f));
			Matrix4x4 lightProj = Matrix4x4::CreatePerspectiveFieldOfView(((float)M_PI) / 4.0f, 1.0f, 0.001f, 200.0f);

			// Render the shadow map
			world = marvinWorld;
			wvp = world * lightView * lightProj;
			currentMaterial = &marvinMaterial;
			DrawModel(true, marvinVerts, marvinFaces, true, 0, &DemoEngine::VS_Simple, NULL);

			world = planeWorld;
			wvp = world * lightView * lightProj;
			currentMaterial = &planeMaterial;
			DrawModel(true, planeVerts, planeFaces, true, 0, &DemoEngine::VS_Simple, NULL);

			// Set the rasterizer targets back to the back buffer
			this->rasterizer->SetTargets(backBufferTarget, backBufferDepth);
			this->rasterizer->SetFillMode(Solid);

			numSpotLights = 1;
			spotLights.push_back(SpotLight(
				Vector4(1.0f, 1.0f, 1.0f, 1.0f),// Diffuse Colour
				Vector4(1.0f, 1.0f, 1.0f, 1.0f),// Specular Colour
				pos,							// Light Position
				dir,							// Light Direction
				((float)M_PI / 7.0f),			// Inner Cone Angle (0 -> PI)
				((float)M_PI / 4.0f),			// Outer Cone Angle (theta -> PI)
				1.5f,							// Falloff
				0.0f,							// Constant Attenuation
				0.0f,							// Linear Attenuation
				0.0001f));						// Quadratic Attenuation

			world = marvinWorld;
			lightWVP = world * lightView * lightProj;
			wvp = world * view * projection;
			currentMaterial = &marvinMaterial;
			DrawModel(true, marvinVerts, marvinFaces, true, 4, &DemoEngine::VS_PhongShadowedSpotLight, &DemoEngine::PS_PhongShadowedSpotLight);

			world = planeWorld;
			lightWVP = world * lightView * lightProj;
			wvp = world * view * projection;
			currentMaterial = &planeMaterial;
			DrawModel(true, planeVerts, planeFaces, true, 4, &DemoEngine::VS_PhongShadowedSpotLight, &DemoEngine::PS_PhongShadowedSpotLight);
		}
		break;
	}
}
void DemoEngine::DrawMarvinAndPlane(const Matrix4x4& marvinWorld, const Matrix4x4& planeWorld, VertexShaderCallback vertexShader, PixelShaderCallback pixelShader, bool usePerVertexNormals, int numVector4Slots)
{
	const vector<Vertex, AlignAlloc<Vertex, 16>> marvinVerts = marvin->GetFrames()[0].GetVertices();
	const vector<Face, AlignAlloc<Face, 16>> marvinFaces = marvin->GetFrames()[0].GetFaces();

	const vector<Vertex, AlignAlloc<Vertex, 16>> planeVerts = plane->GetFrames()[0].GetVertices();
	const vector<Face, AlignAlloc<Face, 16>> planeFaces = plane->GetFrames()[0].GetFaces();

	world = marvinWorld;
	wvp = world * view * projection;
	currentMaterial = &marvinMaterial;
	DrawModel(true, marvinVerts, marvinFaces, usePerVertexNormals, numVector4Slots, vertexShader, pixelShader);

	world = planeWorld;
	wvp = world * view * projection;
	currentMaterial = &planeMaterial;
	DrawModel(true, planeVerts, planeFaces, usePerVertexNormals, numVector4Slots, vertexShader, pixelShader);
}

void DemoEngine::DrawHUD(Gdiplus::Graphics& graphics)
{
	const float col2X = 120.0f;

	// Create GDI+ objects for drawing fonts
	Gdiplus::FontFamily family(L"Arial", NULL);
	Gdiplus::Font font(&family, 8);
	Gdiplus::SolidBrush yellowBrush(Gdiplus::Color(255, 255, 0));
	Gdiplus::SolidBrush whiteBrush(Gdiplus::Color(255, 255, 255));
	Gdiplus::SolidBrush limeBrush(Gdiplus::Color(0, 255, 0));
	Gdiplus::SolidBrush redBrush(Gdiplus::Color(255, 0, 0));

	wostringstream fpsStream;
	fpsStream << fps;

	wostringstream sizeStream;
	sizeStream << this->backBufferTarget->GetWidth() << " x " << this->backBufferTarget->GetHeight();

	graphics.DrawString(sizeStream.str().c_str(), -1, &font, Gdiplus::PointF(0.0f, 0.0f), &limeBrush);

	graphics.DrawString(L"Frames / Second:", -1, &font, Gdiplus::PointF(0.0f, 32.0f), &whiteBrush);
	graphics.DrawString(fpsStream.str().c_str(), -1, &font, Gdiplus::PointF(col2X, 32.0f), &whiteBrush);
	graphics.DrawString(L"Cull Mode:", -1, &font, Gdiplus::PointF(0.0f, 64.0f), &whiteBrush);
	graphics.DrawString(L"Fill Mode:", -1, &font, Gdiplus::PointF(0.0f, 80.0f), &whiteBrush);
	graphics.DrawString(L"Texture Filtering:", -1, &font, Gdiplus::PointF(0.0f, 96.0f), &whiteBrush);
	graphics.DrawString(L"Camera Mode:", -1, &font, Gdiplus::PointF(0.0f, 112.0f), &whiteBrush);
	graphics.DrawString(L"Num Spot Lights:", -1, &font, Gdiplus::PointF(0.0f, 144.0f), &whiteBrush);
	graphics.DrawString(L"Num Point Lights:", -1, &font, Gdiplus::PointF(0.0f, 160.0f), &whiteBrush);
	graphics.DrawString(L"Num Dir. Lights:", -1, &font, Gdiplus::PointF(0.0f, 176.0f), &whiteBrush);
	graphics.DrawString(L"Ambient Light:", -1, &font, Gdiplus::PointF(0.0f, 192.0f), &whiteBrush);

	switch(this->rasterizer->GetCullMode())
	{
	case CullNone: graphics.DrawString(L"None", -1, &font, Gdiplus::PointF(col2X, 64.0f), &redBrush); break;
	case CullClockwise: graphics.DrawString(L"Clockwise", -1, &font, Gdiplus::PointF(col2X, 64.0f), &yellowBrush); break;
	case CullCounterClockwise: graphics.DrawString(L"Counter Clockwise", -1, &font, Gdiplus::PointF(col2X, 64.0f), &limeBrush); break;
	}
	switch(this->rasterizer->GetFillMode())
	{
	case Solid: graphics.DrawString(L"Solid", -1, &font, Gdiplus::PointF(col2X, 80.0f), &limeBrush); break;
	case Wireframe: graphics.DrawString(L"Wireframe", -1, &font, Gdiplus::PointF(col2X, 80.0f), &yellowBrush); break;
	}
	switch(this->filterType)
	{
	case Bilinear: graphics.DrawString(L"Bilinear", -1, &font, Gdiplus::PointF(col2X, 96.0f), &limeBrush); break;
	case Point: graphics.DrawString(L"Point", -1, &font, Gdiplus::PointF(col2X, 96.0f), &yellowBrush); break;
	}

	if (useFreeCamera)
	{
		graphics.DrawString(L"Free", -1, &font, Gdiplus::PointF(col2X, 112.0f), &limeBrush);
	}
	else
	{
		graphics.DrawString(L"Demo Cam", -1, &font, Gdiplus::PointF(col2X, 112.0f), &yellowBrush);
	}

	wstringstream numSpotsStream;
	wstringstream numDirStream;
	wstringstream numPointsStream;

	numSpotsStream << numSpotLights;
	numDirStream << numDirectionalLights;
	numPointsStream << numPointLights;

	graphics.DrawString(numSpotsStream.str().c_str(), -1, &font, Gdiplus::PointF(col2X, 144.0f), numSpotLights > 0 ? &limeBrush : &redBrush);
	graphics.DrawString(numPointsStream.str().c_str(), -1, &font, Gdiplus::PointF(col2X, 160.0f), numPointLights > 0 ? &limeBrush : &redBrush);
	graphics.DrawString(numDirStream.str().c_str(), -1, &font, Gdiplus::PointF(col2X, 176.0f), numDirectionalLights > 0 ? &limeBrush : &redBrush);
	
	bool ambientLightEnabled = ambientLight.LengthSquared() > 0.0f;
	graphics.DrawString(ambientLightEnabled ? L"Enabled" : L"Disabled", -1, &font, Gdiplus::PointF(col2X, 192.0f), ambientLightEnabled ? &limeBrush : &redBrush);

	wstringstream testIndexStream;
	testIndexStream << "Test " << (testIndex + 1) << " of " << NUM_TESTS << " - ";
	switch(testIndex)
	{
	case 0: testIndexStream << "White Wireframe (Culling Disabled)"; break;
	case 1: testIndexStream << "White Wireframe (Culling Enabled)"; break;
	case 2: testIndexStream << "White Flat Shading"; break;
	case 3: testIndexStream << "Faceted Shading - 1 Directional Light"; break;
	case 4: testIndexStream << "Gouraud Shading (Diffuse Only) - 1 Directional Light"; break;
	case 5: testIndexStream << "Simple Texturing (Point Sampling)"; break;
	case 6: testIndexStream << "Simple Texturing (Bilinear Sampling)"; break;
	case 7: testIndexStream << "Point and Bilinear Texture Sampling Comparison"; break;
	case 8: testIndexStream << "Phong Shading - 1 Point Light"; break;
	case 9: testIndexStream << "Phong Shading - 5 Point Lights"; break;
	case 10: testIndexStream << "Phong Shading - 5 Point Lights - Lit Wireframe"; break;
	case 11: testIndexStream << "Phong Shading - 1 Spot Light"; break;
	case 12: testIndexStream << "Phong Shading - 1 Spot Light + Ambient"; break;
	case 13: testIndexStream << "Phong Shading - 1 Spot Light, 1 Dir. Light + Ambient"; break;
	case 14: testIndexStream << "Phong Shading - All Light Types"; break;
	case 15: testIndexStream << "Phong Shading - All Light Types with Additional Geometry"; break;
	case 16: testIndexStream << "Phong Shading - 1 Spot Light with 512x512 Shadow Mapping"; break;
	case 17: testIndexStream << "Shadow Mapped Spot Light, Ambient Light, and MD2 Frame Animation"; break;
	}

	Gdiplus::RectF strBounds;
	graphics.MeasureString(testIndexStream.str().c_str(), -1, &font, Gdiplus::PointF(0, 0), &strBounds);
	graphics.DrawString(testIndexStream.str().c_str(), -1, &font, Gdiplus::PointF((((float)this->backBufferTarget->GetWidth()) - strBounds.Width) * 0.5f, 0.0f), &whiteBrush);

	if (testTimerEnabled)
	{
		wstringstream progressStream;
		progressStream << "Test Progress - " << std::fixed << std::setprecision(0) << (float)((testTimer / MAX_TEST_TIME) * 100.0) << "%";
		graphics.MeasureString(progressStream.str().c_str(), -1, &font, Gdiplus::PointF(0, 0), &strBounds);
		graphics.DrawString(progressStream.str().c_str(), -1, &font, Gdiplus::PointF((((float)this->backBufferTarget->GetWidth()) - strBounds.Width), 0.0f), &whiteBrush);
	}
	else
	{
		const wchar_t* str = L"Test Timer Disabled";
		graphics.MeasureString(str, -1, &font, Gdiplus::PointF(0, 0), &strBounds);
		graphics.DrawString(str, -1, &font, Gdiplus::PointF((((float)this->backBufferTarget->GetWidth()) - strBounds.Width), 0.0f), &redBrush);
	}

	if (testIndex == 7 && !useFreeCamera)
	{
		const wchar_t* str1 = L"Point Sampling";
		const wchar_t* str2 = L"Bilinear Sampling";

		float halfWindowWidth = ((float)backBufferTarget->GetWidth()) * 0.5f;
		float windowHeight = (float)backBufferTarget->GetHeight();

		graphics.MeasureString(str1, -1, &font, Gdiplus::PointF(0, 0), &strBounds);
		graphics.DrawString(str1, -1, &font, Gdiplus::PointF((halfWindowWidth - strBounds.Width) * 0.5f, windowHeight - strBounds.Height), &whiteBrush);

		graphics.MeasureString(str2, -1, &font, Gdiplus::PointF(0, 0), &strBounds);
		graphics.DrawString(str2, -1, &font, Gdiplus::PointF((halfWindowWidth - strBounds.Width) * 0.5f + halfWindowWidth, windowHeight - strBounds.Height), &whiteBrush);
	}
}

// -------------------------------------------------------------
// Vertex and Pixel Shaders
// -------------------------------------------------------------

void DemoEngine::VS_Simple(const Vertex& input, VS_Output& output) const
{
	output.Position = wvp * Vector4(input.Position, 1.0f);
}
void DemoEngine::VS_GouraudDiffuse(const Vertex& input, VS_Output& output) const
{
	output.Position = wvp * Vector4(input.Position, 1.0f);

	// Calculate sum of all directional lights in the scene
	Vector4 totalLight;
	for (UINT i = 0; i < numDirectionalLights; i++)
	{
		const DirectionalLight& light = directionalLights[i];

		float l_dot_n = Vector3::Dot((-light.GetDirection()), input.Normal);
		l_dot_n = max(l_dot_n, 0.0f);

		Vector4 lightDiff = light.GetDiffuse();
		totalLight += lightDiff * l_dot_n;
	}

	// Output summed light intensity in slot 0
	output.Slots[0] = totalLight;
}
void DemoEngine::VS_SimpleTextured(const Vertex& input, VS_Output& output) const
{
	output.Position = wvp * Vector4(input.Position, 1.0f);
	output.Slots[0] = Vector4(input.TexCoord, 0.0f, 0.0f);
}
void DemoEngine::VS_PhongTexturedCommon(const Vertex& input, VS_Output& output) const
{
	output.Position = wvp * Vector4(input.Position, 1.0f);
	output.Slots[0] = Vector4(world * input.Position, 0.0f);
	output.Slots[1] = Vector4(world.TransformNormal(input.Normal), 0.0f);
	output.Slots[2] = Vector4(input.TexCoord, 0.0f, 0.0f);
}
void DemoEngine::VS_PhongShadowedSpotLight(const Vertex& input, VS_Output& output) const
{
	output.Position = wvp * Vector4(input.Position, 1.0f);
	output.Slots[0] = Vector4(world * input.Position, 0.0f);
	output.Slots[1] = Vector4(world.TransformNormal(input.Normal), 0.0f);
	output.Slots[2] = Vector4(input.TexCoord, 0.0f, 0.0f);
	output.Slots[3] = lightWVP * Vector4(input.Position, 1.0f);
}

void DemoEngine::PS_White(const VS_Output& input, Vector4& output) const
{
	output = Vector4(1.0f);
}
void DemoEngine::PS_Slot0Color(const VS_Output& input, Vector4& output) const
{
	output = input.Slots[0];
}
void DemoEngine::PS_SimpleTexturedPoint(const VS_Output& input, Vector4& output) const
{
	output = currentMaterial->GetDiffuseTexture()->SamplePoint(input.Slots[0].AsVector2());
}
void DemoEngine::PS_SimpleTexturedBilinear(const VS_Output& input, Vector4& output) const
{
	output = currentMaterial->GetDiffuseTexture()->SampleBilinear(input.Slots[0].AsVector2());
}
void DemoEngine::PS_PhongCommon(const VS_Output& input, Vector4& output) const
{
	// Extract interpolated pixel attributes
	Vector3 posWorld = input.Slots[0].AsVector3();
	Vector3 normal = Vector3::Normalize(input.Slots[1].AsVector3());
	Vector2 texCoord = input.Slots[2].AsVector2();

	// Calculate the common viewing vector for specular reflection
	Vector3 v = Vector3::Normalize(camPos - posWorld);

	// Sample the diffuse texture at this pixel
	Vector4 diffuseSample;
	switch(filterType)
	{
	case Point: diffuseSample = currentMaterial->GetDiffuseTexture()->SamplePoint(texCoord); break;
	case Bilinear: diffuseSample = currentMaterial->GetDiffuseTexture()->SampleBilinear(texCoord); break;
	}

	// Calculate ambient light
	output = diffuseSample * currentMaterial->GetAmbientColor() * ambientLight;

	// Calculate point light contributions
	for (UINT i = 0; i < numPointLights; i++)
	{
		const PointLight& light = pointLights[i];

		// Get direction vector to light, and distance to light
		Vector3 dirToLight = light.GetPosition() - posWorld;
		float distToLight = dirToLight.Length();
		dirToLight = Vector3::Normalize(dirToLight);

		// Calculate light attenuation
		float atten = light.CalculateAttenuation(distToLight);

		// Calculate diffuse factor
		float diffuseI = Vector3::Dot(dirToLight, normal);
		diffuseI = max(diffuseI, 0.0f);

		// Calculate specular factor
		Vector3 h = Vector3::Normalize(dirToLight + v);
		float n_dot_h = Vector3::Dot(normal, h);
		float specularI = pow(max(n_dot_h, 0.0f), currentMaterial->GetSpecularPower());

		// Sum light values to the final pixel color
		output += light.GetDiffuse() * diffuseSample * diffuseI * atten;
		output += light.GetSpecular() * specularI * atten;
	}
	
	// Calculate spot light contributions
	for (UINT i = 0; i < numSpotLights; i++)
	{
		const SpotLight& light = spotLights[i];
		
		// Get direction vector to light, and distance to light
		Vector3 dirToLight = light.GetPosition() - posWorld;
		float distToLight = dirToLight.Length();
		dirToLight = Vector3::Normalize(dirToLight);

		// Calculate spot light factor, and skip if the factor <= 0.0f
		float spotLightFactor;
		if (light.CalculateSpotLightFactor(Vector3::Dot((-light.GetDirection()), dirToLight), spotLightFactor))
		{
			// Calculate light attenuation
			float atten = light.CalculateAttenuation(distToLight);

			// Calculate diffuse factor
			float diffuseI = Vector3::Dot(dirToLight, normal);
			diffuseI = max(diffuseI, 0.0f);

			// Calculate specular factor
			Vector3 h = Vector3::Normalize(dirToLight + v);
			float n_dot_h = Vector3::Dot(normal, h);
			float specularI = pow(max(n_dot_h, 0.0f), currentMaterial->GetSpecularPower());

			// Sum light values to the final pixel color
			output += light.GetDiffuse() * diffuseSample * diffuseI * atten * spotLightFactor;
			output += light.GetSpecular() * specularI * atten * spotLightFactor;
		}
	}

	// Calculate Directional Light contributions
	for (UINT i = 0; i < numDirectionalLights; i++)
	{
		const DirectionalLight& light = directionalLights[i];

		// Calculate diffuse factor
		float diffuseI = Vector3::Dot((-light.GetDirection()), normal);
		diffuseI = max(diffuseI, 0.0f);

		// Calculate specular factor
		Vector3 h = Vector3::Normalize(-light.GetDirection() + v);
		float n_dot_h = Vector3::Dot(normal, h);
		float specularI = pow(max(n_dot_h, 0.0f), currentMaterial->GetSpecularPower());

		// Sum light values to the final pixel color
		output += light.GetDiffuse() * diffuseSample * diffuseI;
		output += light.GetSpecular() * specularI;
	}
}
void DemoEngine::PS_PhongShadowedSpotLight(const VS_Output& input, Vector4& output) const
{
	Vector3 posWorld = input.Slots[0].AsVector3();
	Vector3 normal = Vector3::Normalize(input.Slots[1].AsVector3());
	Vector2 texCoord = input.Slots[2].AsVector2();

	// Calculate the common viewing vector for specular reflection
	Vector3 v = Vector3::Normalize(camPos - posWorld);

	// Sample the diffuse texture at this pixel
	Vector4 diffuseSample;
	switch(filterType)
	{
	case Point: diffuseSample = currentMaterial->GetDiffuseTexture()->SamplePoint(texCoord); break;
	case Bilinear: diffuseSample = currentMaterial->GetDiffuseTexture()->SampleBilinear(texCoord); break;
	}

	// Calculate ambient light
	output = diffuseSample * currentMaterial->GetAmbientColor() * ambientLight;

	const SpotLight& light = spotLights[0];

	// Get direction vector to light, and distance to light
	Vector3 dirToLight = light.GetPosition() - posWorld;
	float distToLight = dirToLight.Length();
	dirToLight = Vector3::Normalize(dirToLight);

	// Calculate spot light factor, and skip if the factor <= 0.0f
	float spotLightFactor;
	if (light.CalculateSpotLightFactor(Vector3::Dot((-light.GetDirection()), dirToLight), spotLightFactor))
	{
		float shadowFactor = 1.0f;

		Vector4 lightProjPos = input.Slots[3];
		lightProjPos /= lightProjPos.W;
		if (lightProjPos.IsInView())
		{
			Vector2 shadowMapTexCoord = (lightProjPos.AsVector2() + Vector2(1.0f)) * 0.5f;
			float shadowMapDepth = shadowMapDepthBuffer->SamplePoint(shadowMapTexCoord);

			if (shadowMapDepth < lightProjPos.Z - 0.0000008f)
			{
				shadowFactor = 0.2f;
			}
		}

		// Calculate light attenuation
		float atten = light.CalculateAttenuation(distToLight);

		// Calculate diffuse factor
		float diffuseI = Vector3::Dot(dirToLight, normal);
		diffuseI = max(diffuseI, 0.0f);

		// Calculate specular factor
		Vector3 h = Vector3::Normalize(dirToLight + v);
		float n_dot_h = Vector3::Dot(normal, h);
		float specularI = pow(max(n_dot_h, 0.0f), currentMaterial->GetSpecularPower());

		// Sum light values to the final pixel color
		output += light.GetDiffuse() * diffuseSample * diffuseI * atten * spotLightFactor * shadowFactor;
		output += light.GetSpecular() * specularI * atten * spotLightFactor * shadowFactor;
	}
}