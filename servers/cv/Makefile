# Makefile for CV server
PROG=	cv
SRCS=	utility.c list.c process.c mutex.c condition-var.c main.c

DPADD+=	${LIBSYS}
LDADD+=	-lsys

MAN=

BINDIR?= /usr/sbin
FILES=cv.conf
FILESNAME=cv
FILESDIR= /etc/system.conf.d

.include <minix.service.mk>
