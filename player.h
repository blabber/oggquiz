/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

#include "oggfile.h"

#ifndef _player_h_
#define _player_h_

void            player_play(oggfile_t * oggfile);
void            player_stop();

#endif
