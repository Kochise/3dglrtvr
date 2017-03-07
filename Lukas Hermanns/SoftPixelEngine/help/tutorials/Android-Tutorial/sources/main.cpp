
#include <SoftPixelEngine.hpp>

using namespace sp;

/**
README:
Do not edit the "android_native_app_glue.c" and "android_native_app_glue.h" files!
These are files provided in the Android NDK. Add these files to all of your
Android Apps using the SoftPixel Engine. The code in these files provides
the interface between C++ and Java.
*/

// In the Android NDK the main function looks always exactly like that:
void android_main(android_app* App)
{
    // Pass the "android_app" object to the device creation.
    SoftPixelDevice* spDevice = createGraphicsDevice(
        App, video::RENDERER_OPENGLES1, "SPE Android Tutorail"
    );
    
    /**
    On Android the input controller can be used to check for some special keys:
    volume +, volume - etc. Or to get the gyrosensor data. You can also check
    the keys like on a normal keyboard when the mobile device supports that.
    */
    io::InputControl* spControl     = spDevice->getInputControl();
    
    // Get the OpenGL|ES 1.1 render system and context.
    video::RenderSystem* spRenderer = spDevice->getRenderSystem();
    video::RenderContext* spContext = spDevice->getRenderContext();
    scene::SceneGraph* spScene      = spDevice->getSceneGraph();
    
    spRenderer->setClearColor(255);
    
    // Create the scene
    scene::Camera* Cam = spScene->createCamera();
    Cam->setPosition(dim::vector3df(0, 0, -3.5f));
    
    scene::Light* Lit = spScene->createLight();
    spScene->setLighting();
    
    /**
    Load the animation model. We disable texture loading state because the texture filename
    in some 3d models are wrong. Thus we load and map the texture manual.
    All resources (e.g. 3d models, textures, sounds etc.) are stored in the "assets" folder
    of your Android project. You always have such a folder when you create a new Android project
    in the "eclipse" IDE. In this tutorial you can also see these assets under:
    "Android-Tutorial" -> "App" -> "assets".
    When loading assets we don't use a path. Just a simple filename. The Android NDK has several
    functions to extract these resources from the APK file which the SoftPixel Engine will use.
    By the way: the APK file is the final Android App compressed to one file (like a ZIP file).
    */
    scene::SceneGraph::setTextureLoadingState(false);
    
    scene::Mesh* Obj = spScene->loadMesh("beast.b3d");
    Obj->setScale(0.01f);
    
    scene::SceneGraph::setTextureLoadingState(false);
    
    Obj->addTexture(spRenderer->loadTexture("beast1.jpg"));
    
    scene::Animation* Anim = Obj->getAnimation();
    
    f32 AnimSpeed = 0.2f;
    
    if (Anim && Anim->getType() == scene::ANIMATION_SKELETAL)
    {
        scene::AnimationSkeletal* BoneAnim = static_cast<scene::AnimationSkeletal*>(Anim);
        BoneAnim->play(scene::ANIM_LOOP);
        BoneAnim->setSpeed(AnimSpeed);
    }
    
    /**
    We disable the volume keys for their default usage. i.e. when the user
    presses the physically keys volume up or down the music volume won't change.
    We want to use these keys for another feature.
    */
    spControl->setKeyEnable(io::KEY_VOLUME_UP, false);
    spControl->setKeyEnable(io::KEY_VOLUME_DOWN, false);
    
    /**
    Here we only update the event. There is no escape key or anything else.
    The loop will be terminated until the user quits the app (e.g. by pressing the 'home-button').
    */
    while (spDevice->updateEvent())
    {
        spRenderer->clearBuffers();
        
        // Update animation speed by the volume keys
        if (spControl->keyHit(io::KEY_VOLUME_UP))
            AnimSpeed += 0.1f;
        if (spControl->keyHit(io::KEY_VOLUME_DOWN))
            AnimSpeed -= 0.1f;
        math::Clamp(AnimSpeed, 0.0f, 1.0f);
        
        /**
        Update touch events. For mobile devices with touch screens you don't have a cursor,
        mouse or something like that. Here we call the touch events "pointer" because
        the user can point to any position on the screen with his fingers.
        */
        if (spControl->getPointerCount() == 1)
        {
            /**
            Functions like "getPointerPosition" or "getMotionSpeed" have one parameter:
            the pointer index. Because you can have several touch events at once.
            */
            const f32 TurnSpeed = spControl->getMotionSpeed(0).X;
            
            Obj->turn(dim::vector3df(0, TurnSpeed, 0));
        }
        
        // Update animations and render the scene
        spScene->updateAnimations();
        spScene->renderScene();
        
        spContext->flipBuffers();
    }
    
    deleteDevice();
}
