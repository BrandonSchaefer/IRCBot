IRCBot made mainly for use on twitch channels

Dependices for debian/ubuntu:

    libcurl4-gnutls-dev
    libjsoncpp-dev

Optional dependices for testing on debian/ubuntu:

    google-mock

To compile on debian/ubuntu:

    mkdir build
    cd build
    cmake ..
    make -j4

To run tests:

    make check

Windows:

    TODO

Commands, all start with ! (ex. !fib 3):

    stats      <weapon>
    compare    <weapon> <weapon>
    custom     <perm> <match> <return_str>
    remove     <match>
    fib        <n>
    google     <search_str>
    set lastfm <username>
    song
    bot join
    bot leave

Custom commands are saved in IRCBots/saves, which is updated per channel that is joined
