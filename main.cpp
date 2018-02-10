#include <stdint.h>
#include <iostream>
#include <cmath>
#include "VoxelModel.hpp"
#include "surfaceExtraction.hpp"

int main(){

  VoxelModel* one=  new VoxelModel;
  *(one) = allocateModel (200,200,200);

  addSphere(*one, 100,100,100,100);
  subtractSphere(*one, 0,0,0,85);

  writeSTL(*one, "\"That's no moon..\".stl");

  deleteModel (*one);
  one =nullptr;
}
