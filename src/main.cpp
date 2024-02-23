#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

using namespace geode::prelude;


class $modify(SaveBtnEditor, EditorUI) {
	bool init(LevelEditorLayer* editorLayer ) {
		if (!EditorUI::init(editorLayer)) return false;
		
		// make a button that says save
		auto buttonSprite = CircleButtonSprite::create(CCLabelBMFont::create("Save", "bigFont.fnt"), CircleBaseColor::Green, CircleBaseSize::Small);

		auto saveBtn = CCMenuItemSpriteExtra::create(
			buttonSprite, this, menu_selector(SaveBtnEditor::onSaveBtn)
		);

		saveBtn->setID("save_button"_spr);

		// menu to add button to (either settings or undo menu if betteredit is here)		
		auto menu = getSaveMenu();

		if (menu != nullptr) {
			menu->addChild(saveBtn);
			menu->updateLayout();
		}

		return true;
	}

	void onSaveBtn(CCObject* target) {
		// make save confirmation popup
		createQuickPopup(
			"Save",
			"<cy>Save</c> the level?",
			"NO", "YES",
			[&](auto, bool btn2) {
				if (btn2) {
					// create editor pause layer then save using editor pause menu
					auto editorPauseLayer = EditorPauseLayer::create(this->m_editorLayer);
					
					if (editorPauseLayer == nullptr) {
						FLAlertLayer::create(
							"ERROR",
							"Failed to save level.",
							"It's joever."
						)->show();
						
						return;
					}	

					editorPauseLayer->saveLevel();

					FLAlertLayer::create(
						"SUCCESS",
						"<cy>Saved</c> the level.",
						"OK"
					)->show();
				}
			}
		);
    }

	// Gets menu for save button from editor UI or returns nullptr if none was found
	// returns undo-menu if betteredit is present, otherwise returns settings menu
	CCNode* getSaveMenu() {
		auto menu = Loader::get()->getLoadedMod("hjfod.betteredit") ? this->getChildByID("undo-menu") : this->getChildByID("settings-menu");

		if (menu == nullptr) {
			FLAlertLayer::create(
				"ERROR", 
				"Failed to load savebtn menu :(",  	
				"It's joever"       			
			)->show();	
		}

		return menu;
	}

	// Gets save button from editor UI or returns nullptr if none was found
	CCNode* getSaveBtn() {
		auto menu = getSaveMenu();

		// if the menu is null then obviously there wont be a save button there so return null
		if (menu == nullptr) {
			return nullptr;
		}

		return menu->getChildByID("save_button"_spr);
	}

	
	void showUI(bool p0) {
		EditorUI::showUI(p0);
		
		// check playtest no ui
		bool playtestNoUI = GameManager::sharedState()->getGameVariable("0046");

		if (!playtestNoUI) {
			// don't fuck with visibility if playtest no ui is disabled
			return;
		}

		auto saveBtn = getSaveBtn();

		if (saveBtn != nullptr) {
			saveBtn->setVisible(p0);
		}
	}
};
