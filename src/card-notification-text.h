#ifndef CARD_NOTIFICATION_TEXT_H
#define CARD_NOTIFICATION_TEXT_H

#include <string>
#include <vector>

/*!
 * \brief CardNotificationText handles text generation and formatting for card notifications
 *
 * This class is responsible for:
 * - Generating notification messages based on card types
 * - Formatting text with player information
 * - Managing text localization (future expansion)
 * - Text content logic and rules
 */
class CardNotificationText {
public:
  CardNotificationText();
  ~CardNotificationText();

  // Main text generation methods
  std::string generateNotificationText(const std::string& cardType, int playerNumber,
                                       int targetPlayer);
  std::string generateCardEffectText(const std::string& cardType, int playerNumber,
                                     int targetPlayer);

  // Specialized text generation for different card types
  std::string generateDiamondText(int playerNumber, int targetPlayer);
  std::string generateStopText(int playerNumber, int targetPlayer);
  std::string generateRemoveCardText(int playerNumber, int targetPlayer);
  std::string generateDoubleDiamondText(int playerNumber, int targetPlayer);

  // Player name formatting
  std::string getPlayerName(int playerNumber);
  std::string getPlayerDisplayName(int playerNumber);

  // Text formatting utilities
  std::string formatMultiLineText(const std::vector<std::string>& lines);
  std::string formatPlayerAction(const std::string& action, int playerNumber,
                                 int targetPlayer = -1);

  // Text validation and processing
  bool isValidCardType(const std::string& cardType);
  std::vector<std::string> splitTextIntoLines(const std::string& text, int maxLineLength = 50);

  // Configuration
  void setPlayerNames(const std::vector<std::string>& names);
  void setElementNames(const std::vector<std::string>& elements);

private:
  // Text templates and constants
  static const std::vector<std::string> DEFAULT_PLAYER_NAMES;
  static const std::vector<std::string> DEFAULT_ELEMENT_NAMES;

  // Current configuration
  std::vector<std::string> playerNames;
  std::vector<std::string> elementNames;

  // Helper methods
  std::string buildBasicNotification(const std::string& cardType, int playerNumber);
  std::string buildTargetedNotification(const std::string& cardType, int playerNumber,
                                        int targetPlayer);
  std::string getCardDisplayName(const std::string& cardType);

  // Text processing helpers
  std::string capitalizeFirst(const std::string& text);
  std::string formatPlayerReference(int playerNumber);
  std::string formatTargetReference(int targetPlayer);
};

#endif // CARD_NOTIFICATION_TEXT_H