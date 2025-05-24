#pragma once

namespace smp::utils
{

/// @throw qwr::QwrException
void ExecuteContextCommandByName(const std::string& name, const metadb_handle_list& p_handles, uint32_t flags);

/// @throw qwr::QwrException
void ExecuteMainmenuCommandByName(const std::string& name);

/// @throw qwr::QwrException
uint32_t GetMainmenuCommandStatusByName(const std::string& name);

} // namespace smp::utils
