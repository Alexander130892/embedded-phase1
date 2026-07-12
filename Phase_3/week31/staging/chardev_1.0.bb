SUMMARY = "Character device kernel module (Week 27 chardev, Yocto recipe)"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://chardev.c;beginline=1;endline=1;md5=68b329da9893e34099c7d8ad5cb9c940"

inherit kernel-module

SRC_URI = "file://chardev.c \
           file://Makefile \
          "

S = "${WORKDIR}"

KERNEL_MODULE_AUTOLOAD += "chardev"