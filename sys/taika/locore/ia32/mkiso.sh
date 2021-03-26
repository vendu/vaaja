#! /bin/sh

# Thanks for this script go to pecorade; I just did cosmetic changes // vendu

. ./kern.sh

mkcfg()
{
    echo "creating GRUB configuration file $GRUB_CFG_FILE"
    mkdir -p $GRUB_CFG_DIR
    touch $GRUB_CFG_FILE
    echo "setting parameters"
    cat > $GRUB_CFG_FILE <<EOF
set timeout=$GRUB_TIMEOUT # wait seconds before start
set default=$GRUB_DEFAULT # default menu entry
set gfxmode=$GRUB_GFXMODE
set gfxpayload=$GRUB_GFXMODE
EOF

    echo "creating boot menu"
    cat >> $GRUB_CFG_FILE <<EOF
menuentry "zero" {
EOF
    echo "insmod:"
    for mod in $GRUB_MODS
    do
	echo " $mod"
	echo "    insmod $mod" >> $GRUB_CFG_FILE
    done
    echo "configuring graphics mode"

    cat >> $GRUB_CFG_FILE <<EOF
    multiboot /boot/zero
    boot
}
EOF

    return $?
}

mkgrub()
{
    echo "building GRUB"

    ./mkgrub.sh $@

    return $?
}

instfile()
{
    optinarg=""
    dir=""
    mode=""
    file=""
    filelist=""

    for opt in
    do
	file=""
	case $opt in
	    -t) # target directory
		optinarg="d"
		;;
            -m) # installation target mode/permissions
		optinarg="m"
		;;
	    *)
		if [ "$optinarg" = "m" ]; then
		    mode="$opt"
		elif [ "$optinarg" = "t" ]; then
		    dir="$opt"
		elif [ test -f "$opt" ]; then
		    if [ ! -z "$dir" ]; then
			echo "file argument following directory $dir"

			exit 1
		    fi
		    if test -n "$file"; then
			if [ test -n "$filelist" ]; then
			    filelist="$filelist $file"
			else
			    filelist="$file"
			fi
			file=""
		    fi
		    if [ test -d "$opt" ]; then
			dir="$opt"
		    elif [ test -f "$opt" ]; then
			file="$opt"
		    else
			echo "unknown option $opt"

			exit 1
		    fi
		elif [ test -d "$opt" ]; then
		    if [ ! -z "$dir" ]; then
			echo "directory argument $opt following directory $dir"

			exit 1
		    fi
		    dir="$opt"
		fi
		optinarg=""
		;;
	esac
    done
    if test -z "$filelist"; then
	filelist="$file"
    fi
    cp -uvPRf $filelist $dir
    if test -n "$mode"; then
	echo "setting files to mode $mode in $dir:"
	for f in filelist
	do
	    if [ !test -f "$dir/$f" ]; then
		chmod $mode "$dir/$f"
	    fi
	done
    fi
}

build()
{
    echo "erasing and recreating $KERN_ISO_DIR"
    rm -rf $KERN_ISO_DIR
    mkdir -p $KERN_ISO_DIR
    echo "creating $GRUB_CFG_DIR"
    mkdir -p $GRUB_CFG_DIR
#    echo "wiping $KERN_IMAGE $KERN_SYMS $GRUB_CFG_FILE"
#    rm -f $KERN_IMAGE $KERN_SYMS $GRUB_CFG_FILE
    echo "Building kernel with GCC"
    ./build-gcc.sh
    echo "copying $KERN_IMAGE to $GRUB_BOOT_DIR/$KERN_IMAGE"
    sudo install -g 0 -o 0 -p  -m 544 $KERN_IMAGE $GRUB_BOOT_DIR
    echo "copying $KERN_SYMS to $GRUB_BOOT_DIR/$KERN_SYMS"
    sudo install -g 0 -o 0 -p  -m 544 $KERN_SYMS $GRUB_BOOT_DIR
    echo "copying $GRUB_CFG_FILE to $GRUB_BOOT_DIR/$GRUB_CFG_FILE"
    sudo install -g 0 -o 0 -p  -m 544 $GRUB_CFG_FILE $GRUB_CFG_DIR
    echo "creating $GRUB_LIB_DIR"
    mkdir -p $GRUB_LIB_DIR
    for f in $GRUB_MODS
    do
        sudo install -g 0 -o 0 -p  -m 544 "$GRUB_MOD_DIR/$f.mod" $GRUB_LIB_DIR
    done

    return 0
}

mkimg()
{
    echo "building boot disc image"
    $GRUB_BUILD_DIR/bin/grub-mkimage --directory=$GRUB_MOD_DIR --prefix=/boot/grub --format=i386-pc --output=zero.img $GRUB_MODS
    cat $GRUB_BUILD_DIR/lib/grub/$GRUB_ARCH/cdboot.img zero.img > $KERN_ISO_DIR/core.img
    grub-mkrescue -o $KERN_ISO_IMAGE $KERN_ISO_DIR
#    genisoimage -v -A "ZERO" --input-charset "iso8859-1" -R -b core.img -no-emul-boot -boot-load-size 4 -boot-info-table -o $KERN_ISO_IMAGE $KERN_ISO_DIR
}

mkiso()
{
    mkcfg $@
    mkgrub $@
    build $@
    mkimg $@
}

mkiso $@

