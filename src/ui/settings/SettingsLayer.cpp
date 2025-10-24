#include "SettingsLayer.hpp"
#include "BoolSettingCell.hpp"
#include "FloatSettingCell.hpp"
#include "TitleSettingCell.hpp"
#include "ButtonSettingCell.hpp"
#include "DiscordLinkPopup.hpp"
#include "KeybindsPopup.hpp"
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

    // Audio
    this->addHeader("core.audio", "Audio");
    this->addSetting<BoolSettingCell>("core.audio.voice-chat-enabled", "Voice Chat", "");
    this->addSetting<FloatSettingCell>("core.audio.playback-volume", "Voice Volume", "");
    this->addSetting(ButtonSettingCell::create("Audio Device", "Allows to set a proper audio input device.", "Set", [this] {
        // TODO: popup with choosing audio device
    }, CELL_SIZE));
    this->addSetting<BoolSettingCell>("core.audio.voice-loopback", "Voice Loopback", "");

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

void SettingsLayer::addSetting(CCNode* cell) {
    m_list->addCell(cell);
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
