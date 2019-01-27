#pragma once

#include "TextureHelper.hpp"
#include "MathGeoLib\include\MathGeoLib.h"
#include "Material.hpp"
#include "Mesh.hpp"

#include <map>
#include <vector>

class GameObject;
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
        std::vector<size_t> meshIds;
		std::vector<size_t> materialIds;
    };

    using MNameToNodeStructure = std::map<std::string, Node>;

  public:
	ModelHelper(TextureHelper& textureHelper);

	void init() {};
    bool loadModel(const std::string& asset);
    GameObject* getGameObjectFromModel(const std::string& asset);

    void finalize();

private:
    Node loadNode(const std::string& assetPath, const aiScene* scene, const aiNode* node, Node* parent);
    GameObject* getGameObjectFromNode(const Node& node, GameObject* parent);

	TextureHelper& _textureHelper;

    MNameToNodeStructure _modelNodes;
    std::vector<Mesh> _modelMeshes;
    std::vector<Material> _modelMaterials;
};