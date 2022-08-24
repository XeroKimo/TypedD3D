#include "Exceptions.h"

namespace TypedD3D::Helpers
{
    void ThrowIfFailed(HRESULT result)
    {
        if(FAILED(result))
            Throw(result);
    }

    void Throw(HRESULT result)
    {
        switch(result)
        {
        case 0:
            break;
        default:
            throw HRESULTError("Unknown error has occured", result);
        }
    }
}