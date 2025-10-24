#include <Windows.h>
#include <concepts>
extern void D3D12HelloWorld();
extern void D3D11HelloWorld();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    D3D11HelloWorld();

    return 0;
}