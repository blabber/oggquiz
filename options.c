/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

#include <assert.h>
#include <err.h>
#include <getopt.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

#include "options.h"

enum {
        TIME = 60,
        CHOICES = 4,
        PLAYERS = 4
};

static const char *OGG123 = "/usr/local/bin/ogg123";
static const char *OGG123_OPTIONS = "";

void            init_options(struct opts_options *opts);
static void     print_usage(void);

void
init_options(struct opts_options *opts)
{
        assert(opts != NULL);

        opts->time = TIME;
        opts->choices = CHOICES;
        opts->players = PLAYERS;
        opts->ogg123 = OGG123;
        opts->ogg123_options = OGG123_OPTIONS;
}

void
opts_parse_options(struct opts_options *opts, int argc, char **argv)
{
        int             ch;

        assert(opts != NULL);
        assert(argc >= 0);
        assert(argv != NULL);

        struct option   longopts[] = {
                {"time", required_argument, NULL, 't'},
                {"choices", required_argument, NULL, 'c'},
                {"players", required_argument, NULL, 'p'},
                {"ogg123", required_argument, NULL, 'O'},
                {"ogg123-options", required_argument, NULL, 'o'},
                {"help", no_argument, NULL, 'h'}
        };

        init_options(opts);

        while ((ch = getopt_long(argc, argv, "t:c:p:o:e:h", longopts, NULL)) != -1)
                switch (ch) {
                case 't':
                        opts->time = (int)strtol(optarg, (char **)NULL, 10);
                        break;
                case 'c':
                        opts->choices = (int)strtol(optarg, (char **)NULL, 10);
                        if (opts->choices < 1 || opts->choices > CHOICES)
                                errx(EX_USAGE, "choices must not exceed %d", CHOICES);
                        break;
                case 'p':
                        opts->players = (int)strtol(optarg, (char **)NULL, 10);
                        if (opts->players < 1 || opts->players > PLAYERS)
                                errx(EX_USAGE, "players must not exceed %d", PLAYERS);
                        break;
                case 'O':
                        opts->ogg123 = optarg;
                        break;
                case 'o':
                        opts->ogg123_options = optarg;
                        break;
                default:
                        print_usage();
                        exit(1);
                case 'h':
                        print_usage();
                        exit(0);
                }
}

static void
print_usage()
{
        printf("oggquiz [-t | --time seconds] [-c | --choices choices] [-p | --players players]\n");
        printf("        [-o | --ogg123-options options] [-O | --ogg123 command]\n\n");
        printf("oggquiz {-h | --help}\n");
}
