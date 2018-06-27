#include "renderer.h"

using namespace LRTest;
using namespace DirectX;

void Renderer::CreateResources()
{

}

void Renderer::Render()
{
    if (!m_ready)
    {
        return;
    }

    // Eye is at (0,0.7,1.5), looking at point (0,-0.1,0) with the up-vector along the y-axis.
    static const XMVECTORF32 eye = { 0.0f, 0.7f, 1.5f, 0.0f };
    static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
    static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

    // Convert degrees to radians, then convert seconds to rotation angle
    float radiansPerFrame = 0.01f;
    float animRadians = (float) fmod(radiansPerFrame, XM_2PI);

    // Prepare to pass the view matrix, and updated model matrix, to the shader
    XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
    XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(animRadians)));

    // Set render targets to the screen.
    ID3D11RenderTargetView *const targets[1] = { m_renderTargetView.Get() };
    m_d3dContext->OMSetRenderTargets(1, targets, m_depthStencilView.Get());

    // Clear the back buffer and depth stencil view.
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), DirectX::Colors::CornflowerBlue);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    // Prepare the constant buffer to send it to the Graphics device.
    m_d3dContext->UpdateSubresource(
        m_constantBuffer.Get(),
        0,
        NULL,
        &m_constantBufferData,
        0,
        0
        );

    // Each vertex is one instance of the VertexPositionColor struct.
    UINT stride = sizeof(VertexPositionColor);
    UINT offset = 0;
    m_d3dContext->IASetVertexBuffers(
        0,
        1,
        m_vertexBuffer.GetAddressOf(),
        &stride,
        &offset
        );

    m_d3dContext->IASetIndexBuffer(
        m_indexBuffer.Get(),
        DXGI_FORMAT_R16_UINT, // Each index is one 16-bit unsigned integer (short).
        0
        );

    m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_d3dContext->IASetInputLayout(m_inputLayout.Get());

    // Attach our vertex shader.
    m_d3dContext->VSSetShader(
        m_vertexShader.Get(),
        nullptr,
        0
        );

    // Send the constant buffer to the Graphics device.
    m_d3dContext->VSSetConstantBuffers(
        0,
        1,
        m_constantBuffer.GetAddressOf()
        );

    // Attach our pixel shader.
    m_d3dContext->PSSetShader(
        m_pixelShader.Get(),
        nullptr,
        0
        );

    // Draw the objects.
    m_d3dContext->DrawIndexed(
        m_indexCount,
        0,
        0
        );
}