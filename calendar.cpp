#include "calendar.h"

std::string getHoliday(int month, int day)
{
    for (int i=0;i<59;i++)
    {
        if (month==std::stoi(PAGAN_HOLIDAYS[i][0]))
        {
            if (day==std::stoi(PAGAN_HOLIDAYS[i][1]))
            {
                return PAGAN_HOLIDAYS[i][2];
            }
        }
    }
    return "";
}
