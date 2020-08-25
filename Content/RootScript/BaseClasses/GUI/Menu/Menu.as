shared abstract class Menu {
    private string _type;
    string type {
        get { return _type; }
    }

    array<GUIComponent@> components;

    Menu(const string&in type) {
        _type = type;
    }

    void onActivate() {}
    void onDeactivate() {}

    // Returns whether this menu should detach.
    bool onEscapeHit() { return false; }

    // Returns whether this menu should detach.
    bool update() { return false; }
}

shared class MenuManager {
    Menu@ currMenu;

    void deactivateMenu(Menu@ mu) {
        if (@currMenu == @mu) {
            @currMenu = null;
            mu.onDeactivate();
            for (int i = 0; i < mu.components.length(); i++) {
                mu.components[i].onClose();
                for (int j = 0; j < mu.components[i].children.length(); j++) {
                    mu.components[i].children[j].onClose();
                }
            }
        } else {
            // Throw
        }
        World::paused = false;
    }

    void activateMenu(Menu@ mu) {
        if (currMenu == null) {
            @currMenu = mu;
            mu.onActivate();
            for (int i = 0; i < mu.components.length(); i++) {
                mu.components[i].onOpen();
                for (int j = 0; j < mu.components[i].children.length(); j++) {
                    mu.components[i].children[j].onOpen();
                }
            }
        } else {
            // Throw
        }
        World::paused = true;
    }

    void update() {
        if (Input::Escape::isHit()) {
            if (GUITextInput::subscriber != null) {
                GUITextInput::subscriber.deselect();
            } else if (currMenu == null) {
                activateMenu(PauseMenu::instance);
            } else if (currMenu.onEscapeHit()) {
                deactivateMenu(currMenu);
            }
        }
        if (currMenu == null) {
            if (Input::getHit() & Input::Inventory != 0) {
                activateMenu(InventoryMenu::instance);
            }
        } else {
            if (currMenu.update()) {
                deactivateMenu(currMenu);
            } else {
                for (int i = 0; i < currMenu.components.length(); i++) {
                    if (currMenu.components[i].active) {
                        currMenu.components[i].update();
                        for (int j = 0; j < currMenu.components[i].children.length(); j++) {
                            if (currMenu.components[i].children[j].active) {
                                currMenu.components[i].children[j].update();
                            }
                        }
                    }
                }
            }
        }
    }

    void render() {
        if (currMenu != null) {
            for (int i = 0; i < currMenu.components.length(); i++) {
                if (currMenu.components[i].active) {
                    currMenu.components[i].render();
                    for (int j = 0; j < currMenu.components[i].children.length(); j++) {
                        if (currMenu.components[i].children[j].active) {
                            currMenu.components[i].children[j].render();
                        }
                    }
                }
            }
        }
    }
}
