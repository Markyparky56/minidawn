#ifndef _MODEL_H_
#define _MODEL_H_


// INCLUDES //
#include <Windows.h>
#include <fstream>
#include <string>
#include <gl/gl.h>
#include <gl/glu.h>
using namespace std; // This is bad

// MY CLASS INCLUDES //
#include "TokenStream.h" // We don't use this in the working loader
#include <vector>
#include "maths.vector.hpp"
#include "SOIL.h"
#include "Texture.hpp"

template<class T>
struct SortArgs
{
    std::vector<uint32_t>* Indices;
    std::vector<T>* Vec;
    std::vector<float>* OutArray;
};

inline void SortVector3Indices(SortArgs<Vector3>* args);
inline void SortVector2Indices(SortArgs<Vector2>* args);

class Model
{
public:	
	bool Load(char* modelFilename, char* textureFilename);
	void Render();

    inline vector<float>& getVerts()        { return vertices; }
    inline vector<float>& getNorms()        { return normals; }
    inline vector<float>& getTexCoords()    { return texCoords; }
    inline Texture& getTexture()            { return texture; }

private:	
	void LoadTexture(char* filename);
	bool LoadModel(char* filename);
    bool LoadObj(const char* Filename);
	
	int m_vertexCount;
	Texture texture;

	vector<float> vertices, normals, texCoords;
};

#endif