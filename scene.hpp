#ifndef  _SCENE_LIGHT
#define _SCENE_LIGHT

#include "point and vector.hpp"
#include <vector>

struct Light {

  vec3 position, color;
  Light(vec3 pos, vec3 rgb_Color) : position(pos), color(rgb_Color) {}

};

struct Scene {

  vec3 ambient_Color;
  std::vector<Light> light_Sources;
  
  Scene(vec3 rgb_Color, std::vector<Light>& light_Src) : ambient_Color(rgb_Color), light_Sources(light_Src) {}
};

#endif // ! _SCENE_LIGHT

