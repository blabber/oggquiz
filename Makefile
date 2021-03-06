PROG=		oggquiz
SRCS=		${PROG}.c oggfile.c player.c ui.c options.c
MAN6=		${PROG}.6

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
LDADD+=		${libiconv}
LDADD+=		-lcurses

CLEANFILES=	*.[Bb][Aa][Kk] *.core
CTAGS=		ctags

COL=		/usr/bin/col

all:		README

.include <bsd.prog.mk>

README:	${MAN6}
	${MROFF_CMD} ${MAN6} | ${COL} -bx > ${.TARGET}
