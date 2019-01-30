#pragma once

#include "glew-2.0.0/include/GL/glew.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include <map>
#include <vector>


class GameObject;
class Material;

struct aiMesh;

struct Weight
{
    size_t vertex = 0;
    float weight = 0.f;
};

struct Bone
{
    std::string name;
    float4x4 bind;
    std::vector<Weight> weights;
    const GameObject *target = nullptr;

    Bone() {}
    Bone(const Bone& other)
    {
        bind = other.bind;
        name = other.name;
        target = other.target;

        weights.clear();
        for (const Weight& w : other.weights)
        {
            weights.push_back(w);
        }
    }
};

class Mesh
{
  public:
    Mesh(const aiMesh* inMesh);
    ~Mesh();
    Mesh(const Mesh& other);

	void setMaterial(Material* material);
    void draw() const;

    std::vector<std::string> getBoneNames() const;

    bool canBeSkinned() const;
    bool isSkinned() const;
    void skin(const std::map<int, GameObject*>& boneTargets);
    void updateSkin();

    void cleanUp();
    
  private:
    GLuint _vertexCount = 0;
    GLuint _elementCount = 0;

    float3* _vertices = nullptr;
    float3* _normals = nullptr;

    GLuint _vertexVBO = 0;
    GLuint _texCoordVBO = 0;
    GLuint _normalVBO = 0;
    GLuint _indexVBO = 0;

    GLuint _boneCount = 0;
    Bone* _bones = nullptr;
    bool _skinningSet = false;
    
    float3* _verticesSkin = nullptr;
    float3* _normalsSkin = nullptr;


    Material* _material = nullptr;
};