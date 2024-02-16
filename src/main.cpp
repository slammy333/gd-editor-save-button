#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

using namespace geode::prelude;

class $modify(SaveBtnEditor, EditorUI) {
	void onBidenBtn(CCObject* target) {
		// make confirmation popup
		createQuickPopup(
			"Save",
			"<cy>Save</c> the level?",
			"NO", "YES",
			[&](auto, bool btn2) {
				if (btn2) {
					// create editor pause layer then save maybe
					auto editorPauseLayer = EditorPauseLayer::create(this->m_editorLayer);
					
					// check if null
					if (editorPauseLayer == nullptr) {
						FLAlertLayer::create(
						"ERROR",
						"Editor pause layer was null. If this happens, something has definitely gone wrong.",
						"It's joever."
						)->show(); 
						
						return;
					}

					editorPauseLayer->saveLevel();
				}
			}
		);
    }

	bool init(LevelEditorLayer* editorLayer ) {
		if (!EditorUI::init(editorLayer)) return false;
		
		// make a button that says save
		auto buttonSprite = CircleButtonSprite::create(CCLabelBMFont::create("Save", "bigFont.fnt"), CircleBaseColor::Green, CircleBaseSize::Small);

		// ignore the fact that the save button is called bidenBtn.
		// dont worry about it
		auto bidenBtn = CCMenuItemSpriteExtra::create(
			buttonSprite, this, menu_selector(SaveBtnEditor::onBidenBtn)
		);

		// get the settings menu
		auto settingsMenu = this->getChildByID("settings-menu");

		if (settingsMenu == nullptr) {
			// settings menu failed to load :(

			// idk if this popup will work so hopefully it does if an epic fail happens
			FLAlertLayer::create(
				"ERROR", 
				"Failed to load settings menu :(",  	
				"It's joever"       			
			)->show();

			return true;
		}

		// add the button to the settings menu
		settingsMenu->addChild(bidenBtn);
		settingsMenu->updateLayout();

		return true;
	}
};
