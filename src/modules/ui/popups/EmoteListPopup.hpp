#pragma once

#include <ui/BasePopup.hpp>
#include <cue/ListNode.hpp>
#include <vector>

namespace globed {

class EmoteListPopup : public BasePopup<EmoteListPopup> {
public:
    static const cocos2d::CCSize POPUP_SIZE;
    
    int m_selectedEmoteId = -1;
    bool m_isFavoriteMode = false;

private:
    int m_selectedPage = 0;
    int m_maxPages;
    int m_selectingFavoriteSlot = -1;
    
    std::vector<int> m_validEmoteIds;
    std::vector<int> m_favoriteEmoteIds;

    cocos2d::CCMenu* m_emoteMenu;
    cocos2d::CCMenu* m_favoriteEmotesMenu;
        
    CCMenuItemSpriteExtra* m_clearFavoriteBtn;

    cue::ListNode* m_list;
    cue::ListNode* m_bottomList;
    
    CCMenuItemSpriteExtra* m_submitBtn;
    ButtonSprite* m_submitBtnSpr;

    CCMenuItemSpriteExtra* m_leftPageBtn;
    CCMenuItemSpriteExtra* m_rightPageBtn;

    cocos2d::CCLabelBMFont* m_pageLabel;

    cocos2d::extension::CCScale9Sprite* m_favoriteHighlight;
    cocos2d::CCLabelBMFont* m_favoriteInfoLabel;

    bool setup() override;
    void onSubmitBtn();
    void onEmoteBtn(int emoteId);
    void loadEmoteListPage(int page);
    void loadFavoriteEmotesList();
    void updatePage(bool increment);
    void enterFavoriteSelectMode(int emoteSlot);
    void setFavorite(int emoteSlot, int id);
    std::vector<int> getValidEmoteIds(int maxTries); // scuffed but should work
};

}