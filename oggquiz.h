/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

/**
 * Required includes:
 * common.h
 */

enum {
        PLAYERS = 4
};

/*
 * The global options structure is considered read only! The only module
 * allowed to modify it is oggquiz.c.
 */
struct options {
        int             time;
        int             choices;
        int             players;
        char            ogg123[OPTIONLEN];
};
