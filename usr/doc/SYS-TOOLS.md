# Zen System Tools

Note: these tools are heavily inspired by the Linux kernel and distributions. I hereby want to express thank you to all authors and contributors of these systems. :)

## sysctl

### CONFIGURATION FILES

The configuration file is located at

    /etc/sysctl.conf

The configuration lines that start with a # or ; are considered comments and ignored.

Actual configuration lines have syntax like

kernel.net.eth0 = PCIADR
kernel.net.eth0.ip4 = TRUE
kernel.net.eth0.ip6 = FALSE
kernel.net.ip4.recvbuf = 4096 16384 65536

## pkg

The package manager supports the followowing operations.

### CONFIGURATION FILES

    /etc/pkg/pkg.conf
    /etc/pkg/pkg-local.conf

### DATABASE AND PACKAGE SOURCES

Relevant command retrievepackage databases as listed in the plain text files

    /etc/pkg/db-local.txt
    /etc/pkg/db.txt
    /etc/pkg/db-custom.txt

### update

Synchronize package databases (see DATABASES).

### upgrade

Upgrade all installed packages after retrieving them.

Before the upgrades, the
latest versions are retrieved (see DATABASES).

### install [PKG..,]

Install packages listed on the command line (see DATABASES).

You may prefix package names with - or + to request them to be removed or installed respectively.

The version of a package may be specified by appending a = and the package version to the name.

### remove [PKG...]

