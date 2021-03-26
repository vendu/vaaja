#! /bin/sh

. ./kern.sh

conf()
{
    echo "configuring GRUB in $GRUB_SRC_DIR"
    cd $GRUB_SRC_DIR
    if [ ! -f config.h ]; then
	./autogen.sh
        autoreconf -i
    fi
    ./configure --prefix="$GRUB_BUILD_DIR" --target=i686
    make install
    cd $KERN_BUILD_DIR

    return $?
}

build()
{
    echo "building GRUB in $GRUB_SRC_DIR"
    cd $GRUB_SRC_DIR
    make -j 5
    cd $KERN_BUILD_DIR

    return $?
}

fetch()
{
    echo "fetching GRUB from $GRUB_GIT_URI"
    git clone $GRUB_GIT_URI

    return $?
}

update()
{
    echo "updating GRUB in $GRUB_SRC_DIR"
    cd $GRUB_SRC_DIR
    git pull && build
    cd $KERN_BUILD_DIR

    return $?
}

install()
{
    curdir=`pwd`
    echo "installing GRUB"
    cd $GRUB_SRC_DIR
    cd grub
    make install
    cd $KERN_BUILD_DIR

    return $?
}

mkgrub()
{
    if [ ! -d grub ]; then
	fetch
    fi
    update && install

    return $?
}

mkgrub $@

