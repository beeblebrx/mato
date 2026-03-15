#include "editor/LevelFileIO.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <filesystem>

namespace editor
{

    static LevelData makeDefault()
    {
        LevelData data;
        for (auto &row : data.rows)
        {
            row = std::string(kLevelWidth, '.');
        }
        return data;
    }

    LevelData loadLevel(const std::string &filepath)
    {
        if (!std::filesystem::exists(filepath))
        {
            return makeDefault();
        }

        std::ifstream file(filepath);
        if (!file.is_open())
        {
            throw std::runtime_error("Cannot open file: " + filepath);
        }

        // Read the entire file. Extra parens prevent the most vexing parse.
        std::string content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());

        auto sepPos = content.find("---");
        if (sepPos == std::string::npos)
        {
            throw std::runtime_error("Missing '---' separator in " + filepath);
        }

        std::string header = content.substr(0, sepPos);
        std::string grid = content.substr(sepPos + 3);

        // Parse metadata
        LevelData data;
        std::istringstream headerStream(header);
        std::string line;
        while (std::getline(headerStream, line))
        {
            if (line.empty())
                continue;
            auto colonPos = line.find(':');
            if (colonPos == std::string::npos)
                continue;

            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 1);

            // Trim whitespace
            auto trim = [](std::string &s)
            {
                size_t start = s.find_first_not_of(" \t\r\n");
                size_t end = s.find_last_not_of(" \t\r\n");
                s = (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
            };
            trim(key);
            trim(value);

            if (key == "foodsToComplete")
            {
                data.foodsToComplete = std::stoi(value);
            }
            else if (key == "growthPerFood")
            {
                data.growthPerFood = std::stoi(value);
            }
            else if (key == "startDirection")
            {
                data.startDirection = value;
            }
            else if (key == "start")
            {
                auto commaPos = value.find(',');
                if (commaPos == std::string::npos)
                {
                    throw std::runtime_error("Invalid start format in " + filepath);
                }
                std::string xStr = value.substr(0, commaPos);
                std::string yStr = value.substr(commaPos + 1);
                auto trim2 = [](std::string &s)
                {
                    size_t start = s.find_first_not_of(" \t");
                    size_t end = s.find_last_not_of(" \t");
                    s = (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
                };
                trim2(xStr);
                trim2(yStr);
                data.startX = std::stoi(xStr);
                data.startY = std::stoi(yStr);
            }
        }

        // Parse grid rows
        std::istringstream gridStream(grid);
        int rowIdx = 0;
        while (std::getline(gridStream, line))
        {
            if (line.empty())
                continue;
            if (rowIdx >= kLevelHeight)
            {
                throw std::runtime_error("Too many grid rows in " + filepath);
            }
            if (static_cast<int>(line.size()) != kLevelWidth)
            {
                throw std::runtime_error("Row " + std::to_string(rowIdx) + " has " +
                                         std::to_string(line.size()) + " chars, expected " +
                                         std::to_string(kLevelWidth) + " in " + filepath);
            }
            data.rows[rowIdx] = line;
            rowIdx++;
        }

        if (rowIdx != kLevelHeight)
        {
            throw std::runtime_error("Expected " + std::to_string(kLevelHeight) +
                                     " grid rows, got " + std::to_string(rowIdx) + " in " + filepath);
        }

        return data;
    }

    void saveLevel(const std::string &filepath, const LevelData &data)
    {
        // Create parent directories if needed
        auto parent = std::filesystem::path(filepath).parent_path();
        if (!parent.empty())
        {
            std::filesystem::create_directories(parent);
        }

        std::ofstream file(filepath);
        if (!file.is_open())
        {
            throw std::runtime_error("Cannot write to file: " + filepath);
        }

        file << "foodsToComplete: " << data.foodsToComplete << "\n";
        file << "growthPerFood: " << data.growthPerFood << "\n";
        file << "start: " << data.startX << ", " << data.startY << "\n";
        file << "startDirection: " << data.startDirection << "\n";
        file << "---\n";

        for (const auto &row : data.rows)
        {
            file << row << "\n";
        }
    }

}
