#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>
class $modify(HollowMenu, MenuLayer) {
	static void onModify(auto & self) {
		self.setHookPriority("MenuLayer::init", INT_MIN/2 - 100);
	}

	bool init() {
		auto winSize = CCDirector::sharedDirector()->getWinSize();
		if (!MenuLayer::init()) return false;

		auto nodes = CCArrayExt<CCNode*>(this->getChildren());
		for (auto node : nodes) {
			node->setVisible(false);
		}

		auto hollowMenu = CCMenu::create();
		hollowMenu->setID("hollow-menu"_spr);
		hollowMenu->ignoreAnchorPointForPosition(false);
		this->addChild(hollowMenu);

		auto mainButtonsMenus = CCMenu::create();
		mainButtonsMenus->setID("main-buttons-menu");
		mainButtonsMenus->ignoreAnchorPointForPosition(false);
		hollowMenu->addChild(mainButtonsMenus);

		auto modSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
		modSpr->setScale(0.7f);
		auto modBtn = CCMenuItemSpriteExtra::create(modSpr, this, menu_selector(HollowMenu::onMods));
		modBtn->setPosition(ccp(winSize.width - 30.f, 30.f));
		modBtn->setID("geode-mod-button");
		mainButtonsMenus->addChild(modBtn);

		auto playText = CCLabelBMFont::create("Play", "bigFont.fnt");
		auto playBtn = CCMenuItemSpriteExtra::create(playText, this, menu_selector(MenuLayer::onPlay));
		playBtn->setPosition(winSize / 2 + ccp(0.f, 20.f));
		playBtn->setID("play-button");
		mainButtonsMenus->addChild(playBtn);

		auto editorText = CCLabelBMFont::create("Editor", "bigFont.fnt");
		auto editorBtn = CCMenuItemSpriteExtra::create(editorText, this, menu_selector(MenuLayer::onCreator));
		editorBtn->setPosition(winSize / 2 - ccp(0.f, 20.f));
		editorBtn->setID("editor-button");
		mainButtonsMenus->addChild(editorBtn);

		auto quitText = CCLabelBMFont::create("Quit", "bigFont.fnt");
		auto quitBtn = CCMenuItemSpriteExtra::create(quitText, this, menu_selector(MenuLayer::onQuit));
		quitBtn->setPosition(winSize / 2 - ccp(0.f, 60.f));
		quitBtn->setID("quit-button");
		mainButtonsMenus->addChild(quitBtn);

		return true;
	}

	void onMods(CCObject* sender) {
		openModsList();
	}
};