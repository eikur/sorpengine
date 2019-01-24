#include "ModelHelper.hpp"

#include "Utils.hpp"
#include <assimp/include/cimport.h>
#include <assimp/include/postprocess.h>
#include <assimp/include/scene.h>

void ModelHelper::init()
{

}

void ModelHelper::finalize()
{

}

void ModelHelper::loadModel(const std::string& modelPath)
{
    const auto it = _modelNodes.find(modelPath);
    if (it != _modelNodes.cend())
    {
        return; // model has already been loaded
    }

    const aiScene* loadedScene = aiImportFile(modelPath.c_str(), aiProcess_Triangulate | aiProcessPreset_TargetRealtime_MaxQuality);
    if (loadedScene == nullptr)
    {
        Utils::log("Level could not be loaded. Path: %s", modelPath);
        return;
    }

    _modelNodes[modelPath] = loadNode(modelPath.c_str(), loadedScene->mRootNode, nullptr);
}

ModelHelper::Node ModelHelper::loadNode(const char* asset_path, const aiNode* node, ModelHelper::Node* parent)
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

    // load mesh data + materials

    // Recursively process children nodes
    for (size_t i = 0; i < node->mNumChildren; ++i)
    {
        createdNode.children.push_back(loadNode(asset_path, node->mChildren[i], &createdNode));
    }

    return createdNode;
}


/*
GameObject* getGameObjectFromModel(const std::string& modelPath){
loadModel(); // maybe tryLoadModel()... we'll see 

}
*/