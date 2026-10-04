#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

static bool g_restartLocked = false;

class $modify(SafePauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto leftMenu = this->getChildByID("left-button-menu");
        if (!leftMenu) {
            leftMenu = CCMenu::create();
            this->addChild(leftMenu);
        }

        auto offSprite = ButtonSprite::create("Lock: OFF", "goldFont.fnt", "GJ_button_01.png", 0.6f);
        auto onSprite  = ButtonSprite::create("Lock: ON",  "goldFont.fnt", "GJ_button_02.png", 0.6f);

        auto toggleBtn = CCMenuItemToggler::create(
            offSprite,
            onSprite,
            this,
            menu_selector(SafePauseLayer::onToggleRestartLock)
        );
        toggleBtn->setID("restart-lock-toggle"_spr);
        toggleBtn->toggle(g_restartLocked);

        leftMenu->addChild(toggleBtn);
        leftMenu->updateLayout();
    }

    void onToggleRestartLock(CCObject* sender) {
        auto toggler = static_cast<CCMenuItemToggler*>(sender);
        g_restartLocked = toggler->isToggled();

        if (g_restartLocked) {
            Notification::create("Restart Lock: ENABLED", NotificationIcon::Warning, 1.0f)->show();
        } else {
            Notification::create("Restart Lock: DISABLED", NotificationIcon::Success, 1.0f)->show();
        }
    }

    void onRestart(CCObject* sender) {
        if (g_restartLocked) {
            FLAlertLayer::create(
                "Restart Blocked",
                "<cr>Restart Lock is currently ENABLED.</c>\nToggle it OFF in the pause menu to restart.",
                "OK"
            )->show();
            return;
        }
        PauseLayer::onRestart(sender);
    }

    void onRestartFull(CCObject* sender) {
        if (g_restartLocked) {
            FLAlertLayer::create(
                "Restart Blocked",
                "<cr>Restart Lock is currently ENABLED.</c>\nToggle it OFF in the pause menu to restart.",
                "OK"
            )->show();
            return;
        }
        PauseLayer::onRestartFull(sender);
    }
};
