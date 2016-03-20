#ifndef _MODEL_H_
#define _MODEL_H_


// INCLUDES //
#include <Windows.h> // Ew
#include <fstream>
#include <string>
#include <vector>
#include <gl/gl.h> // Eck
#include <gl/glu.h>

using std::vector;
using std::string;

// MY CLASS INCLUDES //
#include "TokenStream.h" // We don't use this in the working loader
#include <vector>
#include "maths.vector.hpp"
#include "SOIL.h"
#include "VertexObject.hpp"

template<class T>
struct SortArgs
{
    std::vector<uint32_t>* Indices;
    std::vector<T>* Vec;
    std::vector<float>* OutArray;
};

inline void SortVector3Indices(SortArgs<Vector3>* args);
inline void SortVector2Indices(SortArgs<Vector2>* args);

class Model : public VertexObject
{
public:	
    Model()
        : VertexObject(ObjectType::Model)
    {}
	bool Load(char* modelFilename, char* textureFilename);

private:	
	void LoadTexture(char* filename);
	bool LoadModel(char* filename);
    bool LoadObj(const char* Filename);
	
	int m_vertexCount;

	/*vector<float> vertices, normals, texCoords;*/
};

#endif