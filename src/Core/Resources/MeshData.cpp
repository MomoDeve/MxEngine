#include "MeshData.h"

namespace MxEngine
{
    MeshData::MeshData()
    {
        this->VBO = GraphicFactory::Create<VertexBuffer>();
        this->VAO = GraphicFactory::Create<VertexArray>();
        this->IBO = GraphicFactory::Create<IndexBuffer>();

        auto VBL = GraphicFactory::Create<VertexBufferLayout>();
        VBL->PushFloat(3); // position //-V525
        VBL->PushFloat(2); // texture
        VBL->PushFloat(3); // normal
        VBL->PushFloat(3); // tangent
        VBL->PushFloat(3); // bitangent

        this->VAO->AddBuffer(*this->VBO, *VBL);
    }

    GResource<VertexArray> MeshData::GetVAO() const
    {
        return this->VAO;
    }

    GResource<VertexBuffer> MeshData::GetVBO() const
    {
        return this->VBO;
    }

    GResource<IndexBuffer> MeshData::GetIBO() const
    {
        return this->IBO;
    }

    const AABB& MeshData::GetAABB() const
    {
        return this->boundingBox;
    }

    MeshData::VertexData& MeshData::GetVertecies()
    {
        return this->vertecies;
    }
    const MeshData::VertexData& MeshData::GetVertecies() const
    {
        return this->vertecies;
    }

    MeshData::IndexData& MeshData::GetIndicies()
    {
        return this->indicies;
    }

    const MeshData::IndexData& MeshData::GetIndicies() const
    {
        return this->indicies;
    }

    void MeshData::BufferVertecies(UsageType usageType)
    {
        auto data = reinterpret_cast<float*>(this->vertecies.data());
        this->VBO->Load(data, this->vertecies.size() * Vertex::Size, usageType);
    }

    void MeshData::BufferIndicies()
    {
        auto data = reinterpret_cast<uint32_t*>(this->indicies.data());
        this->IBO->Load(data, this->indicies.size());
    }

    void MeshData::UpdateBoundingBox()
    {
        this->boundingBox = { MakeVector3(0.0f), MakeVector3(0.0f) };
        if (vertecies.size() > 0)
        {
            this->boundingBox = { vertecies[0].Position, vertecies[0].Position };
            for (const auto& vertex : vertecies)
            {
                this->boundingBox.Min = VectorMin(this->boundingBox.Min, vertex.Position);
                this->boundingBox.Max = VectorMax(this->boundingBox.Max, vertex.Position);
            }
        }
    }

    void MeshData::RegenerateNormals()
    {
        // first set all normal-space vectors to 0
        for (auto& vertex : vertecies)
        {
            vertex.Normal    = MakeVector3(0.0f);
            vertex.Tangent   = MakeVector3(0.0f);
            vertex.Bitangent = MakeVector3(0.0f);
        }

        // then compute normal space vectors for each triangle
        for (size_t i = 0; i < indicies.size() / 3; i++)
        {
            std::array triangle = { // copy 3 vertecies
                vertecies[indicies[3 * i + 0]],
                vertecies[indicies[3 * i + 1]],
                vertecies[indicies[3 * i + 2]],
            };

            auto normal = ComputeNormal(triangle[0].Position, triangle[1].Position, triangle[2].Position);
            auto tanbitan = ComputeTangentSpace(
                triangle[0].Position, triangle[1].Position, triangle[2].Position,
                triangle[0].TexCoord, triangle[1].TexCoord, triangle[2].TexCoord
            );

            for (auto& vertex : triangle)
            {
                vertex.Normal += normal;
                vertex.Tangent += tanbitan[0];
                vertex.Bitangent += tanbitan[1];
            }
            
            for (size_t j = 0; j < triangle.size(); j++)
            {
                vertecies[indicies[3 * i + j]] = triangle[j]; // reassign copied vertecies
            }
        }

        // at the end normalize all normal-space vectors using vertex weights
        for (size_t i = 0; i < vertecies.size(); i++)
        {
            vertecies[i].Normal    = Normalize(vertecies[i].Normal);
            vertecies[i].Tangent   = Normalize(vertecies[i].Tangent);
            vertecies[i].Bitangent = Normalize(vertecies[i].Bitangent);
        }
    }
}