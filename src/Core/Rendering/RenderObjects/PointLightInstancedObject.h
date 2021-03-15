// Copyright(c) 2019 - 2020, #Momo
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met :
// 
// 1. Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and /or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include "RenderHelperObject.h"

namespace MxEngine
{
	struct PointLightBaseData
	{
		Matrix4x4 Transform;
		Vector3 Position;
		float Radius;
		Vector3 Color;
		float AmbientIntensity;

		constexpr static size_t Size = 16 + 3 + 1 + 3 + 1;
	};

	class PointLightInstancedObject : public RenderHelperObject
	{
		VertexBufferHandle instancedVBO;
	public:
		MxVector<PointLightBaseData> Instances;

		PointLightInstancedObject() = default;

		PointLightInstancedObject(VertexBufferHandle vbo, VertexArrayHandle vao, IndexBufferHandle ibo)
			: RenderHelperObject(std::move(vbo), std::move(vao), std::move(ibo))
		{
			this->instancedVBO = GraphicFactory::Create<VertexBuffer>(nullptr, 0, UsageType::STATIC_DRAW);

			auto VBL = GraphicFactory::Create<VertexBufferLayout>();
			VBL->Push<Matrix4x4>(); // transform
			VBL->Push<Vector4>(); // position + radius
			VBL->Push<Vector4>(); // intensity * color + ambient intensity

			this->VAO->AddInstancedVertexBuffer(*this->instancedVBO, *VBL);
		}

		void SubmitToVBO() { instancedVBO->BufferDataWithResize((float*)this->Instances.data(), this->Instances.size() * PointLightBaseData::Size); }
	};
}