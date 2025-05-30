#include "grouphud.h"
#include "data.h"
#include "filetools.h"
#include <memory>
#include <iostream> // For std::cerr

namespace DP {

GroupHud::GroupHud(){
    // Default constructor: unique_ptrs (seasonName, roundName, monthName) are initially nullptr.
    // The setFont method is responsible for their actual initialization with a valid font.
}

void GroupHud::setFont(sf::Font *gameFont) {
    if (!gameFont) {
        std::cerr << "Error: GroupHud::setFont called with null gameFont. Text objects will not be created." << std::endl;
        // Explicitly set to nullptr to ensure they are not used if font is bad, 
        // though default unique_ptr construction is already nullptr.
        seasonName = nullptr;
        roundName = nullptr;
        monthName = nullptr;
        return;
    }

    // Initialize unique_ptrs if they haven_t been already, or set font if they exist.
    if (!seasonName) seasonName = std::make_unique<sf::Text>(*gameFont);
    else seasonName->setFont(*gameFont); 

    if (!roundName) roundName = std::make_unique<sf::Text>(*gameFont);
    else roundName->setFont(*gameFont);

    if (!monthName) monthName = std::make_unique<sf::Text>(*gameFont);
    else monthName->setFont(*gameFont);

    // Check if text objects were successfully created before using them
    if (seasonName) {
        seasonName->setFillColor(sf::Color(255,255,255));
        seasonName->setCharacterSize(30);
    }
    if (roundName) {
        roundName->setCharacterSize(30);
        roundName->setFillColor(sf::Color(255,255,250));
    }
    if (monthName) {
        monthName->setCharacterSize(30);
        setMonthName(1); // Ensure this is called after monthName is valid and font is set
    }
}

void GroupHud::setSeason(int seasonNumber){
    if (seasonName) {
        seasonName->setString("Season: " + DP::seasonsNames[seasonNumber]);
        // Position the entire GroupHud object, not individual text elements
        setPosition(sf::Vector2f(440, 12));
    }
}

void GroupHud::setDeerModeActive(){
    if (seasonName) {
        seasonName->setString("Deer Mode");
        // Position the entire GroupHud object, not individual text elements  
        setPosition(sf::Vector2f(455, 12));
    }
}

void GroupHud::setRoundName(int roundNumber){
    if (roundName) {
        roundName->setString("Round: " + std::to_string(roundNumber));
        // Round text positioned at bottom left, as in original
        roundName->setPosition(sf::Vector2f(30, 700));
    }
}

void GroupHud::setDeerModeCounter(int deerModeCounter)
{
    if (roundName) {
        roundName->setString(std::to_string(deerModeCounter));
        // Deer mode counter positioned at bottom left
        roundName->setPosition(sf::Vector2f(50, 700));
    }
}

void GroupHud::setMonthName(int monthNumber){
    if (monthName) {
        monthName->setString("Month: " + std::to_string(monthNumber));
        // Month positioning - though it won't be drawn in the original behavior
    }
}

void GroupHud::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    states.transform *= getTransform();
    
    // Draw season name (positioned via GroupHud transform)
    if (seasonName) target.draw(*seasonName, states);
    
    // Draw round name (positioned absolutely at bottom left)
    if (roundName) target.draw(*roundName, states);
    
    // Month is NOT drawn in original - commented out to match original behavior
    // if (monthName) target.draw(*monthName, states);
}
}

