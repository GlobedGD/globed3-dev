#include "FloatSettingCell.hpp"

#include <UIBuilder.hpp>

using namespace geode::prelude;

namespace globed {

void FloatSettingCell::setup() {
    m_slider = Build(createSlider())
        .contentSize(100.f, 18.f)
        .pos(m_size.width - 11.f, m_size.height / 2.f)
        .anchorPoint(1.0f, 0.5f)
        .parent(this)
        .collect();

    // get limits
    auto limits = SettingsManager::get().getLimits(m_key);
    if (limits && limits->first.isNumber() && limits->second.isNumber()) {
        m_slider->setRange(limits->first.asDouble().unwrap(), limits->second.asDouble().unwrap());
    }

    m_slider->setCallback([this](auto, double value) {
        this->set(value);
        this->reload();
    });

    m_label = Build<CCLabelBMFont>::create("", "bigFont.fnt")
        .scale(0.35f)
        .pos(m_size.width - 140.f, m_size.height / 2.f)
        .parent(this)
        .collect();
    
    auto floatBG = Build<CCScale9Sprite>::create("square02b_small.png")
        .anchorPoint(0.5f, 0.5f)
        .contentSize(37.f, 20.f)
        .color(0, 0, 0)
        .opacity(90)
        .id("float-background")
        .pos(m_label->getPosition())
        .parent(this)
        .zOrder(m_label->getZOrder() - 1)
        .collect();

    this->reload();
}

void FloatSettingCell::reload() {
    float value = this->get<float>();
    m_slider->setValue(value);
    m_label->setString(fmt::format("{}%", (int)(value * 100)).c_str());
}

}