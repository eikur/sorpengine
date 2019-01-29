#pragma once

#include "ModelHelper.fwd.hpp"

#include "TextureHelper.hpp"
#include "Material.hpp"
#include "Mesh.hpp"

#include <map>

class GameObject;
struct aiScene;
struct aiNode;

class ModelHelper
{
    using ModelNodeRepository = std::map<std::string, Node>;
    using MeshRepository = std::map<size_t, Mesh>;
    using MaterialRepository = std::map<size_t, Material>;

    using AnimationRepository = std::map<size_t, Animation>;

  public:
	ModelHelper(TextureHelper& textureHelper);

	void init() {};
    bool loadModel(const std::string& asset);
    void loadAnimationsForModel(const std::string& model, const std::string& animationPath);

    GameObject* getGameObjectFromModel(const std::string& asset);

    void finalize();

private:
    Node loadNode(const std::string& assetPath, const aiScene* scene, const aiNode* node, Node* parent);
    void loadMaterial(const std::string& assetPath, const aiScene* scene, const aiMesh* mesh, Node& targetNode);
    GameObject* getGameObjectFromNode(const Node& node, GameObject* parent);


	TextureHelper& _textureHelper;

    ModelNodeRepository _modelNodes;
    MeshRepository _modelMeshes;
    MaterialRepository _modelMaterials;

    AnimationRepository _animations;
//    std::vector<AnimationInstance> _animationInstances; // this will be passed to the GO 
};