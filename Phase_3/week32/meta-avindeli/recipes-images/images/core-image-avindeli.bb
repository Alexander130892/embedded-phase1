require recipes-core/images/core-image-minimal.bb
IMAGE_INSTALL:append = " openssh-sftp-server strace hello chardev mydev-app"
