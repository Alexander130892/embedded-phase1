FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append = " file://mydev-overlay.dts"

RPI_KERNEL_DEVICETREE_OVERLAYS:append = " overlays/mydev.dtbo"

do_configure:prepend() {
    cp ${WORKDIR}/mydev-overlay.dts ${S}/arch/arm64/boot/dts/overlays/
    sed -i '/dtbo-\$(CONFIG_ARCH_BCM2835) += \\/a\        mydev.dtbo \\' ${S}/arch/arm64/boot/dts/overlays/Makefile
}