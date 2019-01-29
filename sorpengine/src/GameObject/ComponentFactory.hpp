#pragma once

#include <memory>

// Component files
#include "AnimationComponent.hpp"
#include "Component.hpp"
#include "Camera.hpp"
#include "Image.hpp"
#include "MaterialComponent.hpp"
#include "MeshComponent.hpp"
#include "TransformBuilder.hpp"

class Material;
class Mesh;

struct Animation;

class ComponentFactory
{
public: 
    template<class T, typename... Args>
    std::shared_ptr<T> createComponent(Args... args)
    {
        return createComponent<T>(args...);
    }


    //Transform
    template<>
    std::shared_ptr<Transform> createComponent()
    {
        return TransformBuilder().build();
    }

    template<>
    std::shared_ptr<Transform> createComponent(const float3 pos, const Quat rotation, const float3 scale)
    {
        return TransformBuilder().withPosition(pos).withRotation(rotation).withScale(scale).build();
    }

    // Image
    template<>
    std::shared_ptr<Image> createComponent()
    {
        return std::make_shared<Image>();
    }

    template<>
    std::shared_ptr<Image> createComponent(const std::string assetName)
    {
        if (assetName.empty())
        {
            return createComponent<Image>();
        }

        return std::make_shared<Image>(assetName);
    }

    // MeshComponent
    template<>
    std::shared_ptr<MeshComponent> createComponent()
    {
        return std::make_shared<MeshComponent>(nullptr);
    }

    template<>
    std::shared_ptr<MeshComponent> createComponent(Mesh* mesh)
    {
        return std::make_shared<MeshComponent>(mesh);
    }

	// MaterialComponent
	template<>
	std::shared_ptr<MaterialComponent> createComponent(Material* material)
	{
		return std::make_shared<MaterialComponent>(material);
	}

    // Camera Component
    template<>
    std::shared_ptr<Camera> createComponent()
    {
        return std::make_shared<Camera>();
    }

    // Animation Component
    template<>
    std::shared_ptr<AnimationComponent> createComponent()
    {
        return std::make_shared<AnimationComponent>(nullptr);
    }

    template<>
    std::shared_ptr<AnimationComponent> createComponent(Animation* animation)
    {
        return std::make_shared<AnimationComponent>(animation);
    }

    const ComponentFactory& operator() ()
    {
        return *this;
    };
};

