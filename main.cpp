#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

// Global flag to track whether restarting is blocked
static bool g_restartLocked = false;

class $modify(SafePauseLayer, PauseLayer) {
    void customSetup() {
        // Run original PauseLayer setup
        PauseLayer::customSetup();

        // Get the left button menu on the pause screen
        auto leftMenu = this->getChildByID("left-button-menu");
        if (!leftMenu) {
            leftMenu = CCMenu::create();
            this->addChild(leftMenu);
        }

        // Create 'OFF' (normal) and 'ON' (active) button visuals
        auto offSprite = ButtonSprite::create("Lock: OFF", "goldFont.fnt", "GJ_button_01.png", 0.6f);
        auto onSprite  = ButtonSprite::create("Lock: ON",  "goldFont.fnt", "GJ_button_02.png", 0.6f);

        // Create a toggle button component
        auto toggleBtn = CCMenuItemToggler::create(
            offSprite,
            onSprite,
            this,
            menu_selector(SafePauseLayer::onToggleRestartLock)
        );
        toggleBtn->setID("restart-lock-toggle"_spr);
        
        // Restore state if pause menu is re-opened
        toggleBtn->toggle(g_restartLocked);

        // Add to pause menu layout
        leftMenu->addChild(toggleBtn);
        leftMenu->updateLayout();
    }

    void onToggleRestartLock(CCObject* sender) {
        auto toggler = static_cast<CCMenuItemToggler*>(sender);
        // Toggler updates its internal state when tapped
        g_restartLocked = toggler->isToggled();

        if (g_restartLocked) {
            Notification::create("Restart Lock: ENABLED", NotificationIcon::Warning, 1.0f)->show();
        } else {
            Notification::create("Restart Lock: DISABLED", NotificationIcon::Success, 1.0f)->show();
        }
    }

    // Intercept standard restart button
    void onRestart(CCObject* sender) {
        if (g_restartLocked) {
            FLAlertLayer::create(
                "Restart Blocked",
                "<cr>Restart Lock is currently ENABLED.</c>\nToggle it OFF in the pause menu to restart.",
                "OK"
            )->show();
            return; // Block execution
        }
        PauseLayer::onRestart(sender);
    }

    // Intercept full restart button (e.g. from practice mode reset)
    void onRestartFull(CCObject* sender) {
        if (g_restartLocked) {
            FLAlertLayer::create(
                "Restart Blocked",
                "<cr>Restart Lock is currently ENABLED.</c>\nToggle it OFF in the pause menu to restart.",
                "OK"
            )->show();
            return; // Block execution
        }
        PauseLayer::onRestartFull(sender);
    }
};
