#include <stdafx.h>

#include "resource_helpers.h"

namespace smp
{

std::optional<std::string> LoadStringResource( int resourceId, const char* resourceType )
{
    puResource puRes = uLoadResource( core_api::get_my_instance(), uMAKEINTRESOURCE( resourceId ), resourceType );

    if ( !puRes )
    {
        return std::nullopt;
    }

    return std::string{ static_cast<const char*>( puRes->GetPointer() ), puRes->GetSize() };
}

} // namespace smp
