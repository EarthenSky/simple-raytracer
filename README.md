# DEPRECATED
 reimplemented in zig -> /isos_render

# simple-raytracer
 Simple c++ raytracer

I'm following this https://raytracing.github.io/books/RayTracingInOneWeekend.html tutorial to create the raytracer skeleton.

I might add some cool things to this, who knows. Parsing of blender files would be nice, no? Or rendering videos w/ a movable camera! Or connecting it to a renderer like vulkan! (hopefully vulkan is what I think it is...)

# TODO List:
- implement build2 -> https://build2.org/
- 

# Ideas:
- Don't raytrace evenly, instead interpolating colour & raytracing more densely around close objects & objects of interest. Steps: 1. get a realtime renderer looking at a scene with a low definition camera.  2. expand the pixel-definition of the view and allocate a certain number of traces each cycle & distribute them to pixels. (ordered using a weighted quad-tree?) 3. distribute the pixels to parts of the screen based on which objects are near & more detailed.
- Alternately, we can have rays exit early if the rays around them haven't touched any detailed objects.
- Making some psychadelic materials.
- Animated materials!
- Animated scenes?
- Pixelated shaders (like with celeste) or shaders with small objects-fragments in them.
- 
