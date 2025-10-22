#include <Windows.h>
#include <concepts>
extern void D3D12HelloWorld();
extern void D3D11HelloWorld();

struct Foo
{
    struct Bar
    {

        Bar* operator->() { return this; }
    };

    Bar operator->() const { return Bar{}; }
    Bar operator*() const { return Bar{}; }
};

struct Baz
{

};

template<class Ty>
struct FooBaz
{

};

template<std::derived_from<Foo> Ty>
struct FooBaz<Ty>
{

};

template<std::derived_from<Baz> Ty>
struct FooBaz<Ty>
{

};

template<>
struct FooBaz<Baz>
{

};


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    D3D11HelloWorld();

    return 0;
}