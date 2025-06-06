#include <stdafx.h>

#include "panel_config_json.h"

#include <qwr/fb2k_paths.h>
#include <qwr/string_helpers.h>
#include <qwr/type_traits.h>
#include <qwr/winapi_error_helpers.h>

using namespace smp;

namespace
{

enum class ScriptType : uint8_t
{
    SimpleInMemory = 1,
    SimpleSample = 2,
    SimpleFile = 3,
    Package = 4
};

enum class LocationType : uint8_t
{
    Full = 0,
    Component = 1,
    Profile = 2,
    Fb2k = 3
};

constexpr const char kPropJsonConfigVersion[] = "1";
constexpr const char kPropJsonConfigId[] = "properties";

constexpr const char kSettingsJsonConfigVersion[] = "1";
constexpr const char kSettingsJsonConfigId[] = "settings";

} // namespace

namespace
{

JSON SerializePropertiesToObject(const config::PanelProperties& properties)
{
    try
    {
        auto jsonMain = JSON::object({ { "id", kPropJsonConfigId },
                                        { "version", kPropJsonConfigVersion } });

        auto jsonValues = JSON::object();
        for (const auto& [nameW, pValue]: properties.values)
        {
            const auto propertyName = qwr::ToU8(nameW);
            const auto& serializedValue = *pValue;

            std::visit([&jsonValues, &propertyName](auto&& arg) { jsonValues.push_back({ propertyName, arg }); },
                        serializedValue);
        }

        jsonMain.push_back({ "values", jsonValues });

        return jsonMain;
    }
    catch (const JSON::exception& e)
    {
        throw qwr::QwrException(e.what());
    }
}

config::PanelProperties DeserializePropertiesFromObject(const JSON& jsonMain)
{
    try
    {
        config::PanelProperties properties;

        if (!jsonMain.is_object())
        {
            throw qwr::QwrException("Corrupted serialized properties: not a JSON object");
        }

        if (jsonMain.at("version").get<std::string>() != kPropJsonConfigVersion
             || jsonMain.at("id").get<std::string>() != kPropJsonConfigId)
        {
            throw qwr::QwrException("Corrupted serialized properties: version/id mismatch");
        }

        auto& jsonValues = jsonMain.at("values");
        if (!jsonValues.is_object())
        {
            throw qwr::QwrException("Corrupted serialized properties: values");
        }

        for (auto& [key, value]: jsonValues.items())
        {
            if (key.empty())
            {
                throw qwr::QwrException("Corrupted serialized properties: empty key");
            }

            mozjs::SerializedJsValue serializedValue;
            if (value.is_boolean())
            {
                serializedValue = value.get<bool>();
            }
            else if (value.is_number_integer())
            {
                serializedValue = value.get<int32_t>();
            }
            else if (value.is_number_float())
            {
                serializedValue = value.get<double>();
            }
            else if (value.is_string())
            {
                serializedValue = value.get<std::string>();
            }
            else
            {
                assert(0);
                continue;
            }

            properties.values.emplace(qwr::ToWide(key), std::make_shared<mozjs::SerializedJsValue>(serializedValue));
        }

        return properties;
    }
    catch (const JSON::exception& e)
    {
        throw qwr::QwrException(e.what());
    }
}

} // namespace

namespace smp::config::json
{

PanelSettings LoadSettings(stream_reader& reader, abort_callback& abort)
{
    namespace fs = std::filesystem;

    try
    {
        PanelSettings panelSettings;

        const auto jsonMain = JSON::parse(reader.read_string(abort).get_ptr());
        if (!jsonMain.is_object())
        {
            throw qwr::QwrException("Corrupted serialized settings: not a JSON object");
        }

        if (jsonMain.at("version").get<std::string>() != kSettingsJsonConfigVersion
             || jsonMain.at("id").get<std::string>() != kSettingsJsonConfigId)
        {
            throw qwr::QwrException("Corrupted serialized settings: version/id mismatch");
        }

        if (jsonMain.find("panelId") != jsonMain.end())
        {
            jsonMain.at("panelId").get_to(panelSettings.id);
        }

        panelSettings.isPseudoTransparent = jsonMain.value("isPseudoTransparent", false);

        const auto scriptType = jsonMain.at("scriptType").get<ScriptType>();
        const auto jsonPayload = jsonMain.at("payload");
        switch (scriptType)
        {
        case ScriptType::SimpleInMemory:
        {
            panelSettings.payload = PanelSettings_InMemory{ jsonPayload.at("script").get<std::string>() };
            break;
        }
        case ScriptType::SimpleFile:
        {
            const auto wpath = qwr::ToWide(jsonPayload.at("path").get<std::string>());
            const auto fsPath = fs::path(wpath).lexically_normal();
            const auto fullPath = [&] {
                switch (jsonPayload.at("locationType").get<LocationType>())
                {
                case LocationType::Component:
                {
                    return (qwr::path::Component() / fsPath);
                }
                case LocationType::Fb2k:
                {
                    return (qwr::path::Foobar2000() / fsPath);
                }
                case LocationType::Profile:
                {
                    return (qwr::path::Profile() / fsPath);
                }
                case LocationType::Full:
                {
                    return fsPath;
                }
                default:
                    throw qwr::QwrException("Corrupted serialized settings: unknown file location type");
                }
            }();

            panelSettings.payload = PanelSettings_File{ fullPath.u8string() };
            break;
        }
        case ScriptType::SimpleSample:
        {
            panelSettings.payload = PanelSettings_Sample{ jsonPayload.at("sampleName").get<std::string>() };
            break;
        }
        case ScriptType::Package:
        {
            panelSettings.payload = PanelSettings_Package{ jsonPayload.at("id").get<std::string>(),
                                                           jsonPayload.at("name").get<std::string>(),
                                                           jsonPayload.at("author").get<std::string>() };
            break;
        }
        default:
        {
            throw qwr::QwrException("Corrupted serialized settings: unknown script type");
        }
        }

        panelSettings.properties = DeserializePropertiesFromObject(jsonMain.at("properties"));
        panelSettings.edgeStyle = static_cast<EdgeStyle>(jsonMain.value("edgeStyle", static_cast<uint8_t>(EdgeStyle::Default)));
        panelSettings.isPseudoTransparent = jsonMain.value("isPseudoTransparent", false);

        return panelSettings;
    }
    catch (const JSON::exception& e)
    {
        throw qwr::QwrException(e.what());
    }
    catch (const fs::filesystem_error& e)
    {
        throw qwr::QwrException(e);
    }
    catch (const pfc::exception& e)
    {
        throw qwr::QwrException(e.what());
    }
}

void SaveSettings(stream_writer& writer, abort_callback& abort, const PanelSettings& settings)
{
    namespace fs = std::filesystem;

    try
    {
        auto jsonMain = JSON::object();
        jsonMain.push_back({ "id", kSettingsJsonConfigId });
        jsonMain.push_back({ "version", kSettingsJsonConfigVersion });
        jsonMain.push_back({ "panelId", settings.id });

        auto jsonPayload = JSON::object();
        const auto scriptType = std::visit([&jsonPayload](const auto& data) {
            using T = std::decay_t<decltype(data)>;
            if constexpr (std::is_same_v<T, smp::config::PanelSettings_InMemory>)
            {
                jsonPayload.push_back({ "script", data.script });
                return ScriptType::SimpleInMemory;
            }
            else if constexpr (std::is_same_v<T, smp::config::PanelSettings_File>)
            {
                const auto [path, locationType] = [wpath = qwr::ToWide(data.path)] {
                    try
                    {
                        auto fsPath = fs::path(wpath).lexically_normal();

                        const auto isSubpath = [](const auto& path, const auto& base) {
                            return (path.wstring().find(base.lexically_normal().wstring()) == 0);
                        };

                        if (isSubpath(fsPath, qwr::path::Component()))
                        {
                            return std::make_tuple(fs::relative(fsPath, qwr::path::Component()).u8string(), LocationType::Component);
                        }
                        if (isSubpath(fsPath, qwr::path::Profile()))
                        {
                            return std::make_tuple(fs::relative(fsPath, qwr::path::Profile()).u8string(), LocationType::Profile);
                        }
                        if (isSubpath(fsPath, qwr::path::Foobar2000()))
                        {
                            return std::make_tuple(fs::relative(fsPath, qwr::path::Foobar2000()).u8string(), LocationType::Fb2k);
                        }

                        return std::make_tuple(fsPath.u8string(), LocationType::Full);
                    }
                    catch (const fs::filesystem_error& e)
                    {
                        throw qwr::QwrException(e);
                    }
                }();

                jsonPayload.push_back({ "path", path });
                jsonPayload.push_back({ "locationType", locationType });
                return ScriptType::SimpleFile;
            }
            else if constexpr (std::is_same_v<T, smp::config::PanelSettings_Sample>)
            {
                jsonPayload.push_back({ "sampleName", data.sampleName });
                return ScriptType::SimpleSample;
            }
            else if constexpr (std::is_same_v<T, smp::config::PanelSettings_Package>)
            {
                jsonPayload.push_back({ "id", data.id });
                jsonPayload.push_back({ "name", data.name });
                jsonPayload.push_back({ "author", data.author });
                jsonPayload.push_back({ "version", data.version });
                return ScriptType::Package;
            }
            else
            {
                static_assert(qwr::always_false_v<T>, "non-exhaustive visitor!");
            }
        },
                                            settings.payload);

        jsonMain.push_back({ "scriptType", static_cast<uint8_t>(scriptType) });
        jsonMain.push_back({ "payload", jsonPayload });
        jsonMain.push_back({ "properties", SerializePropertiesToObject(settings.properties) });
        jsonMain.push_back({ "edgeStyle", static_cast<uint8_t>(settings.edgeStyle) });
        jsonMain.push_back({ "isPseudoTransparent", settings.isPseudoTransparent });

        writer.write_string(jsonMain.dump(2), abort);
    }
    catch (const JSON::exception& e)
    {
        throw qwr::QwrException(e.what());
    }
    catch (const fs::filesystem_error& e)
    {
        throw qwr::QwrException(e);
    }
    catch (const pfc::exception& e)
    {
        throw qwr::QwrException(e.what());
    }
}

PanelProperties LoadProperties(stream_reader& reader, abort_callback& abort)
{
    try
    {
        return DeserializeProperties(reader.read_string(abort).get_ptr());
    }
    catch (const pfc::exception& e)
    {
        throw qwr::QwrException(e.what());
    }
}

void SaveProperties(stream_writer& writer, abort_callback& abort, const PanelProperties& properties)
{
    try
    {
        writer.write_string(SerializeProperties(properties), abort);
    }
    catch (const pfc::exception& e)
    {
        throw qwr::QwrException(e.what());
    }
}

PanelProperties DeserializeProperties(const std::string& str)
{
    try
    {
        return DeserializePropertiesFromObject(JSON::parse(str));
    }
    catch (const JSON::exception& e)
    {
        throw qwr::QwrException(e.what());
    }
}

std::string SerializeProperties(const PanelProperties& properties)
{
    try
    {
        return SerializePropertiesToObject(properties).dump(2);
    }
    catch (const JSON::exception& e)
    {
        throw qwr::QwrException(e.what());
    }
}

} // namespace smp::config::json
