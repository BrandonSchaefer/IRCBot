IRCBot made mainly for use on twitch channels

Dependices for debian/ubuntu:

    libcurl4-gnutls-dev

To compile on debian/ubuntu:

    mkdir build
    cd build
    cmake ..
    make -j4

Windows:

    TODO

Commands, all start with ! (ex. !fib 3):

    stats   <weapon>
    compare <weapon> <weapon>
    custom  <perm> <match> <return_str>
    remove  <match>
    fib     <n>
    google  <search_str>
    song

Custom commands are saved in IRCBots/saves, which is updated per channel that is joined
