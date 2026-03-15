#pragma once

#include "editor/LevelFileIO.hpp"
#include <SFML/Window/Keyboard.hpp>

namespace editor
{

    enum class Mode
    {
        Normal,
        EditingField,
        SavePrompt
    };

    enum class Field
    {
        Foods,
        Growth,
        Start,
        Direction
    };

    class Editor
    {
    public:
        explicit Editor(LevelData level);

        void handleKey(sf::Keyboard::Key key);

        const LevelData &level() const { return level_; }
        int cursorX() const { return cursorX_; }
        int cursorY() const { return cursorY_; }
        Mode mode() const { return mode_; }
        Field activeField() const { return activeField_; }
        const std::string &inputBuffer() const { return inputBuffer_; }
        bool shouldExit() const { return shouldExit_; }
        bool shouldSave() const { return shouldSave_; }

    private:
        void handleNormalKey(sf::Keyboard::Key key);
        void handleEditingKey(sf::Keyboard::Key key);
        void handleSavePromptKey(sf::Keyboard::Key key);

        void moveCursor(int dx, int dy);
        void toggleCell();
        void startFieldEdit(Field f);
        void confirmFieldEdit();
        void cancelFieldEdit();

        LevelData level_;
        int cursorX_ = 0;
        int cursorY_ = 0;
        Mode mode_ = Mode::Normal;
        Field activeField_ = Field::Foods;
        std::string inputBuffer_;
        bool shouldExit_ = false;
        bool shouldSave_ = false;
    };

}
