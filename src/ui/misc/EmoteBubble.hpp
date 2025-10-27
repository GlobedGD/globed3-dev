#pragma once

#include <Geode/Geode.hpp>

namespace globed {

class EmoteBubble : public cocos2d::CCNode {
public:
    static EmoteBubble* create();
    void playEmoteSelf(int emoteId);

private:
    cocos2d::CCSprite* m_emoteSpr = nullptr;
    cocos2d::CCSprite* m_bubbleSpr = nullptr;

    void setInvisible(CCNode* sender);
    bool init();
};

}