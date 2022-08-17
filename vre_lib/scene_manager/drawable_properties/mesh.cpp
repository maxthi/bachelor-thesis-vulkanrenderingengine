#include "mesh.hpp"
#include <qrandom.h>

#include "shaders.hpp"
#include "../../scene_reader/structs/drawable_data.hpp"

namespace vre
{
    // local random function to generate random colors
    float RandomFloat(float a, float b) {
        float random = ((float)rand()) / (float)RAND_MAX;
        float diff = b - a;
        float r = random * diff;
        return a + r;
    }

	Mesh::Mesh(const std::string& meshPath, const QVector4D& meshColor, bool randomizeSurfaceColors)
	{
        m_propertyId = Mesh::GetClassPropertyId();

        // get valid mesh path
        m_meshPath = Filesystem::validateFilePath(meshPath);

        // And have it read the given file with some example postprocessing
        // Usually - if speed is not the most important aspect for you - you'll
        // probably to request more postprocessing than we do in this example.
        const aiScene* scene = s_importer.ReadFile(m_meshPath,
            aiProcess_CalcTangentSpace |
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType);

        // If the import failed, report it
        if (!scene) {
            auto error = s_importer.GetErrorString();
            err(error);
        }

        check(scene->mNumMeshes != 0, "Mesh::Mesh: The file loaded didn't contain a readable mesh!");

        uint32_t index;
        aiVector3D aiVector;
        QVector3D vec;
        QVector2D texCoord;

        ai_real xMax{ 0 }, yMax{ 0 }, zMax{ 0 };
        ai_real xMin{ 0 }, yMin{ 0 }, zMin{ 0 };

        // a scene can contain multiple meshes. currently we put them all together in one buffer
        // but this should be handled differently in the future
        for (int i = 0; i < scene->mNumMeshes; i++)
        {
            // a mesh consists of multiple faces
            for (uint32_t j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
            {
                // each face
                for (uint32_t k = 0; k < 3; k++)
                {
                    index = scene->mMeshes[i]->mFaces[j].mIndices[k];

                    aiVector = scene->mMeshes[i]->mVertices[index];
                    vec.setX(aiVector.x);
                    vec.setY(aiVector.y);
                    vec.setZ(aiVector.z);
                    m_vertices.push_back(vec);

                    xMax = aiVector.x > xMax ? aiVector.x : xMax;
                    yMax = aiVector.y > yMax ? aiVector.y : yMax;
                    zMax = aiVector.z > zMax ? aiVector.z : zMax;
                    xMin = aiVector.x < xMin ? aiVector.x : xMin;
                    yMin = aiVector.y < yMin ? aiVector.y : yMin;
                    zMin = aiVector.z < zMin ? aiVector.z : zMin;

                    if (scene->mMeshes[i]->mNormals != nullptr)
                    {
                        aiVector = scene->mMeshes[i]->mNormals[index];
                        vec.setX(aiVector.x);
                        vec.setY(aiVector.y);
                        vec.setZ(aiVector.z);
                        m_normals.push_back(vec);
                    }

                    m_indices.push_back(m_vertices.size()-1);

                    // colors
                    if (randomizeSurfaceColors)
                    {
                        // create a random float for each value r,g,b
                        // between 0 and the value
                        m_colors.push_back(QVector4D(RandomFloat(0.0f, meshColor.x()), RandomFloat(0.0f, meshColor.y()), RandomFloat(0.0f, meshColor.z()), meshColor.w()));
                    }
                    else
                    {
                        // one color value for all vertices
                        m_colors.push_back(QVector4D(meshColor.x() , meshColor.y(), meshColor.z(), meshColor.w()));
                    }

                    if (scene->mMeshes[i]->mTextureCoords[0] != nullptr) {
                        texCoord.setX(scene->mMeshes[i]->mTextureCoords[0][index].x);
                        texCoord.setY(1.0f - scene->mMeshes[i]->mTextureCoords[0][index].y);
                        m_texCoords.push_back(texCoord);
                    }
                }
            }
        }

        float xDim = abs(xMax) + abs(xMin);
        float yDim = abs(yMax) + abs(yMin);
        float zDim = abs(zMax) + abs(zMin);

	}

    Mesh::Mesh(const std::vector<QVector3D>& vertexBuffer, const std::vector<uint32_t>& indexBuffer, const std::vector<QVector4D>& colorBuffer, const std::vector<QVector2D>& texCoords)
    {
        m_vertices = vertexBuffer;
        m_indices = indexBuffer;
        m_colors = colorBuffer;
        m_texCoords = texCoords;
    }

    void Mesh::AllocateMemory(std::shared_ptr<DrawableData>& drawableData)
    {
        auto drawable = drawableData->mNode->GetDrawable();
        auto shaders = drawable->GetProperty<ShadersBase>();

        auto iteratorMapMeshToShader = s_mapMeshToShaderId.find(this);
        std::shared_ptr<std::unordered_map<uint64_t, std::weak_ptr<VertexAndIndexBuffer>>> mapShaderIdToVertexBuffer;
        uint64_t shaderIndex = shaders->GetShaderCombination()->GetShaderCombinationIndex();
        if (iteratorMapMeshToShader != s_mapMeshToShaderId.end())
        {
            mapShaderIdToVertexBuffer = iteratorMapMeshToShader->second;
            auto iteratorMapShaderToVertexBuffer = mapShaderIdToVertexBuffer->find(shaderIndex);
            if (iteratorMapShaderToVertexBuffer != mapShaderIdToVertexBuffer->end())
            {
                auto buffer = iteratorMapShaderToVertexBuffer->second.lock();
                if (buffer != nullptr)
                {
                    drawableData->mVertexAndIndexBuffer = buffer;
                    return;
                }
                else
                {
                    // all instances of the vertex buffer have been removed.
                    mapShaderIdToVertexBuffer->erase(shaderIndex);
                }
            }
        }
        else
        {
            mapShaderIdToVertexBuffer = std::make_shared<std::unordered_map<uint64_t, std::weak_ptr<VertexAndIndexBuffer>>>();
            s_mapMeshToShaderId.insert({ this, mapShaderIdToVertexBuffer });
        }

        auto vertexLayout = shaders->GetShaderCombination()->GetVertexShader()->GetVertexLayout();

        // how mesh data is stored depends on the vertex layout
        drawableData->mVertexAndIndexBuffer = vertexLayout->CreateVertexAndIndexBuffer(drawable);

        // store a weak ptr for look up
        mapShaderIdToVertexBuffer->insert({ shaderIndex, drawableData->mVertexAndIndexBuffer });
    }

    std::vector<QVector3D>* Mesh::GetVertices()
    {
        return &m_vertices;
    }

    std::vector<uint32_t>* Mesh::GetIndices()
    {
        return &m_indices;
    }

    std::vector<QVector2D>* Mesh::GetTexCoords()
    {
        return &m_texCoords;
    }

    std::vector<QVector3D>* Mesh::GetNormals()
    {
        return &m_normals;
    }

    std::vector<QVector4D>* Mesh::GetColors()
    {
        return &m_colors;
    }
}