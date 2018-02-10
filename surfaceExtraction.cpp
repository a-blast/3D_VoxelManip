
#include <cstdint>
#include <iostream>
#include "surfaceExtraction.hpp"
#include "VoxelModel.hpp"


void writeSTL(VoxelModel model, const char* filename){

  FILE* stl = std::fopen(filename, "w");

  uint8_t header {0};

  if (std::fwrite(&header, sizeof(uint8_t), 80, stl) !=80){
    std::cerr << "Failed fwrite header\n";
  }

  uint32_t numTri {0};
  Triangle* newTri = new Triangle[2];
  
  if (std::fwrite(&numTri, sizeof(uint32_t), 1, stl) !=1){
    std::cerr << "Failed fwrite numtri\n";
  }

  for (int z=0; z<model.zdim; z++){
    for(int y=0; y<model.ydim; y++){
      for(int x=0; x<model.xdim; x++){
        if (getBit(model, x, y, z)) {

          if ((x == 0) || !(getBit(model, x-1, y, z))){

          writeTriangles(NX, stl, newTri, x, y, z);	
          numTri=numTri+2;
          }
          if ((x == model.xdim-1) || !(getBit(model, x+1, y, z))){

                writeTriangles(PX, stl, newTri, x, y, z);	
          numTri=numTri+2;
          }
          if ((y == 0) || !(getBit(model, x, y-1, z))){

                writeTriangles(NY, stl, newTri, x, y, z);	
          numTri=numTri+2;
          }
          if ((y == model.ydim-1) || !(getBit(model, x, y+1, z))){

                writeTriangles(PY, stl, newTri, x, y, z);	
          numTri=numTri+2;	
          }
          if ((z == 0) || !(getBit(model, x, y, z-1))){

                writeTriangles(NZ, stl, newTri, x, y, z);	
          numTri=numTri+2;		
          }
          if ((z == model.zdim-1) || !(getBit(model, x, y, z+1))){

                writeTriangles(PZ, stl, newTri, x, y, z);	
          numTri=numTri+2;      
          } 
        }
      }		
    }
  }

  if ((std::fseek(stl, sizeof(uint8_t)*80, SEEK_SET)) != 0){
    std::cerr << "Failed seek\n";
    return;
  }
  
  if (std::fwrite(&numTri, sizeof(uint32_t), 1, stl) !=1){
    std::cerr << "Failed fwrite numtri\n";
  }

  
}

void writeTriangles(FaceType face, FILE* stl, Triangle* newTri, int x, int y, int z){
  
  		extractFace(x, y, z, face, newTri[0], newTri[1]);
		uint16_t gap {0};		
  
		if (std::fwrite(&newTri[0], sizeof(Triangle), 1, stl) !=1){
		  std::cerr << "\nFailed fwrite of tri\n";
	       	}
	 	if (std::fwrite(&gap, sizeof(uint16_t), 1, stl) !=1){
		  std::cerr << "\nFailed fwrite of tri\n";
	       	}
	      	if (std::fwrite(&newTri[1], sizeof(Triangle), 1, stl) !=1){
		  std::cerr << "\nFailed fwrite of tri\n";
	       	}
	 	if (std::fwrite(&gap, sizeof(uint16_t), 1, stl) !=1){
		  std::cerr << "\nFailed fwrite of tri\n";
	       	}	
}

inline void fillPlane(int a1, int a2, int b1, int b2, int c, int cInd, Triangle& t1, Triangle& t2){
    t1.v1[cInd] = c;
    t2.v1[cInd] = c;
    t1.v2[cInd] = c;
    t2.v2[cInd] = c;
    t1.v3[cInd] = c;
    t2.v3[cInd] = c;
    int aInd = (cInd +1) % 3;
    int bInd = (cInd +2) % 3;
    
    t1.v1[aInd] = a1;
    t1.v2[aInd] = a2;
    t1.v3[aInd] = a2;
    
    t2.v1[aInd] = a1;
    t2.v2[aInd] = a2;
    t2.v3[aInd] = a1;
    
    t1.v1[bInd] = b1;
    t1.v2[bInd] = b1;
    t1.v3[bInd] = b2;
    
    t2.v1[bInd] = b1;
    t2.v2[bInd] = b2;
    t2.v3[bInd] = b2;
    
}

void extractFace(int x, int y, int z, FaceType face, Triangle& t1, Triangle& t2){
    for(int i= 0; i < 3; i++){
        t1.normal[i] = 0;
        t2.normal[i] = 0;
    }
    switch(face){
        case NX:
            t1.normal[0] = -1;
            t2.normal[0] = -1;
            fillPlane(y + 1, y, z, z+1, x, 0, t1, t2);
            break;
        case NY:
            t1.normal[1] = -1;
            t2.normal[1] = -1;
            fillPlane(z + 1, z, x, x+1, y, 1, t1, t2);
            break;
        case NZ:
            t1.normal[2] = -1;
            t2.normal[2] = -1;
            fillPlane(x + 1, x, y, y + 1, z, 2, t1, t2);
            break;
        case PX:
            t1.normal[0] = 1;
            t2.normal[0] = 1;
            fillPlane(y, y + 1, z, z +1, x + 1, 0, t1, t2);
            break;
        case PY:
            t1.normal[1] = 1;
            t2.normal[1] = 1;
            fillPlane(z, z + 1, x, x + 1, y + 1, 1, t1, t2);
            break;
        case PZ:
            t1.normal[2] = 1;
            t2.normal[2] = 1;
            fillPlane(x, x +1, y, y + 1, z + 1, 2, t1, t2);
            break;
    }
}

