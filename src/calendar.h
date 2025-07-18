/*
 * Pagan
 *
 * $OpenBSD: calendar.pagan,v 1.2 2002/03/20 22:53:40 mickey Exp $
 */

#ifndef _calendar_pagan_
#define _calendar_pagan_
#include <array>
#include <iostream>
#include <string>

static std::array<std::array<std::string, 3>, 59> PAGAN_HOLIDAYS = {
    {{{"01", "02", "Frigg's Distaff"}},
     {{"01", "06", "Celtic day of the Three-Fold Goddess"}},
     {{"01", "10", "Geraints Day"}},
     {{"01", "13", "Midvintersblot"}},
     {{"01", "20", "Midvintersblot"}},
     {{"01", "22", "Festival of the Muses"}},
     {{"01", "25", "Old Disting Feast"}},
     {{"01", "30", "Upelly - old fire festival"}},
     {{"01", "31", "Honoring the Valkyries"}},
     {{"02", "02", "1st Cross-Quarter Day"}},
     {{"02", "02", "Imbolg / Imbolc (other Celtic names) / Brighid / Oimelc"}},
     {{"02", "11", "The Day the Birds Begin to Sing"}},
     {{"02", "14", "Festival of Vali"}},
     {{"03", "02", "Day of the Crows"}},
     {{"03", "13", "Burgsonndeg - festival of rebirth of the sun and the approach of spring"}},
     {{"03", "17", "Celebration of Trefuilnid Treochar (St.Patrick's day)"}},
     {{"03", "18", "Sheela's Day"}},
     {{"03", "19", "1st Quarter Day - Spring (Vernal) Equinox"}},
     {{"03", "20", "Festival of Iduna"}},
     {{"03", "21", "Ostara / Eostre (Saxon goddess of Spring)"}},
     {{"04", "30",
       "May Eve / Walpurgisnacht (witches' Sabbath) / Walpurgis Night (after St. Walpurga)"}},
     {{"05", "01", "May Day / Beltane / Bealtaine - Celtic bonfire festival"}},
     {{"05", "04", "2nd Cross-Quarter Day"}},
     {{"05", "12", "The Cat Parade"}},
     {{"05", "14", "Midnight Sun Festival"}},
     {{"05", "18", "Festival of the Horned God Cennunos"}},
     {{"05", "20", "Festival of Mjollnir, Thor's Hammer"}},
     {{"05", "24", "Mother's Day, in celebration of the Triple Goddes"}},
     {{"06", "20", "2nd Quarter Day - Summer Solstice"}},
     {{"06", "21", "Litha (Norse/Anglo-Saxon for longest day)"}},
     {{"06", "23", "St. John's Eve - European Midsummer celebration"}},
     {{"08", "01", "Lugnasad / Lughnasada / Lunasa - Gaelic summer games of Lug (sun-god)"}},
     {{"08", "02", "Lady Godiva Day"}},
     {{"08", "04", "Loch mo Naire"}},
     {{"08", "05", "3rd Cross-Quarter Day"}},
     {{"08", "10", "Puck Faire"}},
     {{"08", "14", "Day of the Burryma"}},
     {{"08", "17", "Odin's Ordeal"}},
     {{"08", "23", "Feast of Ilmatar"}},
     {{"08", "28", "Frey Faxi"}},
     {{"08", "29", "Festival of Urda"}},
     {{"09", "21", "3rd Quarter Day - Fall (Autumnal) Equinox"}},
     {{"10", "08", "Day of Remembrance for Erik the Red"}},
     {{"10", "09", "Leif Erikson Day"}},
     {{"10", "11", "Old Lady of the Trees"}},
     {{"10", "14", "Winter Nights - festival celebrating the harvest"}},
     {{"10", "18", "Great Horn Faire"}},
     {{"10", "24", "Feast of the Spirits of the Air"}},
     {{"10", "27", "Allan Apple Day"}},
     {{"10", "31", "Hallowmas / Allhallowmas / Allhallows"}},
     {{"11", "01", "Samhain - Celtic feast of departing Sun & new year"}},
     {{"11", "05", "4th Cross-Quarter Day"}},
     {{"12", "06", "Festival of Nicolas, an aspect of Odin, leader of the hunt"}},
     {{"12", "09", "Yule Cleansing - The Danish fetch water for brewing the Yule-Ale"}},
     {{"12", "13", "Wodan's Jag - the Wild Hunt of Odin and his Army"}},
     {{"12", "18", "Festival of Epona"}},
     {{"12", "20", "4th Quarter Day - Winter Solstice"}},
     {{"12", "21", "Festival of Beiwe / Nertha / Alban Athuan"}},
     {{"12", "21", "Yule (Norse for wheel) - Germanic 12-day feast"}}}};

std::string getHoliday(int month, int day);

#endif /* !_calendar_pagan_ */
