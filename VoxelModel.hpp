#include <stdint.h>

#ifndef VoxelModel_h
#define VoxelModel_h

struct VoxelModel{

  uint64_t xdim;
  uint64_t ydim;
  uint64_t zdim;
  uint8_t* bitmap;

};

int getIndex(const VoxelModel &model, int x, int y, int z);

void getXYZ(const VoxelModel &model, int index, int& x, int& y, int& z);

int getByteNumber(int index);

uint8_t getBitNumber(int index);

bool getBit(VoxelModel model, int x, int y, int z);

void setBit(VoxelModel& model, int x, int y, int z);

void clearBit(VoxelModel& model, int x, int y, int z);

void toggleBit(VoxelModel& model, int x, int y, int z);

//Create a VoxelModel struct and initialize it to be an empty model with appropriate dimensions
VoxelModel allocateModel(int nx, int ny, int nz);

//set all voxels in th model to off
void clearModel(VoxelModel& model);

//set all the voxels in the model to on
void fillModel(VoxelModel& model);

//deallocate all heap memory and set the model's size to be 0, and data to be null.
void deleteModel(VoxelModel& model);

void addSphere (VoxelModel& model,int cx,int cy,int cz,float r);

void subtractSphere (VoxelModel& model,int cx,int cy,int cz,float r);

void toggleSphere (VoxelModel& model,int cx,int cy,int cz,float r);

void addSphere (VoxelModel& model,int cx,int cy,int cz,float r);

#endif
