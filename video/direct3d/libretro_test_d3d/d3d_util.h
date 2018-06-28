#ifndef _D3D_UTIL_H_
#define _D3D_UTIL_H_

struct ID3D11Device;

void d3d11_init(ID3D11Device* device, int width, int height);

#endif