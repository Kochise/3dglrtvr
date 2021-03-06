/*
 * SoftPixel Engine main header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

/*
 * ========== LICENSE (zlib/libpng): ==========
 * 
 * Copyright (c) 2008 Lukas Hermanns
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 *     1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 * 
 *     2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 * 
 *     3. This notice may not be removed or altered from any source
 *     distribution.
 * 
 * Please note that the SoftPixel Engine is based in part on the work of the
 * Independent JPEG Group, the zlib and the libpng. This means that if you use
 * the SoftPixel Engine in your product, you must acknowledge somewhere in your
 * documentation that you've used the IJG code. It would also be nice to mention
 * that you use the SoftPixel Engine, the zlib and libpng. See the README files
 * in the jpeglib, the zlib and libpng for further information.
 * 
 */

#ifndef __cplusplus
#   error The 'SoftPixel Engine' is only compilable with C++!
#endif

#include "Base/spStandard.hpp"
#include "Base/spDimension.hpp"                     // dim
#include "Base/spInputOutput.hpp"                   // io
#include "Base/spMath.hpp"                          // math
#include "Platform/spSoftPixelDeviceOS.hpp"
#include "GUI/spGUIManager.hpp"                     // gui
#include "RenderSystem/spRenderSystem.hpp"          // video
#include "SceneGraph/spSceneGraph.hpp"              // scene
#include "Framework/Sound/spSoundDevice.hpp"        // audio
#include "Framework/Network/spNetworkSystem.hpp"    // network
#include "Framework/Physics/spPhysicsSystem.hpp"    // physics
#include "Framework/Tools/spExtendedToolset.hpp"    // tool


//
// ============== FOR DOXYGEN DOCUMENTATION ==============
//

/*! \mainpage SoftPixel Engine 3.1 alpha API documentation
 * 
 * <div algin="center"><img alt="SoftPixel Engine Logo" src="../media/SoftPixelEngineLogo.png"></div>
 * 
 * \section intro Introduction
 * 
 * Welcome to the SoftPixel Engine API documentation where you can find each function available for the current version.
 * For detailed questions you can joint the <a href="http://www.hlc-games.de/forum/">HLC Games board</a>
 * and ask questions in the section of the SoftPixel Engine.
 * You can also download this API docu as <a href="http://softpixelengine.sourceforge.net/download/SoftPixelEngineDocu.zip">ZIP file</a>.
 * 
 * \section getstart Getting started
 * 
 * This is the "GettingStarted" tutorial for the first step using the SoftPixel Engine.
 * 
 * \code
 * #include <SoftPixelEngine.hpp>
 * 
 * using namespace sp; // Main namespace
 * 
 * int main()
 * {
 *     // The softpixel-device opens the screen (windowed screen or fullscreen is possible)
 *     SoftPixelDevice* spDevice = createGraphicsDevice(
 *         video::RENDERER_AUTODETECT, dim::size2di(640, 480), 32, "GettingStarted"
 *     );
 *     
 *     video::RenderSystem* spRenderer = spDevice->getRenderSystem(); // Render system for drawing operations
 *     video::RenderContext* spContext = spDevice->getRenderContext(); // Render context to flip video buffers
 *     scene::SceneGraph* spScene = spDevice->getSceneGraph(); // Scene graph system to handle a lots of scene objects
 *     io::InputControl* spControl = spDevice->getInputControl(); // Controller for keyboard- and the mouse input events
 *     
 *     scene::Camera* Cam = spScene->createCamera(); // Creates a camera to see our scene
 *     scene::Light* Lit = spScene->createLight(); // Creates a light (default: Directional light)
 *     spScene->setLighting(); // Activates the lighting
 *     
 *     scene::Mesh* Obj = spScene->createMesh(scene::MESH_TEAPOT); // Creates one of the standard objects
 *     Obj->setPosition(dim::vector3df(0, 0, 2.5)); // Sets the object position (x, y, z)
 *     
 *     // The main loop in which the device will be updated
 *     while (spDevice->updateEvent() && !spControl->keyDown(io::KEY_ESCAPE))
 *     {
 *         spRenderer->clearBuffers(); // Clears the video buffer
 *         spScene->renderScene(); // Renders the scene
 *         
 *         Obj->turn(dim::vector3df(1, 1, 1)); // Rotates our object
 *         
 *         spContext->flipBuffers(); // Swaps the video buffer, so we can see the current frame
 *     }
 *     
 *     deleteDevice(); // Deletes the device context and closes the screen
 *     
 *     return 0;
 * }
 * \endcode
 */

/**
 * \page page_credits Credits
 * 
 * \htmlonly
 * <ul>
 * <li>Lukas Hermanns (Creator, developer, project lead)</li>
 * </ul>
 * \endhtmlonly
 */

/**
 * \page page_license License
 * 
 * \htmlonly
 * <pre>
 * Copyright (c) 2008 Lukas Hermanns
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 *     1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 * 
 *     2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 * 
 *     3. This notice may not be removed or altered from any source
 *     distribution.
 * </pre>
 * \endhtmlonly
 */

/**
 * \page page_todo Todo list
 * 
 * \htmlonly
 * <ul>
 * <li>More scene managers</li>
 * <li>Clipping planes -> correct enable/disable</li>
 * <li>3D File formats loader & saver update</li>
 * <li>Collision detection robustness</li>
 * <li>Stencil shadow volumes</li>
 * <li>2D Drawing extend for renderers (Ray and D3D9)</li>
 * </ul>
 * \endhtmlonly
 */

/**
 * \page page_changes17_18 Changes from 1.7 to 1.8
 * 
 * \code
 * // === Changes for Movies === //
 * 
 * namespace movie -> has been removed
 * movie::Movie -> video::Movie
 * movie::MovieManager::loadMovie -> video::VideoDriver::loadMovie
 * movie::MovieManager::renderMovieTexture -> video::Movie::renderTexture
 * movie::MovieManager::playMovie -> video::Movie::play
 * 
 * // Example:
 * video::Movie* MyMovieObject = spRenderer->loadMovie("MyMovieFile.avi");
 * 
 * if (!MyMovieObject->valid())
 *     io::printError("The movie could not be loaded");
 * 
 * MyMovieObject->play();
 * MyMovieObject->setSeek(0.5); // To the middle of the movie
 * MyMovieObject->setSpeed(1.5); // Play with 1.5 times the speed, negative values are also possible but much slower!
 * 
 * MyMovieObject->renderTexture(MyTextureObject);
 * 
 * MyMovieObject->reload("MyMovieField2.avi"); // You can also reload an other movie file using the same movie object handle
 * // /Example
 * 
 * 
 * // === Changes for Sounds === //
 * 
 * createListener -> createSoundDevice
 * 
 * // Example:
 * sound::SensitiveSoundDevice* spListener = sound::createSoundDevice();
 * 
 * sound::Sound* MySoundObject = spListener->loadSound("MySoundFile.wav");
 * 
 * if (!MySoundObject->valid())
 *     io::printError("The sound could not be loaded");
 * 
 * MySoundObject->play(); // Remember to the "Movie" class with its simple functions: "play", "pause", "stop"
 * MySoundObject->setVolume(0.5); // 50% volume
 * MySoundObject->setSeek(0.1);
 * MySoundObject->setSpeed(1.5); // 1.5 times the speed, 1.0 is default (in the old version 0.0 was normal, -1.0 was slow, 1.0 was fast)
 * 
 * while (!MySoundObject->playing()) // Check if the sound is currently playing
 *     if (MySoundObject->finish()) // Stupid request within the above code, but just an example :)
 *         io::printMessage("The sound has been finished");
 * // /Example
 * 
 * // New enumerations
 * SOUNDDEVICE_AUTODETECT
 * SOUNDDEVICE_WINMM
 * SOUNDDEVICE_DUMMY
 * 
 * 
 * // === Changes for Networking === //
 * 
 * // Example:
 * network::NetworkSystem* spNetwork = new network::NetworkSystem(); // (going to be changed in "spDevice->getNetworkSystem()")
 * 
 * if (I_Am_The_Server)
 * {
 *     if (!spNetwork->openServer(Port)) // Default port number is 10000
 *         io::printError("The server could not be opened");
 * }
 * else
 * {
 *     // Useful function is "std::vector<io::stringc> spNetwork->getIPAddressesByName("HostName")" to each IP address of the host by its name
 *     if (!spNetwork->joinServer("192.168.0.1", Port)) // The IP address of the host
 *         io::printError("The server could not be opened");
 * }
 * 
 * spNetwork->sendPacket("MyMessage", 0); // instead of 0 you can use an "network::NetworkClient" pointer for a special Acceptor, otherwise (0) the message will be send to all in the network (which joined the SPE server)
 * 
 * network::SNetworkPacket MyPacket;
 * 
 * if (spNetwork->pickPacket(MyPacket))
 *     io::printMessage(io::stringc("I've got a network message: ") + MyPacket.Message); // Received a message from "MyPacket.Sender"
 * 
 * spNetwork->disconnect(); // Disconnect from the server or the server self
 * // /Example
 * 
 * 
 * // === Changes for File system === //
 * 
 * io::FileLoader -> io::FileSystem
 * 
 * // Example:
 * io::FileSystem* spFileSys = spDevice->getFileSystem(); // alternativly "new io::FileSystem();"
 * io::File* MyFileObject = spFileSys->openFile("MyFile.dat");
 * 
 * if (!MyFileObject) // In this case no object will be created when the file could not be opened (e.g. because of missing file)!
 *     io::printError("The file could not be opened");
 * 
 * MyFileObject->writeString("MyTextField"); // Writes just a string
 * MyFileObject->writeStringN("MyTextField"); // Writes a string and a new line character (on Windows: char(13) & char(10), on Linux: char(10))
 * MyFileObject->writeStringC("MyTextField"); // Writes an ANSI C string, terminated by 0
 * MyFileObject->writeStringData("MyTextField"); // Writes an interger value with the string length and the string self
 * 
 * MyFileObject->writeValue<s32>(math::Random(100)); // Writes a random integer: "s32", 32 bit value
 * 
 * while (!MyFileObject->isEOF()) // Check "EndOfFile"
 *     io::printMessage(MyFileObject->readString()); // Reads a string until the line ends (description see "writeStringN")
 * 
 * MyFileObject->readValue<s32>(); // Reads an interger value
 * 
 * FILE* AnsiCFileHandle = MyFileObject->getHandle(); // Old name "getID", return the ANSI C "FILE" pointer/ handle to proceed own ANSI C file operations.
 * s32 FileSizeInBytes = MyFileObject->getSize(); // Old name "getLength"
 * 
 * spFileSys->closeFile(MyFileObject);
 * // /Example
 * 
 * 
 * // === Changes for Entities and Nodes === //
 * 
 * Node::doTranslate -> Node::translate
 * Node::doTransform -> Node::transform
 * Node::doTurn -> Node::turn
 * Node::doMove -> Node::move
 * Entity::doTextureTranslate -> Entity::textureTranslate
 * Entity::doTextureTransform -> Entity::textureTransform
 * Entity::doTextureTurn -> Entity::textureTurn
 * Entity::doMeshTranslate-> Entity::meshTranslate
 * Entity::doMeshTransform -> Entity::meshTranslate
 * Entity::doMeshTurn-> Entity::meshTurn
 * 
 * // Example:
 * scene::Entity* MyMeshObjectA = spSmngr->createModel(scene::ENTITY_TEAPOT);
 * scene::Entity* MyMeshObjectB = spSmngr->createModel(scene::ENTITY_TORUS, 15); // 15 is the count of segments, allowed because first parameter of "SModelConstruct" structure is the segmentation, all parameters have default values.
 * 
 * scene::SModelConstruct MyConstruct;
 * MyConstruct.Segments = 10; // Segmentation
 * MyConstruct.DegreeLength = 360.0f * 2; // two rotations for the spiral
 * MyConstruct.Size = 1.0f; // Distance between the ring of the spiral in one rotation
 * scene::Entity* MyMeshObjectA = spSmngr->createModel(scene::ENTITY_SPIRAL, MyConstruct);
 * // /Example
 * 
 * 
 * // === Changes for Animation === //
 * 
 * +scene::AnimationNode
 * +scene::AnimationMorphTarget
 * +scene::AnimationSkeletal
 * scene::Animation::doAnimate -> play (+pause, stop, setSpeed, getSeek etc.)
 * 
 * 
 * // === Changes for UserControl === //
 * 
 * UserControl::isKeyPressed -> keyDown
 * UserControl::isKeyReleased -> keyReleased
 * UserControl::isKeyHit -> keyHit
 * UserControl::isMouseButtonPressed -> mouseDown
 * UserControl::isMouseButtonReleased -> mouseReleased
 * UserControl::isMouseButtonHit -> mouseHit
 * \endcode
 */

/**
 * \page page_changes18_19 Changes from 1.8 to 1.9
 * 
 * \code
 * // === Function name changes (dim::matrix4) === //
 * matrixReset -> reset
 * matrixTranslate -> translate
 * matrixRotateX -> rotateX
 * matrixRotateY -> rotateY
 * matrixRotateZ -> rotateZ
 * matrixRotateYXZ -> rotateYXZ
 * matrixRotate -> rotate
 * getArray16 -> getArray
 * 
 * // === Function name changes (dim::vector3d) === //
 * isEqual -> equal
 * isEmpty -> empty
 * 
 * // === Function name changes (scene::Entity) === //
 * getBoundingBox -> getMeshBoundingBox
 * getBoundingSphere -> getMeshBoundingSphere
 * getMeshSurfaceCount -> getSurfaceCount
 * getMeshTriangleCount -> getTriangleCount
 * getMeshVerticesCount -> getVertexCount
 * getTriangleVertices -> getTriangleIndices
 * 
 * // When used before "updateModeling(false)" use now: "updateMeshBuffer"
 * // When used before "updateModeling" oder "updateModeling(true)" use now: "updateNormals"
 * updateModeling -> updateMeshBuffer
 * 
 * // === Function name changes (scene::SceneManager) === //
 * createJoint -> createNode
 * 
 * // === Function name changes (video::Texture) === //
 * setTransFilter -> setColorKey
 * setTransFilterAlpha -> setColorKeyAlpha
 * setTransFilterMask -> setColorKeyMask
 * 
 * // === Other === //
 * math::isEqual -> math::Equal
 * video::SMeshMaterial::hShader -> video::SMeshMaterial::ShaderObject
 * \endcode
 */

/**
 * \page page_changes19_20 Changes from 1.9 to 2.0
 * 
 * \code
 * // === Class name changes === //
 * scene::Entity -> scene::Mesh
 * scene::Sprite -> scene::Billboard
 * 
 * // === Function name changes (scene::SceneManager) === //
 * createPivot -> createMesh
 * createModel -> createMesh
 * loadModel -> loadMesh
 * saveModel -> saveMesh
 * createSprite -> createBillboard
 * getEntityList -> getMeshList
 * getSpriteList -> getBillboardList
 * 
 * // === Function name changes (scene::Mesh) === //
 * selectSurface -> setActiveSurface
 * 
 * // === Enumeration entry name changes === //
 * NODE_ENTITY -> NODE_MESH
 * NODE_SPRITE -> NODE_BILLBOARD
 * 
 * ENTITY_CUBE -> MESH_CUBE
 * ENTITY_SPHERE -> MESH_SPHERE
 * ...
 * 
 * MODEL_SPM -> MESHFORMAT_SPM
 * MODEL_3DS -> MESHFORMAT_3DS
 * ...
 * 
 * SCENE_SPS -> SCENEFORMAT_SPS
 * SCENE_BSP -> SCENEFORMAT_BSP
 * \endcode
 */

/**
 * \page page_changes20_21 Changes from 2.0 to 2.1
 * 
 * \code
 * // === Class & structure name changes === //
 * ModelCreator -> BasicMeshGenerator
 * SMeshMaterial -> SMaterialStates
 * SHeightMapData -> SHeightMapTexture
 * 
 * // === Function name changes (scene::Mesh) === //
 * setShadingType -> setShading
 * flipMesh -> meshFlip
 * fitMesh -> meshFit
 *
 * // === Removed functions (scene::Mesh) === //
 * drawNormals
 * renderCelShading (can be solved using shaders)
 * setVertexCoordArray
 * getTriangleArray
 * getTriangleList
 * 
 * // === Function name changes (other) === //
 * video::color::flipColors -> setInverse (+getInverse)
 * 
 * // === Function design changes (video::VideoDriver) === //
 * loadShader() -> ...
 * 
 * // === Class inheritance changes === //
 * 
 * BasicObject                 BasicObject
 *  |_ scene::Node              |_ scene::Node
 *      |_ scene::Mesh              |_ scene::MaterialNode
 *      |_ scene::Billboard         |   |_ scene::Mesh
 *      |_ scene::Terrain   ->      |   |_ scene::Billboard
 *      |_ scene::Camera            |   |_ scene::Terrain
 *      |_ scene::Light             |_ scene::Camera
 *                                  |_ scene::Light
 * 
 * // === Enumeration entry name changes === //
 * SCENEFORMAT_BSP -> SCENEFORMAT_BSP1 and SCENEFORMAT_BSP3
 * 
 * // === Other changes === //
 * ESkyBoxTypes -> has been removed (was needed because TGA image files where not flipped vertical; this has been solved)
 * \endcode
 */

/**
 * \page page_changes21_30 Changes from 2.1 to 3.0
 * 
 * \code
 * // === Function name changes (video::Texture) === //
 * makeRenderTarget -> setRenderTarget / getRenderTarget
 * make1Dimensional -> setDimension
 * make2Dimensional -> setDimension
 * make3Dimensional -> setDimension
 * makeCubeMap -> setDimension
 * 
 * // === Class inheritance changes === //
 * 
 *                                   BasicObject
 *                                    |_ Node
 * BasicObject                            |_ scene::SceneNode (this is the new Node class)
 *  |_ scene::Node                            |_ scene::Camera
 *      |_ scene::MaterialNode                |_ scene::Light
 *      |   |_ scene::Mesh                    |_ scene::RenderNode
 *      |   |_ scene::Billboard  ->               |_ scene::MaterialNode
 *      |   |_ scene::Terrain                     |   |_ scene::Mesh
 *      |_ scene::Camera                          |   |_ scene::Billboard
 *      |_ scene::Light                           |   |_ scene::Terrain
 *                                                |_ scene::SceneGraph (this is the new SceneManager class)
 *                                                    |_ scene::SceneGraphSimple
 *                                                    |_ scene::SceneGraphSimpleChildTree
 * 
 * // === Function name changes (scene::Mesh) === //
 * setColor -> paint
 * 
 * // === Other changes === //
 * scene::SMeshSurfaceObject -> video::MeshBuffer
 * scene::Mesh::setVertexCounter -> video::MeshBuffer::setIndexOffset
 * scene::Mesh::addVertexCounter -> video::MeshBuffer::addIndexOffset
 * Mesh::setMeshReference -> Mesh::setReference
 * MeshBuffer::setSurfaceReference -> MeshBuffer::setReference
 * \endcode
 */

/**
 * \page page_changes30_31 Changes from 3.0 to 3.1
 * 
 * \code
 * // === Namespace name changes === //
 * sound -> audio (since the sound device has changed also the namespace name has been adjusted)
 * 
 * // === Function name changes (math::) === //
 * Round2 -> RoundPow2
 * SeedRandom -> math::Randomizer::seedRandom
 * Rnd -> math::Randomizer::randInt
 * Random -> math::Randomizer::randInt
 * 
 * createSoundDevice -> SoftPixelDevice::getSoundDevice
 * 
 * // === Other changes === //
 * video::Texture::getDimensionType -> video::Texture::getDimension
 * 
 * scene::SceneGraph::saveScene -> Has been removed, you can create and save scenes with the "SoftPixel Sandbox".
 * 
 * // === Macros === //
 * #define foreach BOOST_FOREACH -> if you don't want this macro define SP_DONT_DEFINE_BOOST_MACROS before including "SoftPixelEngine.hpp".
 * 
 * // === Enumeration entry name changes === //
 * SCENEFORMAT_SPS -> SCENEFORMAT_SPSB
 * \endcode
 */

/**
 * \page page_howto1 How to create a cube-map
 * 
 * \code
 * // Load first cubemap face (+X).
 * video::Texture* CubeMap = spRenderer->loadTexture("CubeMapFace0.jpg");
 * 
 * // Load other 5 cubemap faces and add them at the end of the image buffer.
 * for (s32 i = 1; i < 6; ++i)
 *     CubeMap->addTextureFrame(spRenderer->loadTexture("CubeMapFace" + io::stringc(i) + ".jpg"), video::TEXTURE_FRAME_VERTICAL);
 * 
 * // Configure the cubemap texture.
 * CubeMap->setDimension(video::TEXTURE_CUBEMAP);
 * CubeMap->setWrapMode(video::TEXWRAP_CLAMP);
 * 
 * // Add the cubemap to the object and configure texture coordinate generation.
 * MeshObject->addTexture(CubeMap);
 * MeshObject->getMeshBuffer(0)->setMappingGen(0, video::MAPGEN_REFLECTION_MAP);
 * \endcode
 */

/**
 * \page page_howto2 How to create a dynamic cube-map
 * 
 * \code
 * // Create the texture with six times the height (because of the six cubemap faces).
 * const s32 CubeMapSize = 256;
 * video::Texture* CubeMap = spRenderer->createTexture(dim::size2di(CubeMapSize, CubeMapSize*6));
 * 
 * // Configure the cubemap texture and convert it to a render-target texture.
 * CubeMap->setDimension(video::TEXTURE_CUBEMAP);
 * CubeMap->setWrapMode(video::TEXWRAP_CLAMP);
 * CubeMap->setRenderTarget(true);
 * 
 * // Add the cubemap to the object and configure texture coordinate generation.
 * MeshObject->addTexture(CubeMap);
 * MeshObject->getMeshBuffer(0)->setMappingGen(0, video::MAPGEN_REFLECTION_MAP);
 * 
 * // ...
 * 
 * // Update the cubemap every frame (or only each second frame to save performance).
 * MeshObject->setVisible(false);
 * spRenderer->updateCubeMap(CubeMap, MeshObject->getPosition(true));
 * MeshObject->setVisible(true);
 * \endcode
 */

/**
 * \page page_howto3 How to create a tracking shot
 * 
 * \code
 * // Move with "tool::moveCameraFree" your camera where you want a node in your animation sequence and capture its location.
 * if (spControl->mouseHit(io::MOUSE_LEFT))
 * {
 *     const dim::vector3df Pos(Cam->getPosition(true));
 *     const dim::quaternion Rot(dim::quaternion(Cam->getRotationMatrix(true)).getInverse());
 *     
 *     // Store it in a file where you can copy and paste the code.
 *     File->writeStringN(
 *         "Anim->addSequence(dim::vector3df(" + io::stringc(Pos.X) + ", " + io::stringc(Pos.Y) + ", " + io::stringc(Pos.Z) +
 *         "), dim::quaternion(" + io::stringc(Rot.X) + ", " + io::stringc(Rot.Y) + ", " + io::stringc(Rot.Z) + ", " + io::stringc(Rot.W) + "));"
 *     );
 * }
 * 
 * // ...
 * 
 * // After creating your sequence copy and paste the generated code to create your animation and do some configurations.
 * Anim->setSplineTranslation(true);
 * Anim->setSpeed(0.01f);
 * \endcode
 */

//! Main namespace in which everything can be found.
namespace sp
{

//! Basic classes such as vectors, planes, matrix etc. can be found in this namespace.
namespace dim { }

//! Namespace for each GUI (Graphics User Interface) objects (e.g. Windows, Buttons etc.) for developing level editors or the like.
namespace gui { }

//! All simple text output and string class are in this namespace.
namespace io { }

//! This namespace contains all mathematical basic functions such as sine, cosine, distance calculations etc.
namespace math { }

//! This namespace contains all network functions such as connecting, sending and receiving messages.
namespace network { }

//! Namespace for physics simulations using NewtonGameDynamics library.
namespace physics { }

//! Add 3D object management functions and scene rendering can be found in this namespace.
namespace scene { }

//! All audio, sound and music operation functions can be found here. Also volumetric 3D sound effects are in this namespace.
namespace audio { }

//! This namespace contains all extendet tools for particular game development which should be used in a level editor.
namespace tool { }

//! All rendering and drawing operations for 2D and 3D can be found in this namespace.
namespace video { }

} // /namespace sp



// ================================================================================
