#ifndef NETWORKCONSTANTS_H
#define NETWORKCONSTANTS_H

#include <QString>

inline QString userName;
inline QString clientAddress;
inline QString clientPort;

inline QString ServerAddress = "45.40.228.194";
inline constexpr int ServerPort = 9999;
inline QString mianPetId;
inline QString randomKey;

inline constexpr int MianPetVersion = 20200727;


/* method begin */

inline constexpr int GET = 0;
inline constexpr int HERATBEAT = 1;
inline constexpr int LOGOUT = 2;

/* method end */


/* hint begin */

inline constexpr int FOOD_SHOP_INFO = 0;
inline constexpr int CLEAN_SHOP_INFO = 1;
inline constexpr int PETPROFILE = 2;
inline constexpr int CORE_KEY_FOR_PASSWORD_TRANSPORTATION = 3;
inline constexpr int LOGIN = 4;
inline constexpr int BUY = 5;
inline constexpr int USE = 6;
inline constexpr int WORKBEGIN = 7;

/* hint end */

#endif // NETWORKCONSTANTS_H
