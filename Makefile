PROG=		oggquiz
SRCS=		${PROG}.c oggfile.c player.c ui.c

NO_MAN=         yes

CLEANFILES=	*.[Bb][Aa][Kk] *.core

CFLAGS+=	-Wall --ansi --pedantic
CFLAGS+=	-I/usr/local/include
LDFLAGS+=	-L/usr/local/lib 
LDADD+=		-lvorbisfile
LDADD+=		-liconv

CTAGS=		ctags

FIND=		/usr/bin/find
INDENT=		/usr/bin/indent
XARGS=		/usr/bin/xargs

indent: .PHONY
	${FIND} . -type f -name '*.[c,h]' | ${XARGS} -n 1 ${INDENT}

.include <bsd.prog.mk>
