#pragma once

#include <js_objects/object_base.h>

#include <optional>
#include <string>

class JSObject;
struct JSContext;
struct JSClass;

namespace mozjs
{

class JsFbProfiler
    : public JsObjectBase<JsFbProfiler>
{
public:
    static constexpr bool HasProto = true;
    static constexpr bool HasGlobalProto = true;
    static constexpr bool HasStaticFunctions = false;
    static constexpr bool HasProxy = false;
    static constexpr bool HasPostCreate = false;

    static const JSClass JsClass;
    static const JSFunctionSpec* JsFunctions;
    static const JSPropertySpec* JsProperties;
    static const JsPrototypeId PrototypeId;
    static const JSNative JsConstructor;

public:
    ~JsFbProfiler() override = default;

    static std::unique_ptr<JsFbProfiler> CreateNative( JSContext* cx, const std::string& name );
    static size_t GetInternalSize( const std::string& name );

public: // ctor
    static JSObject* Constructor( JSContext* cx, const std::string& name = "" );
    static JSObject* ConstructorWithOpt( JSContext* cx, size_t optArgCount, const std::string& name );

public:
    void Print( const std::string& additionalMsg = "", bool printComponentInfo = true );
    void PrintWithOpt( size_t optArgCount, const std::string& additionalMsg, bool printComponentInfo );
    void Reset();

public:
    uint32_t get_Time();

private:
    JsFbProfiler( JSContext* cx, const std::string& name );

private:
    [[maybe_unused]] JSContext* pJsCtx_ = nullptr;
    std::string name_;
    pfc::hires_timer timer_;
};

} // namespace mozjs
