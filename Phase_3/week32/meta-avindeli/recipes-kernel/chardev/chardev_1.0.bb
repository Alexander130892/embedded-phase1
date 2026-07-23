SUMMARY = "Character device kernel module (Week 27 chardev, Yocto recipe)"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://chardev.c;beginline=1;endline=1;md5=fcab174c20ea2e2bc0be64b493708266"

inherit module

SRC_URI = "file://chardev.c \
           file://Makefile \
          "

S = "${WORKDIR}"

KERNEL_MODULE_AUTOLOAD += "chardev"