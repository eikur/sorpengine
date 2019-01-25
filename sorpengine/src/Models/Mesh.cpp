#include "Mesh.hpp"

#include <assimp/include/cimport.h>
#include <assimp/include/postprocess.h>
#include <assimp/include/scene.h>

// remove when doing materials
#include "Application.hpp"
#include "TextureHelper.hpp"

Mesh::Mesh(const aiMesh* inMesh)
{
    _numElements = inMesh->mNumFaces * 3;
    _numVertices = inMesh->mNumVertices;

    if (inMesh->HasPositions())
    {
        _vertices = new float3[_numVertices];
        for(std::size_t i = 0; i < _numVertices; ++i)
        {
            const aiVector3D& vertex = inMesh->mVertices[i];
            _vertices[i] = float3(vertex.x, vertex.y, vertex.z);
        }
        
        glGenBuffers(1, &_vertexVBO);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexVBO);
        glBufferData(GL_ARRAY_BUFFER, _numVertices * sizeof(_vertices[0]), &_vertices[0], GL_DYNAMIC_DRAW);   

        // not freeing _vertices to be able to skin meshes
    }

    if (inMesh->HasTextureCoords(0))
    {
        float2* textureCoords = new float2[_numVertices];
        for (std::size_t i = 0; i < _numVertices; ++i)
        {
            const aiVector3D& texCoord = inMesh->mTextureCoords[0][i];
            textureCoords[i] = float2(texCoord.x, texCoord.y);
        }

        glGenBuffers(1, &_texCoordVBO);
        glBindBuffer(GL_ARRAY_BUFFER, _texCoordVBO);
        glBufferData(GL_ARRAY_BUFFER, _numVertices * sizeof(textureCoords[0]), &textureCoords[0], GL_STATIC_DRAW);

        delete[] textureCoords;
    }

    if (inMesh->HasNormals()) 
    {
        _normals = new float3[_numVertices];
        for (size_t i = 0; i < inMesh->mNumVertices; ++i) 
        {
            const aiVector3D& normal = inMesh->mNormals[i];
            _normals[i] = float3(normal.x, normal.y, normal.z);
        }

        glGenBuffers(1, &_normalVBO);
        glBindBuffer(GL_ARRAY_BUFFER, _normalVBO);
        glBufferData(GL_ARRAY_BUFFER, _numVertices * sizeof(_normals[0]), &_normals[0], GL_DYNAMIC_DRAW);

        // not freeing _normals to be able to skin meshes
    }

    if (inMesh->HasFaces()) 
    {
        const GLuint numIndices = inMesh->mNumFaces * inMesh->mFaces[0].mNumIndices;
        GLuint* indices = new GLuint[numIndices];
        for (size_t i = 0; i < inMesh->mNumFaces; ++i) {
            for (size_t j = 0; j < inMesh->mFaces[i].mNumIndices; ++j) {
                indices[inMesh->mFaces[i].mNumIndices * i + j] = inMesh->mFaces[i].mIndices[j];
            }
        }

        glGenBuffers(1, &_indexVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexVBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices  * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

        delete[] indices;
    }


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Mesh::~Mesh()
{
    delete _vertices;
    delete[] _normals;
}

void Mesh::cleanUp()
{
    glDeleteBuffers(1, &_vertexVBO);
    glDeleteBuffers(1, &_texCoordVBO);
    glDeleteBuffers(1, &_normalVBO);
    glDeleteBuffers(1, &_indexVBO);
}

void Mesh::draw() const
{
    // bind proper texture when drawing, this is default texture for images
    App->getTextureHelper().useTexture(1);
    
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(1.f, 1.f, 1.f, 1.f);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glColor4f(1.f, 0.f, 0.f, 1.f);

    if (_vertexVBO) {
        glBindBuffer(GL_ARRAY_BUFFER, _vertexVBO);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, 0);
    }

    if (_texCoordVBO) {
        glBindBuffer(GL_ARRAY_BUFFER, _texCoordVBO);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, 0);
    }

    if (_normalVBO) {
        glBindBuffer(GL_ARRAY_BUFFER, _normalVBO);
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, 0);
    }

    if (_indexVBO) 
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexVBO);
        glDrawElements(GL_TRIANGLES, _numElements, GL_UNSIGNED_INT, NULL);
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    // unbind texture
    App->getTextureHelper().stopUsingTexture();
}

Mesh::Mesh(const Mesh& other)
{
    _numVertices = other._numVertices;
    _numElements = other._numElements;

    _vertexVBO = other._vertexVBO;
    _texCoordVBO = other._texCoordVBO;
    _normalVBO = other._normalVBO;
    _indexVBO = other._indexVBO;

    delete[] _vertices;
    _vertices = new float3[_numVertices];
    for (size_t i = 0; i < _numVertices; ++i)
    {
        _vertices[i] = other._vertices[i];
    }

    delete[] _normals;
    _normals = new float3[_numVertices];
    for (size_t i = 0; i < _numVertices; ++i)
    {
        _normals[i] = other._normals[i];
    }
}