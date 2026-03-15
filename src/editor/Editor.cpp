#include "editor/Editor.hpp"

#include <algorithm>

namespace editor
{

    Editor::Editor(LevelData level)
        : level_(std::move(level))
    {
    }

    void Editor::handleKey(sf::Keyboard::Key key)
    {
        switch (mode_)
        {
        case Mode::Normal:
            handleNormalKey(key);
            break;
        case Mode::EditingField:
            handleEditingKey(key);
            break;
        case Mode::SavePrompt:
            handleSavePromptKey(key);
            break;
        }
    }

    void Editor::handleNormalKey(sf::Keyboard::Key key)
    {
        switch (key)
        {
        case sf::Keyboard::Up:
            moveCursor(0, -1);
            break;
        case sf::Keyboard::Down:
            moveCursor(0, 1);
            break;
        case sf::Keyboard::Left:
            moveCursor(-1, 0);
            break;
        case sf::Keyboard::Right:
            moveCursor(1, 0);
            break;
        case sf::Keyboard::Space:
            toggleCell();
            break;
        case sf::Keyboard::F:
            startFieldEdit(Field::Foods);
            break;
        case sf::Keyboard::G:
            startFieldEdit(Field::Growth);
            break;
        case sf::Keyboard::S:
            startFieldEdit(Field::Start);
            break;
        case sf::Keyboard::D:
            startFieldEdit(Field::Direction);
            break;
        case sf::Keyboard::Escape:
            mode_ = Mode::SavePrompt;
            break;
        default:
            break;
        }
    }

    void Editor::handleEditingKey(sf::Keyboard::Key key)
    {
        // Direction field: auto-confirm on arrow key
        if (activeField_ == Field::Direction)
        {
            switch (key)
            {
            case sf::Keyboard::Up:
                level_.startDirection = "up";
                mode_ = Mode::Normal;
                return;
            case sf::Keyboard::Down:
                level_.startDirection = "down";
                mode_ = Mode::Normal;
                return;
            case sf::Keyboard::Left:
                level_.startDirection = "left";
                mode_ = Mode::Normal;
                return;
            case sf::Keyboard::Right:
                level_.startDirection = "right";
                mode_ = Mode::Normal;
                return;
            case sf::Keyboard::Escape:
                cancelFieldEdit();
                return;
            default:
                return;
            }
        }

        // Digit keys for numeric/start fields
        if (key >= sf::Keyboard::Num0 && key <= sf::Keyboard::Num9)
        {
            char digit = '0' + (key - sf::Keyboard::Num0);
            inputBuffer_ += digit;
            return;
        }
        if (key >= sf::Keyboard::Numpad0 && key <= sf::Keyboard::Numpad9)
        {
            char digit = '0' + (key - sf::Keyboard::Numpad0);
            inputBuffer_ += digit;
            return;
        }

        // Comma for Start field
        if (key == sf::Keyboard::Comma && activeField_ == Field::Start)
        {
            inputBuffer_ += ',';
            return;
        }

        switch (key)
        {
        case sf::Keyboard::BackSpace:
            if (!inputBuffer_.empty())
            {
                inputBuffer_.pop_back();
            }
            break;
        case sf::Keyboard::Enter:
            confirmFieldEdit();
            break;
        case sf::Keyboard::Escape:
            cancelFieldEdit();
            break;
        default:
            break;
        }
    }

    void Editor::handleSavePromptKey(sf::Keyboard::Key key)
    {
        if (key == sf::Keyboard::Y)
        {
            shouldSave_ = true;
            shouldExit_ = true;
        }
        else if (key == sf::Keyboard::N)
        {
            shouldSave_ = false;
            shouldExit_ = true;
        }
        else if (key == sf::Keyboard::Escape)
        {
            mode_ = Mode::Normal;
        }
    }

    void Editor::moveCursor(int dx, int dy)
    {
        cursorX_ = std::clamp(cursorX_ + dx, 0, kLevelWidth - 1);
        cursorY_ = std::clamp(cursorY_ + dy, 0, kLevelHeight - 1);
    }

    void Editor::toggleCell()
    {
        char &cell = level_.rows[cursorY_][cursorX_];
        if (cursorX_ == level_.startX && cursorY_ == level_.startY)
        {
            // Don't allow toggling the start position to wall
            return;
        }
        cell = (cell == '#') ? '.' : '#';
    }

    void Editor::startFieldEdit(Field f)
    {
        activeField_ = f;
        mode_ = Mode::EditingField;

        if (f == Field::Direction)
        {
            inputBuffer_.clear();
            return;
        }

        // Pre-fill input buffer with current value
        switch (f)
        {
        case Field::Foods:
            inputBuffer_ = std::to_string(level_.foodsToComplete);
            break;
        case Field::Growth:
            inputBuffer_ = std::to_string(level_.growthPerFood);
            break;
        case Field::Start:
            inputBuffer_ = std::to_string(level_.startX) + "," + std::to_string(level_.startY);
            break;
        default:
            break;
        }
    }

    void Editor::confirmFieldEdit()
    {
        if (inputBuffer_.empty())
        {
            cancelFieldEdit();
            return;
        }

        switch (activeField_)
        {
        case Field::Foods:
        {
            int val = std::stoi(inputBuffer_);
            if (val > 0)
                level_.foodsToComplete = val;
            break;
        }
        case Field::Growth:
        {
            int val = std::stoi(inputBuffer_);
            if (val > 0)
                level_.growthPerFood = val;
            break;
        }
        case Field::Start:
        {
            auto commaPos = inputBuffer_.find(',');
            if (commaPos != std::string::npos)
            {
                int x = std::stoi(inputBuffer_.substr(0, commaPos));
                int y = std::stoi(inputBuffer_.substr(commaPos + 1));
                if (x >= 0 && x < kLevelWidth && y >= 0 && y < kLevelHeight &&
                    level_.rows[y][x] == '.')
                {
                    level_.startX = x;
                    level_.startY = y;
                }
            }
            break;
        }
        default:
            break;
        }

        mode_ = Mode::Normal;
        inputBuffer_.clear();
    }

    void Editor::cancelFieldEdit()
    {
        mode_ = Mode::Normal;
        inputBuffer_.clear();
    }

}
