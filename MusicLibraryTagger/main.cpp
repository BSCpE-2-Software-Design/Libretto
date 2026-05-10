#include <iostream>
#include "Library.h"
#include "FilterEngine.h"
#include "BrowserView.h"

int main() {
    Library lib;
    // Attempt to load a library file; if you have a different filename change it here
    lib.loadFromJson("library.json");

    FilterEngine fe;
    BrowserView view(lib, fe);

    bool running = true;
    while (running) {
        view.render();
        char c = view.getInput();
        switch (c) {
        case 'j':
            view.moveUp();
            break;
        case 'k':
            view.moveDown();
            break;
        case 'f':
            view.addFilter();
            break;
        case 'x':
            view.clearFilters();
            break;
        case 's':
            view.savePlaylist();
            break;
        case 'q':
            running = false;
            break;
        default:
            // ignore unknown commands
            break;
        }
    }

    return 0;
}
