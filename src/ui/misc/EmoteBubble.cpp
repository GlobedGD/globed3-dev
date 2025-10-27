#include "EmoteBubble.hpp"

#include <UIBuilder.hpp>
#include <cue/Util.hpp>

using namespace geode::prelude;

namespace globed {

EmoteBubble* EmoteBubble::create() {
    auto ret = new EmoteBubble();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool EmoteBubble::init() {
    if (!CCNode::init()) return false;

    m_emoteSpr = CCSprite::createWithSpriteFrameName("emote_0.png"_spr);

    m_bubbleSpr = Build<CCSprite>::createSpriteName("bubble_emote_spr.png"_spr)
        .anchorPoint(0, 0)
        .scale(0.3f)
        .parent(this);
    
    m_emoteSpr->setPosition((m_bubbleSpr->getContentSize() / 2.f) + CCPoint{0, 15.f});
    m_bubbleSpr->addChild(m_emoteSpr);

    this->setVisible(false);

    return true;
}

void EmoteBubble::playEmoteSelf(int emoteId) {
    auto cache = CCSpriteFrameCache::sharedSpriteFrameCache();

    std::string newFrameName = fmt::format("emote_{}.png"_spr, emoteId).c_str();

    if (cache->spriteFrameByName(newFrameName.c_str())) {
        auto newFrame = cache->spriteFrameByName(newFrameName.c_str());
        m_emoteSpr->setDisplayFrame(newFrame);
        cue::rescaleToMatch(m_emoteSpr, {50.f, 50.f});
        
        m_bubbleSpr->setScale(0.2f);
        m_bubbleSpr->runAction(
            CCSequence::create(
                CCEaseExponentialOut::create(CCScaleTo::create(0.4f, 0.5f)),
                CCDelayTime::create(1.7f),
                CCEaseExponentialIn::create(CCScaleTo::create(0.4f, 0.2f)),
                nullptr
            )
        );
        
        m_emoteSpr->runAction(
        CCSpawn::create(
                CCEaseExponentialOut::create(CCMoveBy::create(0.6f, {0, 10.f})),
                CCEaseBounceOut::create(CCMoveBy::create(0.75f, {0, -10.f})),
                0
            )
        );

        // TODO: Play the sound effect if available
        // should check to see if "emote_sfx_emoteId.ogg" exists in the emotes_sfx folder
        // if it does, then play the sound effect around the player that had the emote bubble appear for
        
        auto fmod = FMODAudioEngine::sharedEngine();
        std::string path = fmt::format("emote_sfx_{}.ogg"_spr, emoteId).c_str();
        fmod->playEffect(path, 1.f + (CCRANDOM_MINUS1_1() * 0.08f), 1.f, 0.75f);


        this->setVisible(true);
        this->runAction(
            CCSequence::create(
                CCDelayTime::create(2.5f),
                CCCallFuncN::create(this, callfuncN_selector(EmoteBubble::setInvisible)),
                nullptr
            )
        );
        
    }
}

void EmoteBubble::setInvisible(CCNode* sender) {
    this->setVisible(false);
}

}