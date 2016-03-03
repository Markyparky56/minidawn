#include "model.h"
#include <thread>
#include <memory>
#include <functional>

using pThread = std::unique_ptr<std::thread>;

bool Model::Load(char* modelFilename, char* textureFilename)
{
	bool result;

	// Load in the model data,
	result = LoadObj(modelFilename);
	if(!result)
	{
		MessageBox(NULL, L"Model failed to load", L"Error", MB_OK);
		return false;
	}

	// Load the texture for this model.
	LoadTexture(textureFilename);
	
	return true;
}

void Model::Render()
{
	// You must insert code here to render your model
	// This function will be called from Scene3D.render()
}

bool Model::LoadModel(char* filename)
{
	std::ifstream fileStream;
    int fileSize = 0;

    fileStream.open( filename, std::ifstream::in );
   
    if( fileStream.is_open() == false )
        return false;

    fileStream.seekg( 0, std::ios::end );
    fileSize = ( int )fileStream.tellg( );
    fileStream.seekg( 0, std::ios::beg );

    if (fileSize <= 0)
    {
        return false;
    }

    char *buffer = new char[fileSize];

    if (buffer == 0)
    {
        return false;
    }

    memset( buffer, '\0', fileSize );

    TokenStream tokenStream, lineStream, faceStream;
    string tempLine, token;

    fileStream.read( buffer, fileSize );
    tokenStream.SetTokenStream( buffer );

    delete[] buffer;

    tokenStream.ResetStream( );

	float tempx, tempy, tempz;
    vector<Vector3> verts, norms;
    vector<Vector2> texC;
    vector<int> faces;
	

    char lineDelimiters[2] = { '\n', ' ' };

    while( tokenStream.MoveToNextLine( &tempLine ) )
    {
        lineStream.SetTokenStream( ( char* )tempLine.c_str( ) );
        tokenStream.GetNextToken( 0, 0, 0 );

        if( !lineStream.GetNextToken( &token, lineDelimiters, 2 ) )
            continue;

        if( strcmp( token.c_str( ), "v" ) == 0 )
        {
            lineStream.GetNextToken( &token, lineDelimiters, 2 );
			tempx = (float)atof(token.c_str());

            lineStream.GetNextToken( &token, lineDelimiters, 2 );
			tempy = (float)atof(token.c_str());

            lineStream.GetNextToken( &token, lineDelimiters, 2 );
			tempz = (float)atof(token.c_str());

			verts.push_back(Vector3(tempx, tempy, tempz));
        }
        else if( strcmp( token.c_str( ), "vn" ) == 0 )
        {
            lineStream.GetNextToken( &token, lineDelimiters, 2 );
			tempx = (float)atof(token.c_str());

            lineStream.GetNextToken( &token, lineDelimiters, 2 );
			tempy = (float)atof(token.c_str());

            lineStream.GetNextToken( &token, lineDelimiters, 2 );
			tempz = (float)atof(token.c_str());

			norms.push_back(Vector3(tempx, tempy, tempz));
        }
        else if( strcmp( token.c_str( ), "vt" ) == 0 )
        {
            lineStream.GetNextToken( &token, lineDelimiters, 2 );
			tempx = (float)atof(token.c_str());

            lineStream.GetNextToken( &token, lineDelimiters, 2 );
			tempy = (float)atof(token.c_str());

			texC.push_back(Vector2(tempx, tempy));
        }
        else if( strcmp( token.c_str( ), "f" ) == 0 )
        {
            char faceTokens[3] = { '\n', ' ', '/' };
            std::string faceIndex;

            faceStream.SetTokenStream( ( char* )tempLine.c_str( ) );
            faceStream.GetNextToken( 0, 0, 0 );

            for( int i = 0; i < 3; i++ )
            {
                faceStream.GetNextToken( &faceIndex, faceTokens, 3 );
                faces.push_back( ( int )atoi( faceIndex.c_str( ) ) );

                faceStream.GetNextToken( &faceIndex, faceTokens, 3 );
                faces.push_back( ( int )atoi( faceIndex.c_str( ) ) );

                faceStream.GetNextToken( &faceIndex, faceTokens, 3 );
                faces.push_back( ( int )atoi( faceIndex.c_str( ) ) );
            }
        }
        else if( strcmp( token.c_str( ), "#" ) == 0 )
        {
           //skip
        }

        token[0] = '\0';
    }

    // "Unroll" the loaded obj information into a list of triangles.
	
    int numFaces = ( int )faces.size( ) / 9;
	m_vertexCount = numFaces * 3;
	
    // You must add code here to sort the model data
	// Model data is stored in vectors verts, norms, texC and faces
	// Sort through the data and store it in the vectors provided (see header file)
	for (int i = 0; i < numFaces; i++)
    {
        for (int i = 0; i < 3; i++)
        {
            vertices.push_back(verts[ faces[numFaces + i] - 1 ].x);
            vertices.push_back(verts[ faces[numFaces + i] - 1 ].y);
            vertices.push_back(verts[ faces[numFaces + i] - 1 ].z);

            normals.push_back(norms[ faces[numFaces + i] - 1 ].x);
            normals.push_back(norms[ faces[numFaces + i] - 1 ].y);
            normals.push_back(norms[ faces[numFaces + i] - 1 ].z);

            texCoords.push_back(texC[ faces[numFaces + i] - 1 ].x);
            texCoords.push_back(texC[ faces[numFaces + i] - 1 ].y);
        }        
    }

	// Your code must end here, prior to the loaded data being deleted
    
    verts.clear( );
    norms.clear( );
    texC.clear( );
    faces.clear( );

    return true;
}

void Model::LoadTexture(char* filename)
{	
	texture.setTexture(SOIL_load_OGL_texture
	(
		filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_INVERT_Y
	));

	//check for an error during the load process
	if( texture.getTexture() == 0 )
	{
		printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
	}
}

// I followed the tutorial here: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
// It's so much easier to understand whats going on in it
bool Model::LoadObj(const char* Filename)
{
    std::vector<Vector3> verts;
    std::vector<Vector3> norms;
    std::vector<Vector2> texCs;
    std::vector<uint32_t> vertIndices, normalIndices, uvIndices;

    FILE* file = fopen(Filename, "r");
    if (file == NULL)
    {
        return false;
    }
    while (true)
    {
        char lineHeader[128];

        // Read first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
        {
            break; // exit loop
        }
        else // Parse
        {
            if (strcmp(lineHeader, "v") == 0) // Vertex
            {
                Vector3 vertex;
                fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
                verts.push_back(vertex);
            }
            else if (strcmp(lineHeader, "vt") == 0) // Tex Coord
            {
                Vector2 uv;
                fscanf(file, "%f %f\n", &uv.x, &uv.y);
                texCs.push_back(uv);
            }
            else if (strcmp(lineHeader, "vn") == 0) // Normal
            {
                Vector3 normal;
                fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
                norms.push_back(normal);
            }
            else if (strcmp(lineHeader, "f") == 0) // Face
            {
                std::string vert1, vert2, vert3;
                unsigned int vertIndex[3], uvIndex[3], normIndex[3];
                int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertIndex[0], &uvIndex[0], &normIndex[0],
                                                                           &vertIndex[1], &uvIndex[1], &normIndex[1],
                                                                           &vertIndex[2], &uvIndex[2], &normIndex[2]);
                if (matches != 9)
                {
                    // confuzzled parser
                    return false;
                }
                for (auto vertI : vertIndex)    { vertIndices.push_back(vertI); }
                for (auto uvI : uvIndex)        { uvIndices.push_back(uvI); }
                for (auto normI : normIndex)    { normalIndices.push_back(normI); }
            }
        }
    }
    // Sort Vector Arrays
    // Multithreaded 'cause reasons
    SortArgs<Vector3> vertexArgs;
    vertexArgs.Indices = &vertIndices;
    vertexArgs.Vec = &verts;
    vertexArgs.OutArray = &vertices;

    SortArgs<Vector2> uvArgs;
    uvArgs.Indices = &uvIndices;
    uvArgs.Vec = &texCs;
    uvArgs.OutArray = &texCoords;

    SortArgs<Vector3> normalArgs; // As opposed to paranormalArgs and supernaturalArgs
    normalArgs.Indices = &normalIndices;
    normalArgs.Vec = &norms;
    normalArgs.OutArray = &normals;

    pThread vertexThread(new std::thread(SortVector3Indices, &vertexArgs));
    pThread normalThread(new std::thread(SortVector3Indices, &normalArgs));
    pThread uvThread(new std::thread(SortVector2Indices, &uvArgs));
    vertexThread->join();
    normalThread->join();
    uvThread->join();
    return true;
}

// To further improve this for threading add a "range" to sort and use a farm
void SortVector3Indices(SortArgs<Vector3>* args)
{
    auto& indices = *(args->Indices);
    auto& vec = *(args->Vec);
    for (int i = 0; i < indices.size(); i++)
    {
        uint32_t index = indices[i];
        Vector3 v = vec[index - 1];
        args->OutArray->push_back(v.x);
        args->OutArray->push_back(v.y);
        args->OutArray->push_back(v.z);
    }
}

void SortVector2Indices(SortArgs<Vector2>* args)
{
    auto& indices = *(args->Indices);
    auto& vec = *(args->Vec);
    for (int i = 0; i < indices.size(); i++)
    {
        uint32_t index = indices[i];
        Vector2 v = vec[index - 1];
        args->OutArray->push_back(v.x);
        args->OutArray->push_back(v.y);
    }
}


