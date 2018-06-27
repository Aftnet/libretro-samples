#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "libretro_d3d.h"

namespace LRTest
{
	class Renderer
	{
		private:
			pD3DCompile ShaderCompile;

			ID3D11Device* mDevice;
			ID3D11DeviceContext* mContext;
			
		public:
			void CreateResources();
			void Render();
			void DestroyResources();
	};
}

#endif
