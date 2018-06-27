#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "libretro_d3d.h"
#include <DirectXMath.h>
#include <windows.h>

namespace LRTest
{
	struct ModelViewProjectionConstantBuffer
	{
		DirectX::XMFLOAT4X4 model;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};

		// Used to send per-vertex data to the vertex shader.
	struct VertexPositionColor
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 color;
	};

	class Renderer
	{
		private:
			bool m_ready;
			pD3DCompile m_shaderCompile;

			ID3D11Device* m_d3ddevice;
			ID3D11DeviceContext* m_d3dcontext;
			
			ID3D11RenderTargetView* m_renderTargetView;
        	ID3D11DepthStencilView* m_depthStencilView;
        	ID3D11VertexShader* m_vertexShader;
        	ID3D11PixelShader* m_pixelShader;
        	ID3D11InputLayout* m_inputLayout;
        	ID3D11Buffer* m_vertexBuffer;
        	ID3D11Buffer* m_indexBuffer;
        	ID3D11Buffer* m_constantBuffer;

        	ModelViewProjectionConstantBuffer m_constantBufferData;

        	unsigned int m_indexCount;

		public:
			void CreateResources();
			void Render();
			void DestroyResources();
	};
}

#endif
