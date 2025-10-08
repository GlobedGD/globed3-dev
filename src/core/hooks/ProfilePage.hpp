#pragma once

#include <globed/config.hpp>
#include <Geode/Geode.hpp>
#include <Geode/modify/ProfilePage.hpp>

#include <globed/core/data/Messages.hpp>
#include <globed/core/net/MessageListener.hpp>

#include <cue/Util.hpp>

namespace globed {

struct GLOBED_MODIFY_ATTR HookedProfilePage : geode::Modify<HookedProfilePage, ProfilePage> {
	struct Fields {
    	std::optional<MessageListener<msg::FetchUserResponseMessage>> m_listener;

		std::vector<uint8_t> m_roles;

		cocos2d::CCSprite* m_roleIcon = nullptr;
	};

	bool init(int accountID, bool ownProfile);
	void loadPageFromUserInfo(GJUserScore* score);

	void fetchUserRoles(int accountId);

	void updateUserRoleIcon(const msg::FetchUserResponseMessage& msg);
};

}
