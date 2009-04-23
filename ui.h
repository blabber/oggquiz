/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

#include "common.h"
#include "oggfile.h"

#ifndef _ui_h_
#define _ui_h_

typedef struct {
        oggfile_t      *oggfiles;
        oggfile_t      *guess;
        oggfile_t      *correct;
}               ui_model_t;

void            ui_display_quiz(ui_model_t * model);
void            ui_display_result(ui_model_t * model);
char            ui_get_answer();
void            ui_pause();
void            ui_setup();
void            ui_teardown();

#endif
