#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>
class $modify(HollowMenu, MenuLayer) {
	struct Fields {
		float m_textScale = 0.5f;
		float m_textGapDist = m_textScale * 40.f;
		float m_menuTransitionSpeed = 0.5f;
		CCMenu* m_currentMenu; //Should point to any of the other menus :3
		CCMenu* m_hollowMenu;
		CCMenu* m_mainButtonsMenu;
		CCMenu* m_mainEditorMenu;
		CCMenu* m_mainOptionsMenu;
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

		m_fields->m_hollowMenu = CCMenu::create();
		m_fields->m_hollowMenu->setZOrder(59);
		m_fields->m_hollowMenu->setID("hollow-menu"_spr);
		m_fields->m_hollowMenu->ignoreAnchorPointForPosition(false);
		this->addChild(m_fields->m_hollowMenu);

		m_fields->m_mainButtonsMenu = CCMenu::create();
		m_fields->m_mainButtonsMenu->setID("main-buttons-menu");
		m_fields->m_mainButtonsMenu->ignoreAnchorPointForPosition(false);
		m_fields->m_hollowMenu->addChild(m_fields->m_mainButtonsMenu);

		m_fields->m_mainEditorMenu = CCMenu::create();
		m_fields->m_mainEditorMenu->setID("main-editor-menu");
		m_fields->m_mainEditorMenu->ignoreAnchorPointForPosition(false);
		m_fields->m_hollowMenu->addChild(m_fields->m_mainEditorMenu);

		m_fields->m_mainOptionsMenu = CCMenu::create();
		m_fields->m_mainOptionsMenu->setID("main-options-menu");
		m_fields->m_mainOptionsMenu->ignoreAnchorPointForPosition(false);
		m_fields->m_hollowMenu->addChild(m_fields->m_mainOptionsMenu);

		auto modSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
		modSpr->setScale(0.7f);
		auto modBtn = CCMenuItemSpriteExtra::create(modSpr, this, menu_selector(HollowMenu::onMods));
		modBtn->setPosition(ccp(winSize.width - 30.f, 30.f));
		modBtn->setID("geode-mod-button");
		m_fields->m_hollowMenu->addChild(modBtn);

		auto playBtn = createTextButton("Play", "play-button", ccp(winSize.width/2, winSize.height/2 + 0.f), menu_selector(MenuLayer::onPlay));
		auto editBtn = createTextButton("Editor", "editor-button", playBtn->getPosition() - ccp(0.f, m_fields->m_textGapDist), menu_selector(MenuLayer::onCreator));
		auto optBtn = createTextButton("Options", "options-button", editBtn->getPosition() - ccp(0.f, m_fields->m_textGapDist), menu_selector(HollowMenu::toMainOptions));
		auto achBtn = createTextButton("Achievements", "achievements-button", optBtn->getPosition() - ccp(0.f, m_fields->m_textGapDist), menu_selector(MenuLayer::onAchievements));
		auto exitBtn = createTextButton("Quit", "exit-button", achBtn->getPosition() - ccp(0.f, m_fields->m_textGapDist), menu_selector(MenuLayer::onQuit));
		m_fields->m_mainButtonsMenu->addChild(playBtn);
		m_fields->m_mainButtonsMenu->addChild(editBtn);
		m_fields->m_mainButtonsMenu->addChild(optBtn);
		m_fields->m_mainButtonsMenu->addChild(achBtn);
		m_fields->m_mainButtonsMenu->addChild(exitBtn);

		auto createBtn = createTextButton("Create", "create-button", ccp(winSize.width/2, winSize.height/2 + 0.f), menu_selector(CreatorLayer::onMyLevels));
		auto savedBtn = createTextButton("Saved", "saved-button", ccp(winSize.width/2, winSize.height/2 + 0.f), menu_selector(CreatorLayer::onSavedLevels));
		auto onlineBtn = createTextButton("Online Levels", "online-levels-button", savedBtn->getPosition() - ccp(0.f, m_fields->m_textGapDist), menu_selector(CreatorLayer::onOnlineLevels));
		auto origBtn = createTextButton("Original Creator Layer", "original-creator-layer-button", onlineBtn->getPosition() - ccp(0.f, m_fields->m_textGapDist), menu_selector(MenuLayer::onCreator));
		auto backBtn = createTextButton("Back", "back-button", onlineBtn->getPosition() - ccp(0.f, m_fields->m_textGapDist * 3), menu_selector(HollowMenu::toMainMenu));
		m_fields->m_mainEditorMenu->addChild(createBtn);
		m_fields->m_mainEditorMenu->addChild(savedBtn);
		m_fields->m_mainEditorMenu->addChild(onlineBtn);
		m_fields->m_mainEditorMenu->addChild(backBtn);
		m_fields->m_mainEditorMenu->setOpacity(0.f);
		m_fields->m_mainEditorMenu->setEnabled(false);

		auto origBtn = createTextButton("Original Options", "original-options-button", ccp(winSize.width/2, winSize.height/2 + 0.f), menu_selector(MenuLayer::onOptions));
		auto backBtn = createTextButton("Back", "back-button", origBtn->getPosition() - ccp(0.f, m_fields->m_textGapDist * 3), menu_selector(HollowMenu::toMainMenu));
		m_fields->m_mainOptionsMenu->addChild(origBtn);
		m_fields->m_mainOptionsMenu->addChild(backBtn);
		m_fields->m_mainOptionsMenu->setOpacity(0.f);
		m_fields->m_mainOptionsMenu->setEnabled(false);

		m_fields->m_currentMenu = m_fields->m_mainButtonsMenu;

		return true;
	}

	void toMainOptions(CCObject* sender) {
		fadeOutMenu();
		m_fields->m_currentMenu = m_fields->m_mainOptionsMenu;
		this->scheduleOnce(schedule_selector(HollowMenu::fadeInMenu), m_fields->m_menuTransitionSpeed);
	}

	void toMainEditor(CCObject* sender) {
		fadeOutMenu();
		m_fields->m_currentMenu = m_fields->m_mainEditorMenu;
		this->scheduleOnce(schedule_selector(HollowMenu::fadeInMenu), m_fields->m_menuTransitionSpeed);
	}

	void toMainMenu(CCObject* sender) {
		fadeOutMenu();
		m_fields->m_currentMenu = m_fields->m_mainButtonsMenu;
		this->scheduleOnce(schedule_selector(HollowMenu::fadeInMenu), m_fields->m_menuTransitionSpeed);
	}

	void fadeOutMenu() {
		m_fields->m_currentMenu->runAction(CCFadeOut::create(m_fields->m_menuTransitionSpeed));
		m_fields->m_currentMenu->setEnabled(false);
	}

	void fadeInMenu(float) {
		m_fields->m_currentMenu->runAction(CCFadeIn::create(m_fields->m_menuTransitionSpeed));
		this->scheduleOnce(schedule_selector(HollowMenu::fadeInMenu2), m_fields->m_menuTransitionSpeed);
	}

	void fadeInMenu2(float) {
		m_fields->m_currentMenu->setEnabled(true);
	}

	void onMods(CCObject* sender) {
		openModsList();
	}
};