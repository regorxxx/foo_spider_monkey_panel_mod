#pragma once

namespace smp
{

[[nodiscard]] std::optional<std::string>
LoadStringResource(int resourceId, const char* resourceType);

} // namespace smp
