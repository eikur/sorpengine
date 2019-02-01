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
    for (auto& kvp: _modelMeshes)
    {
        kvp.second.cleanUp();
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
        Utils::log("Model file could not be loaded. Path: %s", asset);
		Utils::log("Error: %s", aiGetErrorString());
        return false;
    }

	const size_t pathEndPos = asset.find_last_of('/');
	const std::string path = asset.substr(0, pathEndPos + 1);
    _modelNodes[asset] = loadNode(path, loadedScene, loadedScene->mRootNode, nullptr);

    return true;
}

Node ModelHelper::loadNode(const std::string& assetPath, const aiScene* scene, const aiNode* node, Node* parent)
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

        loadMaterial(assetPath, scene, mesh, createdNode);

        createdNode.meshIds.push_back(_modelMeshes.size());

        _modelMeshes.insert(std::pair<int, Mesh>(createdNode.meshIds.back(), Mesh(mesh)));
    }

    // Recursively process children nodes
    for (size_t i = 0; i < node->mNumChildren; ++i)
    {
        createdNode.children.push_back(loadNode(assetPath, scene,  node->mChildren[i], &createdNode));
    }

    return createdNode;
}

void ModelHelper::loadAnimationsForModel(const std::string& model, const std::string& animationPath)
{
    const aiScene* loadedScene = aiImportFile(animationPath.c_str(), aiProcess_Triangulate | aiProcessPreset_TargetRealtime_MaxQuality);
    if (loadedScene == nullptr)
    {
        Utils::log("Animation file could not be loaded. Path: %s", animationPath);
        Utils::log("Error: %s", aiGetErrorString());
        return;
    }

    Node* modelNode = nullptr;
    const auto it = _modelNodes.find(model);
    if (it != _modelNodes.cend())
    {
        modelNode = &(it->second);
    }

    const size_t animationIdBaseline = _animations.size();
    const size_t animationCount = loadedScene->mNumAnimations;

    for (size_t animIdx = 0; animIdx < animationCount; ++animIdx)
    {
        const aiAnimation* animation = loadedScene->mAnimations[animIdx];
        const std::string& animationName = animation->mName.C_Str();

        Animation createdAnimation;
        
        createdAnimation.id = animationIdBaseline + animIdx;
        if (modelNode)
        {
            modelNode->animationIds.push_back(createdAnimation.id);
        }

        createdAnimation.name = animationName;

        //const float ticksPerSecond = animation->mTicksPerSecond <= 0 ? 16.6f : animation->mTicksPerSecond;
        // restore this
        const float ticksPerSecond = 16.6f;
        createdAnimation.duration = animation->mDuration * ticksPerSecond;
        
        const size_t channelCount = animation->mNumChannels;
        for (size_t channelIdx = 0; channelIdx < channelCount; ++channelIdx)
        {
            const aiNodeAnim* channel = animation->mChannels[channelIdx];

            AnimationChannel createdChannel;
            createdChannel.name = channel->mNodeName.C_Str();
            
            // store positions
            const size_t positionCount = channel->mNumPositionKeys;
            for (size_t posIdx = 0; posIdx < positionCount; ++posIdx)
            {
                const aiVectorKey posKey = channel->mPositionKeys[posIdx];
                createdChannel.positions.push_back(float3(posKey.mValue.x, posKey.mValue.y, posKey.mValue.z));
            }

            // store rotations
            const size_t rotationCount = channel->mNumRotationKeys;
            for (size_t rotIdx = 0; rotIdx < rotationCount; ++rotIdx)
            {
                const aiQuatKey rotKey = channel->mRotationKeys[rotIdx];
                createdChannel.rotations.push_back(Quat(rotKey.mValue.x, rotKey.mValue.y, rotKey.mValue.z, rotKey.mValue.w));
            }

            createdAnimation.channels.push_back(createdChannel);
        }
        _animations[createdAnimation.id] = createdAnimation;
    }
}


void ModelHelper::loadMaterial(const std::string& assetPath, const aiScene* scene, const aiMesh* mesh, Node& targetNode)
{
    const size_t materialId = mesh->mMaterialIndex;
    const aiMaterial* material = scene->mMaterials[materialId];

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

    const size_t newMaterialId = _modelMaterials.size();
    targetNode.materialIds.push_back(newMaterialId);
    _modelMaterials.insert(std::pair<int,Material>(newMaterialId, Material(materialTextureId)));

    // load material colors
    Material& loadedMaterial = _modelMaterials.at(newMaterialId);
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
}


std::unique_ptr<GameObject> ModelHelper::getGameObjectFromModel(const std::string& asset)
{
    if (!loadModel(asset))
    {
        Utils::log("Couldn't generate game object from model %s", asset);
        return nullptr;
    }

    const Node& node = _modelNodes.at(asset);
    
    return getGameObjectFromNode(node, nullptr);
}

std::unique_ptr<GameObject> ModelHelper::getGameObjectFromNode(const Node& node, GameObject* parent)
{
    std::unique_ptr<GameObject> gameObject = std::make_unique<GameObject>(node.name);

    const float3 pos = node.position;
    const Quat rot = node.rotation;
    const float3 sca = node.scale;

    gameObject->addComponent(ComponentFactory().createComponent<Transform>(pos, rot, sca));

    // add animator here if it's the parent one
    const bool isRootGameObject = parent == nullptr;
    if (isRootGameObject)
        gameObject->markAsModelRoot();
    {
        if (!node.animationIds.empty())
        {
            // load by default the first one
            size_t animationId = node.animationIds.front();
            gameObject->addComponent(ComponentFactory().createComponent<AnimationComponent>(&_animations.at(animationId)));
        }
    }

    if (!node.meshIds.empty())
    {
        for (size_t i = 0; i < node.meshIds.size(); ++i)
        {
            const bool addMeshToChildrenGameObject = i > 0;
            GameObject* targetGameObject = gameObject.get();
            if (addMeshToChildrenGameObject)
            {
                std::unique_ptr<GameObject> additionalGameObject = std::make_unique<GameObject>(gameObject->getName() + std::to_string(i), gameObject.get());
                additionalGameObject->addComponent(ComponentFactory().createComponent<Transform>());

                targetGameObject = additionalGameObject.get();
                gameObject->addChild(std::move(additionalGameObject));
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
        gameObject->addChild(getGameObjectFromNode(child, gameObject.get()));
    }

    return gameObject;
}