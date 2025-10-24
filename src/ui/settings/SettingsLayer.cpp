#include "SettingsLayer.hpp"
#include "AudioDeviceSetupPopup.hpp"
#include "cells/BoolSettingCell.hpp"
#include "cells/FloatSettingCell.hpp"
#include "cells/TitleSettingCell.hpp"
#include "cells/ButtonSettingCell.hpp"
#include "cells/IntSliderSettingCell.hpp"
#include "cells/IntCornerSettingCell.hpp"
#include "DiscordLinkPopup.hpp"
#include "KeybindsPopup.hpp"
#include "SaveSlotSwitcherPopup.hpp"
#include <globed/core/PopupManager.hpp>
#include <core/net/NetworkManagerImpl.hpp>

#include <UIBuilder.hpp>

using namespace geode::prelude;

namespace globed {

bool SettingsLayer::init() {
    BaseLayer::init(true);

    auto winSize = CCDirector::get()->getWinSize();

    m_list = Build(cue::ListNode::create({356.f, 220.f}, {191, 114, 62, 255}))
        .pos(winSize / 2.f)
        .parent(this);
    m_list->setAutoUpdate(false);

    this->addSettings();

    auto rightMenu = Build<CCMenu>::create()
        .layout(ColumnLayout::create()->setAxisAlignment(AxisAlignment::Start))
        .anchorPoint(1.f, 0.f)
        .pos(winSize.width - 8.f, 8.f)
        .contentSize(48.f, winSize.height)
        .id("right-side-menu")
        .parent(this)
        .collect();

    // Reset settings button
    auto resetBtn = Build<CCSprite>::createSpriteName("GJ_deleteBtn_001.png")
        .intoMenuItem([this](auto) {
            geode::createQuickPopup("Reset all settings", "Are you sure you want to reset all settings? This action is <cr>irreversible.</c>", "Cancel", "Ok", [this](auto, bool accepted) {
                if (accepted) {
                    SettingsManager::get().reset();
                    this->refreshAll();
                }
            });
        })
        .id("btn-reset")
        .parent(rightMenu)
        .collect();

    // Save slot button
    Build<CircleButtonSprite>::create(CCSprite::create("icon-folder-settings.png"_spr), CircleBaseColor::Pink)
        .with([&](auto* item) { cue::rescaleToMatch(item, resetBtn); })
        .intoMenuItem([this](auto) {
            auto popup = SaveSlotSwitcherPopup::create();
            popup->setSwitchCallback([this] {
                this->refreshAll();
            });
            popup->show();
        })
        .id("btn-save-slots")
        .parent(rightMenu);

    rightMenu->updateLayout();

    return true;
}

void SettingsLayer::addSettings() {
    // TODO: descriptions
    // Globed
    this->addHeader("core.server", "Server");
    this->addSetting<BoolSettingCell>("core.autoconnect", "Autoconnect", "Automatically connects to your last used server on game startup.");
    this->addSetting<BoolSettingCell>("core.ui.increase-level-list", "Increase Level List", "Increases the levels present per page.");
    this->addSetting<BoolSettingCell>("core.ui.compressed-player-count", "Simple Player Count", "Uses an icon instead of 'X Players' text.");
    this->addSetting(ButtonSettingCell::create("Discord Linking", "Connects your Geometry Dash account to your Discord account via a unique code.", "Link", [this] {
        if (!NetworkManagerImpl::get().isConnected()) {
            globed::alert("Error", "Cannot link while not connected to a server!\nConnect to any server and try again.");
            return;
        }

        DiscordLinkPopup::create()->show();
    }, CELL_SIZE));
    this->addSetting(ButtonSettingCell::create("Keybinds", "", "Edit", [this] {
        KeybindsPopup::create()->show();
    }, CELL_SIZE));

    // Player settings
    this->addHeader("core.player", "Players");
    this->addSetting<FloatSettingCell>("core.player.opacity", "Player Opacity", "Changes the opacity of other players.");
    this->addSetting<BoolSettingCell>("core.player.show-names", "Player Names", "Shows player names above their icons.");
    this->addSetting<BoolSettingCell>("core.player.dual-name", "Player Dual Names", "Shows the players name above their Player 1 and Player 2 icons.");
    this->addSetting<FloatSettingCell>("core.player.name-opacity", "Name Opacity", "Changes the opacity of player names.");
    this->addSetting<BoolSettingCell>("core.player.force-visibility", "Force Visibility", "Forces player visibility in levels.");
    this->addSetting<BoolSettingCell>("core.player.hide-nearby", "Hide Nearby Players", "Hide players that are close to you.");
    this->addSetting<BoolSettingCell>("core.player.hide-practicing", "Hide Practicing Players", "");
    this->addSetting<BoolSettingCell>("core.player.status-icons", "Show Status Icons", "Shows icons for players that are in practice mode, have muted voice chat, etc.");
    this->addSetting<BoolSettingCell>("core.player.rotate-names", "Rotate Names", "Rotates names with the camera.");
    this->addSetting<BoolSettingCell>("core.player.death-effects", "Death Effects", "Show players' death effects.");
    this->addSetting<BoolSettingCell>("core.player.default-death-effects", "Default Death Effects", "Changes all other players' death effects to the default one.");

    // Level UI
    this->addHeader("core.level", "Level UI");
    this->addSetting<BoolSettingCell>("core.level.progress-indicators", "Progress Icons", "Shows players icons on the progress bar.");
    this->addSetting<FloatSettingCell>("core.level.progress-opacity", "Progress Opacity", "");
    this->addSetting<BoolSettingCell>("core.level.voice-overlay", "Voice Chat Overlay", "Shows an overlay of players currently talking in voice chat.\nThis is useful for reporting players.");
    this->addSetting<BoolSettingCell>("core.level.force-progressbar", "Force Progress Bar", "Forces the progress bar to always be visible.");
    this->addSetting<BoolSettingCell>("core.level.progress-indicators", "Progress Icons (Classic)", "");
    this->addSetting<BoolSettingCell>("core.level.progress-indicators-plat", "Progress Icons (Plat)", "");
    this->addSetting<FloatSettingCell>("core.level.progress-opacity", "Progress Opacity", "");
    this->addSetting<BoolSettingCell>("core.level.voice-overlay", "Voice Chat Overlay", "");
    this->addSetting<BoolSettingCell>("core.level.self-status-icons", "Show Own Status Icons", "");
    this->addSetting<BoolSettingCell>("core.level.self-name", "Show Own Name", "");

    // Overlay
        AudioDeviceSetupPopup::create()->show();
    }, CELL_SIZE));
    this->addSetting<BoolSettingCell>("core.audio.voice-proximity", "Voice Proximity (Plat)", "");
    this->addSetting<BoolSettingCell>("core.audio.classic-proximity", "Voice Proximity (Classic)", "");
    this->addSetting<BoolSettingCell>("core.audio.deafen-notification", "Deafen Notification", "");
    this->addSetting<BoolSettingCell>("core.audio.only-friends", "Friends Only Voice", "");
    this->addSetting<BoolSettingCell>("core.audio.voice-loopback", "Voice Loopback", "");
    auto bufferSize = this->addSetting<IntSliderSettingCell>("core.audio.buffer-size", "Audio Buffer Size", "");

    // Preload
    this->addHeader("core.player", "Preloading");
    this->addSetting<BoolSettingCell>("core.preload.enabled", "Preload Assets", "Load Globed assets on game start.");
    this->addSetting<BoolSettingCell>("core.preload.defer", "Defer Preloading", "");

    // Advanced settings
    this->addHeader("core.dev", "Advanced");
    this->addSetting<BoolSettingCell>("core.ui.allow-custom-servers", "Allow Custom Servers", "Allows adding and connecting to custom user-hosted servers.");

#ifdef GLOBED_DEBUG
    bool showDebug = true;
#else
    bool showDebug = globed::value<bool>("core.dev.enable-dev-settings").value_or(false);
#endif

    if (showDebug) {
        this->addSetting<FloatSettingCell>("core.dev.packet-loss-sim", "Packet Loss Simulation", "");
        this->addSetting<BoolSettingCell>("core.dev.net-debug-logs", "Network Debug Logs", "");
        this->addSetting<BoolSettingCell>("core.dev.fake-data", "Use Fake Data", "");
    }

    m_list->updateLayout();
}

void SettingsLayer::addHeader(CStr key, CStr text) {
    this->addSetting<TitleSettingCell>(key, text, "");
}

void SettingsLayer::addSetting(BaseSettingCellBase* cell) {
    m_list->addCell(cell);
}

void SettingsLayer::refreshAll() {
    for (auto cell : m_list->iter<BaseSettingCellBase>()) {
        cell->reload();
    }
}

SettingsLayer* SettingsLayer::create() {
    auto ret = new SettingsLayer;
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

}
