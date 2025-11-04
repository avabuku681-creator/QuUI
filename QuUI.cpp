// QuUI.cpp
// Точка входа SDK — инициализирует подсистемы при подключении/линковке QuUI
#include "QuUI.h"

#include <atomic>
#include <mutex>
#include <iostream>

namespace QuUI
{
    static std::atomic<bool> s_initialized{false};
    static std::mutex s_initMutex;

    // Внутренние шаги инициализации (заглушки — можно заменить реальными реализациями)
    static void InitLogging()   { std::cout << "[QuUI] InitLogging\n"; }
    static void InitPlatform()  { std::cout << "[QuUI] InitPlatform\n"; }
    static void InitRenderer()  { std::cout << "[QuUI] InitRenderer\n"; }
    static void InitInput()     { std::cout << "[QuUI] InitInput\n"; }
    static void InitResources() { std::cout << "[QuUI] InitResources\n"; }

    static void ShutdownResources() { std::cout << "[QuUI] ShutdownResources\n"; }
    static void ShutdownInput()     { std::cout << "[QuUI] ShutdownInput\n"; }
    static void ShutdownRenderer()  { std::cout << "[QuUI] ShutdownRenderer\n"; }
    static void ShutdownPlatform()  { std::cout << "[QuUI] ShutdownPlatform\n"; }
    static void ShutdownLogging()   { std::cout << "[QuUI] ShutdownLogging\n"; }

    bool Initialize()
    {
        std::lock_guard<std::mutex> lk(s_initMutex);
        if (s_initialized.load(std::memory_order_acquire))
            return true;

        // Порядок инициализации: логирование -> платформа -> рендер -> ввод -> ресурсы
        InitLogging();
        InitPlatform();
        InitRenderer();
        InitInput();
        InitResources();

        s_initialized.store(true, std::memory_order_release);
        std::cout << "[QuUI] Initialized\n";
        return true;
    }

    void Shutdown()
    {
        std::lock_guard<std::mutex> lk(s_initMutex);
        if (!s_initialized.load(std::memory_order_acquire))
            return;

        // Обратный порядок при завершении
        ShutdownResources();
        ShutdownInput();
        ShutdownRenderer();
        ShutdownPlatform();
        ShutdownLogging();

        s_initialized.store(false, std::memory_order_release);
        std::cout << "[QuUI] Shutdown complete\n";
    }

    bool IsInitialized()
    {
        return s_initialized.load(std::memory_order_acquire);
    }

    const char* Version()
    {
        return "QuUI SDK 0.1.0";
    }
} // namespace QuUI

// Автоматическая инициализация при загрузке модуля / старте программы.
// Если вы не хотите автозапуск — удалите этот объект и вызывайте QuUI::Initialize() вручную.
namespace {
    struct AutoInit {
        AutoInit()  { QuUI::Initialize(); }
        ~AutoInit() { QuUI::Shutdown(); }
    } s_autoInit;
}

// C-совместимый API-обёртки (удобно для DLL/SO)
extern "C" {

bool QuUI_Initialize()    { return QuUI::Initialize(); }
void QuUI_Shutdown()      { QuUI::Shutdown(); }
bool QuUI_IsInitialized() { return QuUI::IsInitialized(); }
const char* QuUI_Version(){ return QuUI::Version(); }

}