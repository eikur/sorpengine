#include "ModelHelper.hpp"

#include "Mesh.hpp"
#include "GameObject/GameObject.hpp"
#include "GameObject/ComponentFactory.hpp"
#include "Utils.hpp"

#include <assimp/include/cimport.h>
#include <assimp/include/postprocess.h>
#include <assimp/include/scene.h>

ModelHelper::ModelHelper(TextureHelper& textureHelper) : _textureHelper(textureHelper)
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
		Utils::log("Error: %s", aiGetErrorString());
        return false;
    }

	const size_t pathEndPos = asset.find_last_of('/');
	const std::string path = asset.substr(0, pathEndPos + 1);
    _modelNodes[asset] = loadNode(path, loadedScene, loadedScene->mRootNode, nullptr);

    return true;
}

ModelHelper::Node ModelHelper::loadNode(const std::string& assetPath, const aiScene* scene, const aiNode* node, ModelHelper::Node* parent)
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
		const size_t meshId = node->mMeshes[i];
		const aiMesh* mesh = scene->mMeshes[meshId];

		const size_t materialId = mesh->mMaterialIndex;
		const aiMaterial* material = scene->mMaterials[materialId];

		// get material texture
		GLuint materialTextureId = 0;
		const bool materialHasTexture = material->GetTextureCount(aiTextureType_DIFFUSE);
		if (materialHasTexture)
		{
			aiString textureFile;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFile) == AI_SUCCESS)
			{
				materialTextureId = _textureHelper.loadTexture(assetPath + textureFile.C_Str());
			}
		}

		createdNode.materialIds.push_back(_modelMaterials.size());
		_modelMaterials.emplace_back(Material(materialTextureId));

		// load material colors
		Material& loadedMaterial = _modelMaterials.back();
		aiColor4D color;

		if (material->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
		{
			loadedMaterial.setColor({ color.r, color.g, color.b, 1.0f }, Material::ColorComponent::Ambient);
		}
		if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
		{
			loadedMaterial.setColor({ color.r, color.g, color.b, 1.0f }, Material::ColorComponent::Diffuse);
		}
		if (material->Get(AI_MATKEY_COLOR_EMISSIVE, color) == AI_SUCCESS)
		{
			loadedMaterial.setColor({ color.r, color.g, color.b, 1.0f }, Material::ColorComponent::Emissive);
		}
		if (material->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
		{
			loadedMaterial.setColor({ color.r, color.g, color.b, 1.0f }, Material::ColorComponent::Specular);
		}
		if (material->Get(AI_MATKEY_COLOR_TRANSPARENT, color) == AI_SUCCESS)
		{
			loadedMaterial.setColor({ color.r, color.g, color.b, 1.0f }, Material::ColorComponent::Transparent);
		}

        createdNode.meshIds.push_back(_modelMeshes.size());

        _modelMeshes.emplace_back(Mesh(mesh));
    }

    // Recursively process children nodes
    for (size_t i = 0; i < node->mNumChildren; ++i)
    {
        createdNode.children.push_back(loadNode(assetPath, scene,  node->mChildren[i], &createdNode));
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

    if (!node.meshIds.empty())
    {
        for (size_t i = 0; i < node.meshIds.size(); ++i)
        {
            const bool addMeshToChildrenGameObject = i > 0;
            GameObject* targetGameObject = gameObject;
            if (addMeshToChildrenGameObject)
            {
                targetGameObject = new GameObject(gameObject->getName() + std::to_string(i), gameObject);
                targetGameObject->addTransform(ComponentFactory().createComponent<Transform>());
                gameObject->addChild(targetGameObject);
            }

            const size_t meshId = node.meshIds.at(i);
            Mesh& mesh = _modelMeshes.at(meshId);

            targetGameObject->addComponent(ComponentFactory().createComponent<MeshComponent>(&mesh));

		    if (!node.materialIds.empty())
		    {
			    const size_t materialId = node.materialIds.at(i);
			    Material& material = _modelMaterials.at(materialId);

			    mesh.setMaterial(&material);

			    targetGameObject->addComponent(ComponentFactory().createComponent<MaterialComponent>(&material));
		    }
        }
    }

    for (const Node& child : node.children)
    {
        gameObject->addChild(getGameObjectFromNode(child, gameObject));
    }

    return gameObject;
}