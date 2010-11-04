# New ports collection makefile for:	oggquiz
# Date created:			06 Jan 2010
# Whom:				Tobias Rehbein <tobias.rehbein@web.de>
#
# $FreeBSD$

PORTNAME=	oggquiz
PORTVERSION=	1.1.1
CATEGORIES=	games
MASTER_SITES=	# No master site
DISTFILES=	${PORTNAME}-${TAG}.tar

MAINTAINER=	tobias.rehbein@web.de
COMMENT=	A simple quiz on songs from your ogg/vorbis collection

FETCH_DEPENDS=	git:${PORTSDIR}/devel/git
LIB_DEPENDS=	vorbis.4:${PORTSDIR}/audio/libvorbis

USE_ICONV=	yes

MAKE_ARGS=	BINDIR=${PREFIX}/bin \
		MANDIR=${PREFIX}/man/man \
		NO_MANCOMPRESS=yes

#XXX Let this point to your local clone of the git repository
REPO?=		/path/to/repo/clone/

TAG=		v${PORTVERSION}
WRKSRC=		${WRKDIR}/${PORTNAME}-${TAG}

PLIST_FILES=	bin/oggquiz
MAN6=		oggquiz.6

do-fetch:
	( cd "${REPO}" \
		&& ${LOCALBASE}/bin/git archive \
			--prefix "${PORTNAME}-${TAG}/" \
			-o ${DISTDIR}/${DISTFILES} \
			${TAG} )

.include <bsd.port.mk>
