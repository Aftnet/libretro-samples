#include "d3d_util.h"
#include <d3d11.h>
#include <dxgi.h>

static ID3D11Texture2D* render_target_texture = nullptr;
static ID3D11RenderTargetView* render_target_view = nullptr;
static ID3D11Texture2D* stencil_view = nullptr;

#define SAFE_RELEASE(x) if(x) { x->Release(); x = NULL; }

void ThrowIfFailed(HRESULT hr)
{
	if (hr < 0)
	{
		throw;
	}
}

void d3d_util_init(ID3D11Device* device, int width, int height)
{
	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	ThrowIfFailed(device->CreateTexture2D(&desc, nullptr, &render_target_texture));
	device->CreateRenderTargetView(render_target_texture, nullptr, &render_target_view);
}

void d3d_util_deinit()
{
	SAFE_RELEASE(render_target_texture);
	SAFE_RELEASE(render_target_view);
	SAFE_RELEASE(stencil_view);
}

void* d3d_util_get_render_target()
{
	return render_target_texture;
}

void* d3d_util_get_depth_stencil_view()
{
	return stencil_view;
}