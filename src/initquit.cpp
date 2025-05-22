#include <stdafx.h>

#include <libPPUI/gdiplus_helpers.h>
#include <config/delayed_package_utils.h>
#include <events/event_dispatcher.h>
#include <fb2k/playlist_lock.h>
#include <js_engine/js_engine.h>

#include <utils/thread_pool_instance.h>

#include <qwr/abort_callback.h>
#include <qwr/error_popup.h>

#include <Scintilla.h>

DECLARE_COMPONENT_VERSION(SMP_NAME, SMP_VERSION, SMP_ABOUT);
VALIDATE_COMPONENT_FILENAME(SMP_DLL_NAME);

namespace smp::com
{
    ITypeLibPtr g_typelib;
}

namespace
{
    CAppModule wtl_module;
    GdiplusScope scope;
    HMODULE rich_edit_ctrl{};

    class InitStageCallback : public init_stage_callback
    {
        void on_init_stage(t_uint32 stage) override
        {
            if (stage == init_stages::before_config_read)
            {
                try
                {
                    smp::config::ProcessDelayedPackages();
                }
                catch (const qwr::QwrException& e)
                {
                    qwr::ReportErrorWithPopup(SMP_UNDERSCORE_NAME, fmt::format("Failed to process delayed packages:\n{}", e.what()));
                }
            }
            else if (stage == init_stages::before_ui_init)
            {
                try
                {
                    smp::PlaylistLockManager::Get().InitializeLocks();
                }
                catch (const qwr::QwrException& e)
                {
                    qwr::ReportErrorWithPopup(SMP_UNDERSCORE_NAME, fmt::format("Failed to initialize playlist locks: {}", e.what()));
                }

                const auto ins = core_api::get_my_instance();

                Scintilla_RegisterClasses(ins);
                rich_edit_ctrl = LoadLibraryW(CRichEditCtrl::GetLibraryName());

                const auto path = wil::GetModuleFileNameW(ins);
                std::ignore = LoadTypeLibEx(path.get(), REGKIND_NONE, &smp::com::g_typelib);

                std::ignore = wtl_module.Init(nullptr, ins);
            }
        }
    };

    void on_quit()
    {
        mozjs::JsEngine::GetInstance().PrepareForExit();
        smp::EventDispatcher::Get().NotifyAllAboutExit();
        qwr::GlobalAbortCallback::GetInstance().Abort();
        smp::GetThreadPoolInstance().Finalize();
        Scintilla_ReleaseResources();
        FreeLibrary(rich_edit_ctrl);
        wtl_module.Term();
        smp::com::g_typelib.Release();
    }

    FB2K_SERVICE_FACTORY(InitStageCallback);
    FB2K_RUN_ON_QUIT(on_quit);
}
