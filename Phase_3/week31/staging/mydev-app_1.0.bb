SUMMARY = "Autoload user_program"

LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://user_program.c;beginline=1;endline=1;md5=fcab174c20ea2e2bc0be64b493708266"

SRC_URI = "file://user_program.c \
           file://mydev-mknod.service \
           file://mydev-app.service \
          "
S = "${WORKDIR}"

inherit systemd

do_compile () {
    ${CC} ${CFLAGS} ${LDFLAGS} -o user_program user_program.c
}

do_install () {
    install -d ${D}${bindir}
    install -m 0755 ${WORKDIR}/user_program ${D}${bindir}

    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/mydev-mknod.service ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/mydev-app.service ${D}${systemd_system_unitdir}
}

SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE:${PN} = "mydev-mknod.service mydev-app.service"
SYSTEMD_AUTO_ENABLE = "enable"

