#include "Renderer.h"
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

using namespace DirectX;
using namespace DirectX::PackedVector;

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw hr;
	}
}

void Renderer::Init()
{

}

void Renderer::Deinit()
{

}

void Renderer::RenderFrame()
{

}