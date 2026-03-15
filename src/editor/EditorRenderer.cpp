#include "editor/EditorRenderer.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>

namespace editor
{

    static constexpr float kCellSize = 24.f;
    static constexpr float kCellFill = 22.f;
    static constexpr float kGridHeight = kLevelHeight * kCellSize;

    static const sf::Color kWallColor{127, 140, 141};
    static const sf::Color kEmptyColor{30, 30, 30};
    static const sf::Color kBorderColor{50, 50, 50};
    static const sf::Color kCursorColor{255, 255, 0};
    static const sf::Color kStartMarkerColor{205, 127, 50};
    static const sf::Color kMetaBgColor{20, 20, 20};
    static const sf::Color kTextColor{200, 200, 200};
    static const sf::Color kHighlightColor{100, 200, 255};
    static const sf::Color kEditingColor{255, 200, 50};
    static const sf::Color kOverlayBg{0, 0, 0, 180};

    EditorRenderer::EditorRenderer(sf::RenderWindow &window)
        : window_(window)
    {
        font_.loadFromFile("assets/fonts/Comfortaa-Regular.otf");
    }

    void EditorRenderer::draw(const Editor &editor)
    {
        drawGrid(editor);
        drawStartMarker(editor);
        drawCursor(editor);
        drawMetadata(editor);

        if (editor.mode() == Mode::SavePrompt)
        {
            drawSavePrompt();
        }
    }

    void EditorRenderer::drawGrid(const Editor &editor)
    {
        sf::RectangleShape cell({kCellFill, kCellFill});

        const auto &level = editor.level();

        for (int y = 0; y < kLevelHeight; y++)
        {
            for (int x = 0; x < kLevelWidth; x++)
            {
                float px = x * kCellSize + 1.f;
                float py = y * kCellSize + 1.f;
                cell.setPosition(px, py);

                if (level.rows[y][x] == '#')
                {
                    cell.setFillColor(kWallColor);
                }
                else
                {
                    cell.setFillColor(kEmptyColor);
                }

                window_.draw(cell);
            }
        }
    }

    void EditorRenderer::drawCursor(const Editor &editor)
    {
        sf::RectangleShape outline({kCellSize, kCellSize});
        outline.setPosition(editor.cursorX() * kCellSize, editor.cursorY() * kCellSize);
        outline.setFillColor(sf::Color::Transparent);
        outline.setOutlineColor(kCursorColor);
        outline.setOutlineThickness(-2.f);
        window_.draw(outline);
    }

    void EditorRenderer::drawStartMarker(const Editor &editor)
    {
        const auto &level = editor.level();
        sf::CircleShape marker(5.f);
        marker.setFillColor(kStartMarkerColor);
        marker.setPosition(
            level.startX * kCellSize + kCellSize / 2.f - 5.f,
            level.startY * kCellSize + kCellSize / 2.f - 5.f);
        window_.draw(marker);
    }

    void EditorRenderer::drawMetadata(const Editor &editor)
    {
        // Background for metadata area
        sf::RectangleShape bg({720.f, 120.f});
        bg.setPosition(0.f, kGridHeight);
        bg.setFillColor(kMetaBgColor);
        window_.draw(bg);

        const auto &level = editor.level();
        bool editing = editor.mode() == Mode::EditingField;

        auto drawField = [&](float x, float y, const std::string &label, char shortcut,
                             const std::string &value, Field field)
        {
            sf::Text text;
            text.setFont(font_);
            text.setCharacterSize(16);

            // Draw bracket letter highlighted
            std::string prefix = "[";
            prefix += shortcut;
            prefix += "]";
            std::string rest = label.substr(1); // Skip the first char (same as shortcut)

            std::string display;
            if (editing && editor.activeField() == field)
            {
                display = prefix + rest + ": " + editor.inputBuffer() + "_";
                text.setFillColor(kEditingColor);
            }
            else
            {
                display = prefix + rest + ": " + value;
                text.setFillColor(kTextColor);
            }

            text.setString(display);
            text.setPosition(x, y);
            window_.draw(text);
        };

        float y1 = kGridHeight + 20.f;
        float y2 = kGridHeight + 55.f;

        drawField(20.f, y1, "Foods to complete", 'F',
                  std::to_string(level.foodsToComplete), Field::Foods);

        drawField(370.f, y1, "Growth per food", 'G',
                  std::to_string(level.growthPerFood), Field::Growth);

        drawField(20.f, y2, "Start", 'S',
                  std::to_string(level.startX) + ", " + std::to_string(level.startY), Field::Start);

        std::string dirDisplay = level.startDirection;
        if (editing && editor.activeField() == Field::Direction)
        {
            dirDisplay = "(press arrow key)";
        }
        drawField(370.f, y2, "Direction", 'D', dirDisplay, Field::Direction);

        // Help text at bottom
        sf::Text help;
        help.setFont(font_);
        help.setCharacterSize(12);
        help.setFillColor(sf::Color(100, 100, 100));
        help.setString("Arrows: move  Space: toggle wall  Escape: save/exit");
        help.setPosition(20.f, kGridHeight + 92.f);
        window_.draw(help);
    }

    void EditorRenderer::drawSavePrompt()
    {
        // Semi-transparent overlay
        sf::RectangleShape overlay({720.f, 600.f});
        overlay.setFillColor(kOverlayBg);
        window_.draw(overlay);

        sf::Text prompt;
        prompt.setFont(font_);
        prompt.setCharacterSize(28);
        prompt.setFillColor(sf::Color::White);
        prompt.setString("Save? (Y/N)");

        auto bounds = prompt.getLocalBounds();
        prompt.setPosition(360.f - bounds.width / 2.f, 270.f - bounds.height / 2.f);
        window_.draw(prompt);
    }

}
