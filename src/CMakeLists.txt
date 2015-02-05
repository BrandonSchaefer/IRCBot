find_package (PkgConfig)

set (IRC_BOT_REQUIRED
     libcurl
    )

pkg_check_modules(IRC_BOT REQUIRED ${IRC_BOT_REQUIRED})

set (IRC_BOT_LOADER_SRC
     ChannelIO.cpp
     CommandHandler.cpp
     IRCBot.cpp
     IRCBotOffline.cpp
     IRCBotController.cpp
     LastFMCurrentSong.cpp
     LoadedChannelController.cpp
     LoadBasicCommands.cpp
     SymWeaponInfo.cpp
     Utils.cpp
     main.cpp
    )

set (IRC_BOT_LOADER_HDR
     ChannelIO.h
     CommandBreed.h
     CommandHandler.h
     IRCBot.h
     IRCBotOffline.h
     IRCBotController.h
     LastFMCurrentSong.h
     LoadBasicCommands.h
     LoadedChannelController.h
     LoadedChannelData.h
     SymWeaponInfo.h
     Utils.h
    )

include_directories (${IRC_BOT_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

add_executable (irc_bot     ${IRC_BOT_LOADER_SRC} ${IRC_BOT_LOADER_HDR})
add_library    (irc_bot_lib ${IRC_BOT_LOADER_SRC} ${IRC_BOT_LOADER_HDR})

target_link_libraries (irc_bot -lcurl ${IRC_BOT_LIBRARIES} ${IRC_BOT_LDFLAGS})
