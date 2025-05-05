#include <stdafx.h>

#include "resource_helpers.h"

namespace smp
{

std::optional<qwr::u8string> LoadStringResource( int resourceId, const char* resourceType )
{
    puResource puRes = uLoadResource( core_api::get_my_instance(), uMAKEINTRESOURCE( resourceId ), resourceType );

    if ( !puRes )
    {
        return std::nullopt;
    }

    return qwr::u8string{ static_cast<const char*>( puRes->GetPointer() ), puRes->GetSize() };
}

} // namespace smp
