

                   Fusion2X - OpenGL ES-CL 1.0 implementation

                   (c) 2008, Markus Trenkwalder a.k.a. Trenki
                             http://www.trenki.net/




Fusion2X is an OpenGL ES Common Lite 1.0 API based layer on top of Trenki's
Software Renderer. The underlying software renderer is very good but requires
the manual implementation of vertex and pixels shaders and thus a great amount
of knowledge and skill in 3D computer graphics. OpenGL is an industry standard
API and many already know how to use it, so getting started may be easier. But
on the other hand it is very unlikely that you will reach the same performance
as you could reach with the raw software renderer.

Fusion2X implements most of the OpenGL ES-CL 1.0 API but omits some 
functionality (at least in this release) and I will probably never make it 100%
standard compliant.

Instead of giving you a list of what is implemented I will give you a list of 
what is not implemented as this will be shorter.

* Two sided lighting. This is rarely used and I don't plan to implement this at
  all although it could be hacked in.

* glReadPixels. Not yet implemented and not really required since you can 
  directly read from the color and depth buffer surfaces you provided to the
  implementation.

* No texture compression. Compressed textures are not supported and all relevant
  entry points will generate the error GL_INVALID_OPERATION.

* No CopyTex{Sub}Image2D support yet.

* No TexSubImage2D support yet.

* glTexImage2D only accepts a restricted set of internalformat and type
  parameters. Specifically no alpha and luminance textures are supported
  currently. GL_RGB textures are supported fully while for GL_RGBA textures only
  GL_UNSIGNED_SHORT_5_5_5_1 and GL_UNSIGNED_BYTE works as type parameter.

* No mipmaping. Only level 0 is supported in the texture functions.

* Only nearest filtering even if you specify GL_LINEAR as the texture filter.

* No glShadeModel support. Specifically GL_FLAT is not supported and when you
  set it it will be ignored.

* No glPolygonOffset support. You can set values but they will be ignored.

* While there is support for the texture matrix there is no support for
  projective texture mapping in this release. Only the resulting s and t texture
  coordinates are considered and r and q are assumed to be 0 and 1 respectivly.
  
* RESCALE_NORMAL and NORMALIZE don't work atm. This is a known bug.

* Performance is very poor in this unoptimized version.

If you find any bugs or want to contribute you can send me your suggestions 
and/or patches to my email address.

Enjoy!
        Markus Trenkwalder                      (trenki2@gmx.net)