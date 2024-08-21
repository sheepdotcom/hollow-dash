#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>
class $modify(HollowMenu, MenuLayer) {
	struct Fields {
		float m_textGapDist = 40.f;
		float m_textScale = 0.5f;
	};

	static void onModify(auto& self) {
		self.setHookPriority("MenuLayer::init", INT_MIN/2 - 100);
	}

	CCMenuItemSpriteExtra* createTextButton(std::string name, std::string id, CCPoint pos, SEL_MenuHandler callback) {
		auto text = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
		text->setScale(m_fields->m_textScale);
		auto btn = CCMenuItemSpriteExtra::create(text, this, callback);
		btn->setPosition(pos);
		btn->setID(id);
		return btn;
	}

	bool init() {
		auto winSize = CCDirector::sharedDirector()->getWinSize();
		if (!MenuLayer::init()) return false;

		auto nodes = CCArrayExt<CCNode*>(this->getChildren());
		for (auto node : nodes) {
			node->setVisible(false);
		}

		auto hollowMenu = CCMenu::create();
		hollowMenu->setZOrder(59);
		hollowMenu->setID("hollow-menu"_spr);
		hollowMenu->ignoreAnchorPointForPosition(false);
		this->addChild(hollowMenu);

		auto mainButtonsMenu = CCMenu::create();
		mainButtonsMenu->setID("main-buttons-menu");
		mainButtonsMenu->ignoreAnchorPointForPosition(false);
		hollowMenu->addChild(mainButtonsMenu);

		auto modSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
		modSpr->setScale(0.7f);
		auto modBtn = CCMenuItemSpriteExtra::create(modSpr, this, menu_selector(HollowMenu::onMods));
		modBtn->setPosition(ccp(winSize.width - 30.f, 30.f));
		modBtn->setID("geode-mod-button");
		hollowMenu->addChild(modBtn);

		auto playBtn = createTextButton("Play", "play-button", ccp(winSize.width/2, winSize.height/2 + 0.f), menu_selector(MenuLayer::onPlay));
		auto editBtn = createTextButton("Editor", "editor-button", playBtn->getPosition() - ccp(0.f, m_fields->m_textGapDist * m_fields->m_textScale), menu_selector(MenuLayer::onCreator));
		auto optBtn = createTextButton("Options", "options-button", editBtn->getPosition() - ccp(0.f, m_fields->m_textGapDist * m_fields->m_textScale), menu_selector(MenuLayer::onOptions));
		auto achBtn = createTextButton("Achievements", "achievements-button", optBtn->getPosition() - ccp(0.f, m_fields->m_textGapDist * m_fields->m_textScale), menu_selector(MenuLayer::onAchievements));
		auto exitBtn = createTextButton("Quit", "exit-button", achBtn->getPosition() - ccp(0.f, m_fields->m_textGapDist * m_fields->m_textScale), menu_selector(MenuLayer::onQuit));
		mainButtonsMenu->addChild(playBtn);
		mainButtonsMenu->addChild(editBtn);
		mainButtonsMenu->addChild(optBtn);
		mainButtonsMenu->addChild(achBtn);
		mainButtonsMenu->addChild(exitBtn);

		return true;
	}

	void onMods(CCObject* sender) {
		openModsList();
	}
};