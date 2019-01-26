#include "ModelHelper.hpp"

#include "Mesh.hpp"
#include "GameObject/GameObject.hpp"
#include "GameObject/ComponentFactory.hpp"
#include "Utils.hpp"

#include <assimp/include/cimport.h>
#include <assimp/include/postprocess.h>
#include <assimp/include/scene.h>

void ModelHelper::init()
{

}

void ModelHelper::finalize()
{
    for (Mesh& mesh : _modelMeshes)
    {
        mesh.cleanUp();
    }
}

bool ModelHelper::loadModel(const std::string& asset)
{
    const auto it = _modelNodes.find(asset);
    if (it != _modelNodes.cend())
    {
        return true; // bail out if model has already been loaded
    }

    const aiScene* loadedScene = aiImportFile(asset.c_str(), aiProcess_Triangulate | aiProcessPreset_TargetRealtime_MaxQuality);
    if (loadedScene == nullptr)
    {
        Utils::log("Level could not be loaded. Path: %s", asset);
        return false;
    }

    _modelNodes[asset] = loadNode(loadedScene, loadedScene->mRootNode, nullptr);

    return true;
}

ModelHelper::Node ModelHelper::loadNode(const aiScene* scene, const aiNode* node, ModelHelper::Node* parent)
{
    Node createdNode;
    
    createdNode.name = node->mName.C_Str();
    if (parent != nullptr)
    {
        createdNode.parent = parent;
    }

     // Node transformations
    aiVector3D scaling;
    aiQuaternion rotation;
    aiVector3D translation;

    node->mTransformation.Decompose(scaling, rotation, translation);

    createdNode.scale = float3(scaling.x, scaling.y, scaling.z);
    createdNode.rotation = Quat(rotation.x, rotation.y, rotation.z, rotation.w);
    createdNode.position = float3(translation.x, translation.y, translation.z);

    for (size_t i = 0; i < node->mNumMeshes; ++i)
    {
        // load materials + texture data here

        createdNode.mesh_ids.push_back(_modelMeshes.size());

        const size_t loadedNodeMeshId = node->mMeshes[i];
        _modelMeshes.emplace_back(std::move(Mesh(scene->mMeshes[loadedNodeMeshId])));
    }

    // Recursively process children nodes
    for (size_t i = 0; i < node->mNumChildren; ++i)
    {
        createdNode.children.push_back(loadNode(scene,  node->mChildren[i], &createdNode));
    }

    return createdNode;
}

GameObject* ModelHelper::getGameObjectFromModel(const std::string& asset)
{
    if (!loadModel(asset))
    {
        Utils::log("Couldn't generate game object from model %s", asset);
        return nullptr;
    }

    const Node& node = _modelNodes.at(asset);
    
    return getGameObjectFromNode(node, nullptr);
}

GameObject* ModelHelper::getGameObjectFromNode(const Node& node, GameObject* parent)
{
    GameObject* gameObject = new GameObject(node.name);

    const float3 pos = node.position;
    const Quat rot = node.rotation;
    const float3 sca = node.scale;

    gameObject->addTransform(ComponentFactory().createComponent<Transform>(pos, rot, sca));

    if (!node.mesh_ids.empty())
    {
        const size_t meshId = node.mesh_ids.at(0);
        Mesh& mesh = _modelMeshes.at(meshId);

        gameObject->addComponent(ComponentFactory().createComponent<MeshComponent>(&mesh));
    }

    for (const Node& child : node.children)
    {
        gameObject->addChild(getGameObjectFromNode(child, gameObject));
    }

    return gameObject;
}