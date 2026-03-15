#pragma once

#include "editor/Editor.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace editor
{

    class EditorRenderer
    {
    public:
        EditorRenderer(sf::RenderWindow &window);

        void draw(const Editor &editor);

    private:
        void drawGrid(const Editor &editor);
        void drawCursor(const Editor &editor);
        void drawStartMarker(const Editor &editor);
        void drawMetadata(const Editor &editor);
        void drawSavePrompt();

        sf::RenderWindow &window_;
        sf::Font font_;
    };

}
