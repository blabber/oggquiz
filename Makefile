PROG=		oggquiz
SRCS=		${PROG}.c oggfile.c player.c ui.c options.c
NO_MAN=         yes

CSTD?=		c89
WARNS?=		6
WFORMAT?=	1
# NO_WERROR is needed as libvorbis.h defines some static variables not used
# in this code. This leads to warnings and breaks the build if NO_WERROR is
# unset.  But I prefer NO_WERROR to hiding the warning.
NO_WERROR=	yes

CFLAGS+=	-I/usr/local/include
LDFLAGS+=	-L/usr/local/lib 
LDADD+=		-lvorbisfile
LDADD+=		-liconv
LDADD+=		-lcurses

CLEANFILES=	*.[Bb][Aa][Kk] *.core
CTAGS=		ctags

.include <bsd.prog.mk>
