#include "editor/Editor.hpp"
#include "editor/EditorRenderer.hpp"
#include "editor/LevelFileIO.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: mato-editor <filepath.level>\n";
        return 1;
    }

    std::string filepath = argv[1];

    editor::LevelData levelData;
    try
    {
        levelData = editor::loadLevel(filepath);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading level: " << e.what() << "\n";
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode(720, 600), "Mato Level Editor");
    window.setFramerateLimit(60);

    editor::Editor editor(std::move(levelData));
    editor::EditorRenderer renderer(window);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return 0;
            }

            if (event.type == sf::Event::KeyPressed)
            {
                editor.handleKey(event.key.code);
            }
        }

        if (editor.shouldExit())
        {
            break;
        }

        window.clear(sf::Color::Black);
        renderer.draw(editor);
        window.display();
    }

    if (editor.shouldSave())
    {
        try
        {
            editor::saveLevel(filepath, editor.level());
            std::cout << "Saved to " << filepath << "\n";
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error saving: " << e.what() << "\n";
            return 1;
        }
    }

    return 0;
}
