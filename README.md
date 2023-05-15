# COMP3015-CW2
## Demo of some OpenGL rendering techniques.

 - YouTube Video: https://youtu.be/jiIIDEEX6-c

## Scene Controls
```
-/+ Exposure: O/P
Move: WASD
Up: Space
Down: Shift
Look around: Mouse
Switch Effects: Left Mouse Button
```
## Explanation

This project demonstrates the following features:
 - __Bloom__ - Bloom is implemented by blitting the scene into a framebuffer with two colour attachments. The first colour attachment contains the whole scene, whereas the second attachment only contains the bright part of the scenes (if the average colour value of a pixel is over a certain threshold). A box blur is applied to the data found in colour attachment two. Using additive blending, the bright (and now blurred) texture is combined with the original scene to create the bloom effect.
 - __Box Blur__ - A box blur is a much simpler blur than a Gaussian blur to implement, and only requires one pass, rather than two. In a box blur the inner pixel of the kernel is equal to the average of the n-surrounding pixels, where n is the radius of the kernel (or strength of the blur).
 - __Perlin Noise w/ Nightvision Effect__ - Perlin noise generation is implemeted using glm::perlin(...). Perlin noise can be created and loaded into OpenGL as a texture which can then be sampled in any shader. In this case the noise is used to create a nightvision effect as shown in the video.
 - Basic Textures