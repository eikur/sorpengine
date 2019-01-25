#pragma once

#include "MathGeoLib\include\MathGeoLib.h"
#include "Mesh.hpp"

#include <map>
#include <vector>

struct aiScene;
struct aiNode;

class ModelHelper
{
    struct Node
    {
        std::string name;
        Node* parent = nullptr;

        float3 position = float3::zero;
        Quat rotation = Quat::identity;
        float3 scale = float3::one;

        std::vector<Node> children;
        std::vector<size_t> mesh_ids;
    };

    using MNameToNodeStructure = std::map<std::string, Node>;

  public:
    void init();
    void loadModel(const std::string& modelPath);
    void finalize();

private:
    Node loadNode(const char* asset_path, const aiScene* scene, const aiNode* node, Node* parent);

    MNameToNodeStructure _modelNodes;
    std::vector<Mesh> _modelMeshes;
    //std::vector<Material> _modelMaterials;
};