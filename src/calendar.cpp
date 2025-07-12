#include "calendar.h"

#define ARRAY_LEN(x) (sizeof(x) / sizeof(x[0]))

std::string getHoliday(int month, int day) {
  for (size_t i = 0; i < ARRAY_LEN(PAGAN_HOLIDAYS); i++) {
    try {
      if (month == std::stoi(PAGAN_HOLIDAYS[i][0])) {
        if (day == std::stoi(PAGAN_HOLIDAYS[i][1])) {
          return PAGAN_HOLIDAYS[i][2];
        }
      }
    } catch (...) {
      std::cerr << "Failed to convert " << PAGAN_HOLIDAYS[i][0] << " and " << PAGAN_HOLIDAYS[i][1]
                << " into ints\n";
      continue;
    }
  }
  return "";
}
