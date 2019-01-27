#pragma once

#include <vector>
#include "glew-2.0.0/include/GL/glew.h"
#include "MathGeoLib/include/MathGeoLib.h"

class GameObject;
class Material;

struct aiMesh;

struct Weight
{
    std::size_t vertex = 0;
    float weight = 0.f;
};

struct Bone
{
    std::string name;
    Weight* weights = nullptr;
    std::size_t num_weights = 0;
    float4x4 bind;
    const GameObject *attached_to = nullptr;
};

class Mesh
{
  public:
    Mesh(const aiMesh* inMesh);
    ~Mesh();
    Mesh(const Mesh& other);

	void setMaterial(Material* material);
    void draw() const;

    void cleanUp();
    
  private:
    GLuint _numVertices = 0;
    GLuint _numElements = 0;

    float3* _vertices = nullptr;
    float3* _normals = nullptr;

    GLuint _vertexVBO = 0;
    GLuint _texCoordVBO = 0;
    GLuint _normalVBO = 0;
    GLuint _indexVBO = 0;

	Material* _material = nullptr;

    // not used yet
    std::vector<float3> _skinnedVertices;
    std::vector<float3> _skinnedNormals;
    Bone* _boneArray = nullptr;
    std::size_t _numBones = 0;

};