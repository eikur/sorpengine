#include "Mesh.hpp"

#include <assimp/include/cimport.h>
#include <assimp/include/postprocess.h>
#include <assimp/include/scene.h>

// Material related. Cnsider injecting textureHelper
#include "Application.hpp"
#include "TextureHelper.hpp"
#include "Material.hpp"

#include "GameObject/GameObject.hpp"

Mesh::Mesh(const aiMesh* inMesh)
{
    _elementCount = inMesh->mNumFaces * 3;
    _vertexCount = inMesh->mNumVertices;

    if (inMesh->HasPositions())
    {
        _vertices = new float3[_vertexCount];
        for(std::size_t i = 0; i < _vertexCount; ++i)
        {
            const aiVector3D& vertex = inMesh->mVertices[i];
            _vertices[i] = float3(vertex.x, vertex.y, vertex.z);
        }
        
        glGenBuffers(1, &_vertexVBO);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexVBO);
        glBufferData(GL_ARRAY_BUFFER, _vertexCount * sizeof(_vertices[0]), &_vertices[0], GL_DYNAMIC_DRAW);   

        // not freeing _vertices to be able to skin meshes
    }

    if (inMesh->HasTextureCoords(0))
    {
        float2* textureCoords = new float2[_vertexCount];
        for (std::size_t i = 0; i < _vertexCount; ++i)
        {
            const aiVector3D& texCoord = inMesh->mTextureCoords[0][i];
            textureCoords[i] = float2(texCoord.x, texCoord.y);
        }

        glGenBuffers(1, &_texCoordVBO);
        glBindBuffer(GL_ARRAY_BUFFER, _texCoordVBO);
        glBufferData(GL_ARRAY_BUFFER, _vertexCount * sizeof(textureCoords[0]), &textureCoords[0], GL_STATIC_DRAW);

        delete[] textureCoords;
    }

    if (inMesh->HasNormals()) 
    {
        _normals = new float3[_vertexCount];
        for (size_t i = 0; i < inMesh->mNumVertices; ++i) 
        {
            const aiVector3D& normal = inMesh->mNormals[i];
            _normals[i] = float3(normal.x, normal.y, normal.z);
        }

        glGenBuffers(1, &_normalVBO);
        glBindBuffer(GL_ARRAY_BUFFER, _normalVBO);
        glBufferData(GL_ARRAY_BUFFER, _vertexCount * sizeof(_normals[0]), &_normals[0], GL_DYNAMIC_DRAW);

        // not freeing _normals to be able to skin meshes
    }

    if (inMesh->HasFaces()) 
    {
        const GLuint numIndices = inMesh->mNumFaces * inMesh->mFaces[0].mNumIndices;
        GLuint* indices = new GLuint[numIndices];
        for (size_t i = 0; i < inMesh->mNumFaces; ++i) {
            for (size_t j = 0; j < inMesh->mFaces[i].mNumIndices; ++j) {
                indices[inMesh->mFaces[i].mNumIndices * i + j] = inMesh->mFaces[i].mIndices[j];
            }
        }

        glGenBuffers(1, &_indexVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexVBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices  * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

        delete[] indices;
    }

    if (inMesh->HasBones())
    {
        _verticesSkin = new float3[_vertexCount];
        if (_normals != nullptr) 
        {
            _normalsSkin = new float3[_vertexCount];
        }

        _boneCount = inMesh->mNumBones;
        _bones = new Bone[_boneCount];
                
        for (size_t i = 0; i < _boneCount; ++i)
        {
            const aiBone& bone = *inMesh->mBones[i];
            Bone& createdBone = _bones[i];

            createdBone.name = bone.mName.C_Str();
            
            // offset matrix
            for (size_t row = 0; row < 4; ++row) 
            {
                for (size_t col = 0; col < 4; ++col) 
                {
                    createdBone.bind[row][col] = bone.mOffsetMatrix[row][col];
                }
            }

            // bone weights
            for (size_t j = 0; j < bone.mNumWeights; ++j) 
            {
                const aiVertexWeight& weight = bone.mWeights[j];

                Weight createdWeight; 
                createdWeight.vertex = weight.mVertexId;
                createdWeight.weight = weight.mWeight;
                
                createdBone.weights.push_back(createdWeight);
            }
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Mesh::~Mesh()
{
    delete[] _vertices;
    delete[] _normals;

    delete[] _bones;
    delete[] _verticesSkin;
    delete[] _normalsSkin;
}

void Mesh::cleanUp()
{
    glDeleteBuffers(1, &_vertexVBO);
    glDeleteBuffers(1, &_texCoordVBO);
    glDeleteBuffers(1, &_normalVBO);
    glDeleteBuffers(1, &_indexVBO);
}

void Mesh::draw() const
{
	const GLboolean lightingWasEnabled = glIsEnabled(GL_LIGHTING);

	if (_material != nullptr)
	{
		if (!lightingWasEnabled)
		{
			glEnable(GL_LIGHTING);
		}

		App->getTextureHelper().useTexture(_material->getTextureId());

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, _material->getColor(Material::ColorComponent::Ambient));
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, _material->getColor(Material::ColorComponent::Diffuse));
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, _material->getColor(Material::ColorComponent::Specular));
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, _material->getColor(Material::ColorComponent::Emissive));
		//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, _material->getShininess); //TODO add the shininess params
	}

    if (_vertexVBO) {
        glBindBuffer(GL_ARRAY_BUFFER, _vertexVBO);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, 0);
    }

    if (_texCoordVBO) {
        glBindBuffer(GL_ARRAY_BUFFER, _texCoordVBO);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, 0);
    }

    if (_normalVBO) {
        glBindBuffer(GL_ARRAY_BUFFER, _normalVBO);
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, 0);
    }

    if (_indexVBO) 
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexVBO);
        glDrawElements(GL_TRIANGLES, _elementCount, GL_UNSIGNED_INT, NULL);
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    // unbind texture from the material
    App->getTextureHelper().stopUsingTexture();

	if (!lightingWasEnabled)
	{
		glDisable(GL_LIGHTING);
	}
}

Mesh::Mesh(const Mesh& other)
{
    _vertexCount = other._vertexCount;
    _elementCount = other._elementCount;

    _vertexVBO = other._vertexVBO;
    _texCoordVBO = other._texCoordVBO;
    _normalVBO = other._normalVBO;
    _indexVBO = other._indexVBO;

    delete[] _vertices;
    _vertices = new float3[_vertexCount];
    for (size_t i = 0; i < _vertexCount; ++i)
    {
        _vertices[i] = other._vertices[i];
    }

    delete[] _normals;
	if (other._normals != nullptr)
	{
		_normals = new float3[_vertexCount];

		for (size_t i = 0; i < _vertexCount; ++i)
		{
			_normals[i] = other._normals[i];
		}
	}

    delete[] _verticesSkin;
    if (other._verticesSkin != nullptr)
    {
        _verticesSkin = new float3[_vertexCount];
    }
    
    delete[] _normalsSkin;
    if (other._normalsSkin != nullptr)
    {
        _normalsSkin = new float3[_vertexCount];
    }

    delete[] _bones;

    _boneCount = other._boneCount;

    if (other._bones != nullptr)
    {
        _bones = new Bone[_boneCount];
        
        for (size_t i = 0; i < _boneCount; ++i)
        {
            _bones[i] = other._bones[i];
        }
    }
}

void Mesh::setMaterial(Material* material)
{
	_material = material;
}

std::vector<std::string> Mesh::getBoneNames() const
{
    std::vector<std::string> retVal;
    for (size_t i = 0; i < _boneCount; ++i)
    {
        retVal.push_back(_bones[i].name);
    }
    return retVal;
}

bool Mesh::canBeSkinned() const
{
    return _boneCount > 0;
}

bool Mesh::isSkinned() const
{
    return _skinningSet;
}

void Mesh::skin(const std::map<int, GameObject*>& boneTargets)
{
    if (!canBeSkinned() || isSkinned())
    {
        return;
    }

    for (auto& kvp : boneTargets)
    {
        Bone& bone = _bones[kvp.first];
        bone.target = kvp.second;
    }

    _skinningSet = true;
}

void Mesh::updateSkin()
{
    if (!canBeSkinned() || !isSkinned())
    {
        return;
    }
    // update skinning vertices :)
    float4x4 mat4x4 = float4x4::identity;
    float3x3 mat3x3 = float3x3::identity;
    memset(_verticesSkin, 0, _vertexCount * sizeof(float3));
    memset(_normalsSkin, 0, _vertexCount * sizeof(float3));

    for (size_t b = 0; b < _boneCount; ++b)
    {
        const Bone& bone = _bones[b];
        if (bone.target == nullptr)
        {
            continue;
        }

        mat4x4 = bone.target->GetModelSpaceTransformMatrix() * bone.bind;
        mat3x3 = mat4x4.Float3x3Part();

        const size_t weightCount = bone.weights.size();
        for (size_t w = 0; w < weightCount; ++w)
        {
            const Weight& weight = bone.weights.at(w);
            _verticesSkin[weight.vertex] += weight.weight * mat4x4.TransformPos(_vertices[weight.vertex]);
            _normalsSkin[weight.vertex] += weight.weight * mat3x3.Transform(_normals[weight.vertex]);
        }
    }			

    // and bind them again
    glBindBuffer(GL_ARRAY_BUFFER, _vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, _vertexCount * sizeof(_verticesSkin[0]), &_verticesSkin[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, _normalVBO);
    glBufferData(GL_ARRAY_BUFFER, _vertexCount * sizeof(_normalsSkin[0]), &_normalsSkin[0], GL_DYNAMIC_DRAW);
}