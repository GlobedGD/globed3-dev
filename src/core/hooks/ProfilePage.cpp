#include "ProfilePage.hpp"
#include <UIBuilder.hpp>

#include <core/net/NetworkManagerImpl.hpp>
#include "globed/core/data/Messages.hpp"
#include <globed/core/net/NetworkManager.hpp>

#include <ui/misc/Badges.hpp>

static inline std::unordered_map<int, std::vector<uint8_t>> playerRoles;

using namespace geode::prelude;

namespace globed {

void HookedProfilePage::loadPageFromUserInfo(GJUserScore* score) {
	ProfilePage::loadPageFromUserInfo(score);

	if (NetworkManager::get().getConnectionState() != ConnectionState::Connected)
		return;

	auto& nm = NetworkManagerImpl::get();
	
	if (playerRoles.contains(score->m_accountID)) {
		m_fields->m_roles = playerRoles[score->m_accountID];
		updateUserRoleIcon();
		return;
	}

	m_fields->m_listener = nm.listen<msg::FetchUserResponseMessage>([this](const auto& msg) {
		m_fields->m_roles = std::move(msg.roles);
		this->updateUserRoleIcon();
		return ListenerResult::Stop;
	});

	if (score->m_accountID!= 0)
		this->fetchUserRoles(score->m_accountID);
}


void HookedProfilePage::fetchUserRoles(int accountId) {
	cue::resetNode(m_fields->m_roleIcon);

	auto& nm = NetworkManagerImpl::get();

	nm.sendFetchUser(accountId);
}

void HookedProfilePage::updateUserRoleIcon() {
	auto fields = m_fields.self();

	cue::resetNode(fields->m_roleIcon);

	if (!fields->m_roles.empty()) {
		CCMenu *usernameMenu = static_cast<CCMenu*>(m_mainLayer->getChildByIDRecursive("username-menu"));
		if (!usernameMenu)
			return;

		fields->m_roleIcon = createBadge(fields->m_roles[0]);

		fields->m_roleIcon->setID("user-badge"_spr);
		fields->m_roleIcon->setScale(0.65f);

		usernameMenu->addChild(fields->m_roleIcon);
		usernameMenu->updateLayout();

	}

	if (!playerRoles.contains(m_score->m_accountID))
		playerRoles[m_score->m_accountID] = fields->m_roles;
}

}
