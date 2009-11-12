/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

struct opts_options {
        int             time;
        int             choices;
        int             players;
        char const     *ogg123;
};

void            opts_parse_options(struct opts_options *_opts, int _argc, char **_argv);
