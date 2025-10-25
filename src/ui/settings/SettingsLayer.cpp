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
            globed::alert("Error", "Cannot link while not connected to a server. Connect to a server and try again.");
            return;
        }

        DiscordLinkPopup::create()->show();
    }, CELL_SIZE));
    this->addSetting(ButtonSettingCell::create("Keybinds", "", "Edit", [this] {
        KeybindsPopup::create()->show();
    }, CELL_SIZE));

    // Player settings
    this->addHeader("core.player", "Players");
    this->addSetting<FloatSettingCell>("core.player.opacity", "Player Opacity", "Changes the visibility of all players in game.");
    this->addSetting<BoolSettingCell>("core.player.show-names", "Player Names", "Show players; names above their icons.");
    this->addSetting<BoolSettingCell>("core.player.dual-name", "Player Dual Names", "Show players' names above both icons when in dual mode.");
    this->addSetting<FloatSettingCell>("core.player.name-opacity", "Name Opacity", "");
    this->addSetting<BoolSettingCell>("core.player.force-visibility", "Force Visibility", "Forces all players to be visible regardless of distance.");
    this->addSetting<BoolSettingCell>("core.player.hide-nearby-classic", "Hide Nearby Players (Classic)", "");
    this->addSetting<BoolSettingCell>("core.player.hide-nearby-plat", "Hide Nearby Players (Plat)", "");
    this->addSetting<BoolSettingCell>("core.player.hide-practicing", "Hide Practicing Players", "Hides players who are in practice mode.");
    this->addSetting<BoolSettingCell>("core.player.status-icons", "Show Status Icons", "");
    this->addSetting<BoolSettingCell>("core.player.rotate-names", "Rotate Names", "Rotates names to match the camera angle.");
    this->addSetting<BoolSettingCell>("core.player.death-effects", "Death Effects", "Show player death effects.");
    this->addSetting<BoolSettingCell>("core.player.default-death-effects", "Default Death Effects", "Changes all players' death effects to the default effect. Only applicable if Death Effects is enabled.");

    // Level UI
    this->addHeader("core.level", "Level UI");
    this->addSetting<BoolSettingCell>("core.level.progress-indicators", "Progress Icons (Classic)", "");
    this->addSetting<BoolSettingCell>("core.level.progress-indicators-plat", "Progress Icons (Plat)", "");
    this->addSetting<FloatSettingCell>("core.level.progress-opacity", "Progress Opacity", "");
    this->addSetting<BoolSettingCell>("core.level.voice-overlay", "Voice Chat Overlay", "Shows who is speaking in the level.");
    this->addSetting<BoolSettingCell>("core.level.force-progressbar", "Force progressbar", "Forces the progress bar to always show in levels.");
    this->addSetting<BoolSettingCell>("core.level.self-status-icons", "Show Own Status Icons", "Shows you own status (muted, practice, etc) above your icon.");
    this->addSetting<BoolSettingCell>("core.level.self-name", "Show Own Name", "Show your own name above your icon.");

    // Overlay
    this->addHeader("core.overlay", "Ping overlay");
    this->addSetting<BoolSettingCell>("core.overlay.enabled", "Enable Overlay", "");
    this->addSetting<FloatSettingCell>("core.overlay.opacity", "Overlay Opacity", "");
    this->addSetting<IntCornerSettingCell>("core.overlay.position", "Overlay Position", "");
    this->addSetting<BoolSettingCell>("core.overlay.always-show", "Always Show Overlay", "");

    // Audio
    this->addHeader("core.audio", "Audio");
    this->addSetting<BoolSettingCell>("core.audio.voice-chat-enabled", "Voice Chat", "Enable in-game voice chat.");
    this->addSetting<FloatSettingCell>("core.audio.playback-volume", "Voice Volume", "Volume of other players' voices.");
    this->addSetting(ButtonSettingCell::create("Audio Device", "Enables you to change your audio input (microphone).", "Set", [this] {
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
    this->addSetting<BoolSettingCell>("core.preload.enabled", "Preload Assets", "Loads Globed assets (player icons, death effects, etc) when the game launches to reduce in-game loading times.");
    this->addSetting<BoolSettingCell>("core.preload.defer", "Defer Preloading", "");

    // Advanced settings
    this->addHeader("core.dev", "Advanced");
    this->addSetting<BoolSettingCell>("core.ui.allow-custom-servers", "Allow Custom Servers", "Allows connecting to custom user-hosted Globed servers.");

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