#include "d3d_util.h"
#include <d3d11.h>
#include <dxgi.h>

static ID3D11Texture2D* render_target_texture = nullptr;
static ID3D11RenderTargetView* render_target_view = nullptr;
static ID3D11Texture2D* stencil_texture = nullptr;
static ID3D11DepthStencilView* stencil_view = nullptr;

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
	ThrowIfFailed(device->CreateRenderTargetView(render_target_texture, nullptr, &render_target_view));

	UINT sample_count = 1;

	D3D11_TEXTURE2D_DESC ds_desc = { 0 };
	ds_desc.Width = width;
	ds_desc.Height = height;
	ds_desc.MipLevels = 1;
	ds_desc.ArraySize = 1;
	ds_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	ds_desc.SampleDesc.Count = sample_count;
	ds_desc.SampleDesc.Quality = 0;
	ds_desc.Usage = D3D11_USAGE_DEFAULT;
	ds_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ThrowIfFailed(device->CreateTexture2D(&ds_desc, nullptr, &stencil_texture));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc = { 0 };
	dsv_desc.Format = ds_desc.Format;
	dsv_desc.ViewDimension = sample_count > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;

	ThrowIfFailed(device->CreateDepthStencilView(stencil_texture, &dsv_desc, &stencil_view));
}

void d3d_util_deinit()
{
	SAFE_RELEASE(render_target_texture);
	SAFE_RELEASE(render_target_view);
	SAFE_RELEASE(stencil_texture);
	SAFE_RELEASE(stencil_view);
}

void* d3d_util_get_render_target()
{
	return render_target_view;
}

void* d3d_util_get_depth_stencil_view()
{
	return stencil_view;
}