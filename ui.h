/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

struct ui_oggfile {
        char           *artist;
        char           *album;
        char           *title;
};

struct ui_model {
        struct ui_oggfile *oggfiles;
        struct ui_oggfile *guess;
        struct ui_oggfile *correct;
        int             turn;
        int             players;
        int            *scores;
        int             current_player;
        int             choices;
};

struct ui_context;

void            ui_context_close(struct ui_context *_ctx);
struct ui_context *ui_context_open(void);
void            ui_display_quiz(struct ui_context *_ctx, struct ui_model *_model);
void            ui_display_result(struct ui_context *_ctx, struct ui_model *_model);
char            ui_get_key(void);
