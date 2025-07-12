#pragma once
#include <fstream>
#include <functional>
#include <memory>
#include <optional>

#include "exceptions.h"

namespace DeerPortal {

class ErrorHandler {
public:
  using ErrorCallback = std::function<void(const GameException&)>;

  // Global error handler instance
  static ErrorHandler& getInstance();

  // Set custom error callback (for UI notifications)
  void setErrorCallback(ErrorCallback&& callback);

  // Handle exceptions with recovery options
  bool handleException(const GameException& e);

  // Try to load asset with fallback
  template <typename T>
  std::optional<T> tryLoadWithFallback(std::function<T()> loader,
                                       std::function<T()> fallbackLoader) {
    try {
      return loader();
    } catch (const GameException& e) {
      logError(e);
      try {
        return fallbackLoader();
      } catch (const GameException& fallbackError) {
        logError(fallbackError);
        return std::nullopt;
      }
    }
  }

  // Log error to file
  void logError(const GameException& e);

  // Initialize error logging
  void initializeLogging(const std::string& logPath = "deerportal-errors.log");

  // Get error statistics
  struct ErrorStats {
    int assetLoadErrors = 0;
    int systemResourceErrors = 0;
    int gameStateErrors = 0;
    int totalErrors = 0;
  };

  const ErrorStats& getStats() const { return stats; }

private:
  ErrorHandler() = default;

  ErrorCallback errorCallback;
  std::unique_ptr<std::ofstream> errorLog;
  ErrorStats stats;
  bool loggingInitialized = false;
};

} // namespace DeerPortal