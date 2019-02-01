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
    std::unique_ptr<T> createComponent(Args... args)
    {
        return createComponent<T>(args...);
    }


    //Transform
    template<>
    std::unique_ptr<Transform> createComponent()
    {
        return TransformBuilder().build();
    }

    template<>
    std::unique_ptr<Transform> createComponent(const float3 pos, const Quat rotation, const float3 scale)
    {
        return TransformBuilder().withPosition(pos).withRotation(rotation).withScale(scale).build();
    }

    // Image
    template<>
    std::unique_ptr<Image> createComponent()
    {
        return std::make_unique<Image>();
    }

    template<>
    std::unique_ptr<Image> createComponent(const std::string assetName)
    {
        if (assetName.empty())
        {
            return createComponent<Image>();
        }

        return std::make_unique<Image>(assetName);
    }

    // MeshComponent
    template<>
    std::unique_ptr<MeshComponent> createComponent()
    {
        return std::make_unique<MeshComponent>(nullptr);
    }

    template<>
    std::unique_ptr<MeshComponent> createComponent(Mesh* mesh)
    {
        return std::make_unique<MeshComponent>(mesh);
    }

	// MaterialComponent
	template<>
	std::unique_ptr<MaterialComponent> createComponent(Material* material)
	{
		return std::make_unique<MaterialComponent>(material);
	}

    // Camera Component
    template<>
    std::unique_ptr<Camera> createComponent()
    {
        return std::make_unique<Camera>();
    }

    // Animation Component
    template<>
    std::unique_ptr<AnimationComponent> createComponent()
    {
        return std::make_unique<AnimationComponent>(nullptr);
    }

    template<>
    std::unique_ptr<AnimationComponent> createComponent(Animation* animation)
    {
        return std::make_unique<AnimationComponent>(animation);
    }

    const ComponentFactory& operator() ()
    {
        return *this;
    };
};

