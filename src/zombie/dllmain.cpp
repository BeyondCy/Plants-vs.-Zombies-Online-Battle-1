/**
 * @file dllmain.cpp
 * @brief The `DllMain` of the zombie side.
 *
 * @author Chen Zhenshuo (chenzs108@outlook.com)
 * @author Liu Guowen (liugw01@outlook.com)
 * @version 1.0
 * @date 2021-07-10
 * @par GitHub
 * https://github.com/czs108
 * @par
 * https://github.com/lgw1995
 */

#define _WINSOCKAPI_

#include "config.h"
#include "startup.h"

#include <Windows.h>

#include <exception>
#include <filesystem>
#include <format>
#include <memory>
#include <string_view>


using namespace game;

namespace {

constexpr std::string_view CFG_FILE = "online_config.ini";

extern std::unique_ptr<Startup> startup;

}  // namespace


BOOL APIENTRY DllMain(HMODULE, const DWORD reason, LPVOID) {
    try {
        switch (reason) {
            case DLL_PROCESS_ATTACH: {
                startup = std::make_unique<Startup>(
                    Role::Zombie,
                    Config{ std::filesystem::absolute(CFG_FILE).string() });
                startup->Run();
                break;
            }
            case DLL_PROCESS_DETACH: {
                startup.reset();
                break;
            }
            default: {
                break;
            }
        }

        return TRUE;

    } catch (const std::exception& err) {
        const auto msg{ std::format(
            "Failed to load the online modification: {}", err.what()) };
        OutputDebugStringA(msg.c_str());
        return FALSE;
    }
}


namespace {
std::unique_ptr<Startup> startup{};
}  // namespace