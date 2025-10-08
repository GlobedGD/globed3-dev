#include "ProfilePage.hpp"
#include <UIBuilder.hpp>

#include <core/net/NetworkManagerImpl.hpp>
#include "globed/core/data/Messages.hpp"

#include <ui/misc/Badges.hpp>

using namespace geode::prelude;

namespace globed {

bool HookedProfilePage::init(int accountId, bool ownProfile) {
	if (!ProfilePage::init(accountId, ownProfile))
		return false;
	return true;
}

void HookedProfilePage::loadPageFromUserInfo(GJUserScore* score) {
	ProfilePage::loadPageFromUserInfo(score);

	auto& nm = NetworkManagerImpl::get();

	m_fields->m_listener = nm.listen<msg::FetchUserResponseMessage>([this](const auto& msg) {
		this->updateUserRoleIcon(msg);
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

void HookedProfilePage::updateUserRoleIcon(const msg::FetchUserResponseMessage& msg) {
	auto fields = m_fields.self();

	fields->m_roles = std::move(msg.roles);

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
}

}
