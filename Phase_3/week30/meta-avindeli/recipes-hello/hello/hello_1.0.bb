

SUMMARY = "Hello World!, first yocto app"

LICENSE = "CLOSED"

SRC_URI = "	file://hello.c	\
		file://Makefile \
		"
S = "${WORKDIR}"

EXTRA_OEMAKE = "CC='${CC}' CFLAGS='${CFLAGS}' LDFLAGS='${LDFLAGS}'"

do_install () {
    install -d ${D}${bindir}
    install -m 0755 ${S}/hello ${D}${bindir}
}

