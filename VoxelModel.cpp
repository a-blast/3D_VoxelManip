#include <stdint.h>
#include <cmath>
#include <iostream>
#include "VoxelModel.hpp"

int getIndex(const VoxelModel& model, int x, int y, int z){

  int bitIndex {0};
  int arrayIndex {0};

  if (x%2==1)
         bitIndex +=4;
  arrayIndex += x/2;
  // std::cout <<int(arrayIndex)<<"---\n";

  if (y%2==1)
         bitIndex +=2;
  arrayIndex += (y/2)*((model.xdim/2) + model.xdim%2);
  //std::cout <<int(arrayIndex)<<"---\n";

  if (z%2==1)
         bitIndex +=1;
  arrayIndex += (z/2)*((model.xdim/2) + model.xdim%2)*((model.ydim/2) + model.ydim%2);

  //std::cout <<"aI: " << arrayIndex<<" bI: "<< bitIndex<< "\n";

  return (bitIndex+(arrayIndex*8));
}

void getXYZ(const VoxelModel& model, int index, int& x, int& y, int& z){

  int arrayIndex = getByteNumber(index);
  int bitIndex = getByteNumber(index);

  //Identify the position in the 2x2x2 voxel
  x = bitIndex/4;
  y = (bitIndex%4)/2;
  z = ((bitIndex%4)%2);

  //Identify the 2x2x2 voxel
  //arrayIndex equals number of voxels into the model
  //int totalVoxels=(model.xdim)*(model.ydim)*(model.zdim);
  int zvoxels= 1+((arrayIndex-1)/(model.ydim*model.xdim));
  int yvoxels= 1+(((arrayIndex%(model.ydim*model.xdim))-1)/model.xdim);
  int xvoxels= (arrayIndex%(model.ydim*model.xdim))%model.xdim;

  //co-ord is equal to the # of voxels on that axis *2 (2 bit length per voxel) plus either 1 or zero
  x=((xvoxels*2)+x);
  y=((yvoxels*2)+y);
  z=((zvoxels*2)+z);
}

int getByteNumber(int index){
  return index/8;
}

uint8_t getBitNumber(int index){
  return index%8;
}

//update(d)
bool getBit(VoxelModel model, int x, int y, int z){

  int index = getIndex(model, x, y, z);
  uint8_t byteVal = model.bitmap[getByteNumber(index)];
  uint8_t bitToCheck {1};

  return (1 == ((byteVal >> getBitNumber(index) & bitToCheck)));
}

//update(d)
void setBit(VoxelModel& model, int x, int y, int z){

  int index = getIndex(model, x,y,z);
  //std::cout << "INDEX OF SET==" << index << "\n";
  uint64_t bitToCheck {1};
  //std::cout << getByteNumber(index) << ", "<< int(getBitNumber(index)) <<"\n";
  model.bitmap[getByteNumber(index)] |= bitToCheck << int(getBitNumber(index));
}

//update(d)
void clearBit(VoxelModel& model, int x, int y, int z){

  int index = getIndex(model,x,y,z);
  uint64_t bitToCheck {1};

  model.bitmap[getByteNumber(index)] &= ~(bitToCheck << getBitNumber(index));
}

//update(d)
void toggleBit(VoxelModel& model, int x, int y, int z){

  int index = getIndex(model,x,y,z);
  uint64_t bitToCheck {1};

  model.bitmap[getByteNumber(index)] ^= (bitToCheck << getBitNumber(index));
}

//Create a VoxelModel struct and initialize it to be an empty model with appropriate dimensions
VoxelModel allocateModel(int nx, int ny, int nz){


//uint8_t can hold 2x2x2 voxel, so dimensions need to be a multiple of 2.
  if (nx%2 == 1)
    nx++;
  if (ny%2 == 1)
    ny++;
  if (ny%2 == 1)
    nz++;

  uint64_t x {static_cast<uint64_t>(nx)};
  uint64_t y {static_cast<uint64_t>(ny)};
  uint64_t z {static_cast<uint64_t>(nz)};

  uint8_t* temp = new uint8_t[((nx/2)*(ny/2)*(nz/2))]{};

  VoxelModel* vm = new VoxelModel {x,y,z,temp};

  return *(vm);

}

//set all voxels in th model to off
void clearModel(VoxelModel& model){

  delete[] model.bitmap;
  model.bitmap = new uint8_t[((model.xdim/2)*(model.ydim/2)*(model.zdim/2))]();

}

//set all the voxels in the model to on
void fillModel(VoxelModel& model){

  for (uint64_t i =0; i <((model.xdim)*(model.ydim)*(model.zdim)); i++){
    model.bitmap[i]= -1;
  } 
}

//deallocate all heap memory and set the model's size to be 0, and data to be null.
void deleteModel(VoxelModel& model){

  delete &model;
}

void addSphere (VoxelModel& model,int cx,int cy,int cz,float r){

  for (int64_t z =0; z<model.zdim; ++z){
    for (int64_t y=0; y<model.ydim; ++y){
      for (int64_t x=0; x<model.xdim; ++x){
        if (x<=((sqrt(pow(r,2)-pow(y-cy,2)-pow(z-cz,2))+cx))&&
            (x>=(-sqrt(pow(r,2)-pow(y-cy,2)-pow(z-cz,2))+cx))){
          setBit(model, x, y, z);
        }
      }
    }
  }
}

void subtractSphere (VoxelModel& model,int cx,int cy,int cz,float r){

  for (int64_t z =0; z<model.zdim; ++z){
    for (int64_t y=0; y<model.ydim; ++y){
      for (int64_t x=0; x<model.xdim; ++x){
        if (x<=((sqrt(pow(r,2)-pow(y-cy,2)-pow(z-cz,2))+cx))&&
            (x>=(-sqrt(pow(r,2)-pow(y-cy,2)-pow(z-cz,2))+cx))){
          clearBit(model, x, y, z);
        }
      }
    }
  }
}

void toggleSphere (VoxelModel& model,int cx,int cy,int cz,float r){

  for (int64_t z =0; z<model.zdim; ++z){
    for (int64_t y=0; y<model.ydim; ++y){
      for (int64_t x=0; x<model.xdim; ++x){
        if (x<=((sqrt(pow(r,2)-pow(y-cy,2)-pow(z-cz,2))+cx))&&
            (x>=(-sqrt(pow(r,2)-pow(y-cy,2)-pow(z-cz,2))+cx))){
          toggleBit(model, x, y, z);
        }
      }
    }
  }
}




