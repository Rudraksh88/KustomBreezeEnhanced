#!/bin/sh

ORIGINAL_DIR=$(pwd)

confirm() {
    # call with a prompt string or use a default
    read -r -p "${1:-Are you sure? [y/N]} " response
    case "$response" in
        [yY][eE][sS]|[yY])
            true
            ;;
        *)
            false
            ;;
    esac
}

uninstall() {
    sudo make uninstall && echo "Uninstalled successfully!"
}

# Recreate install manifest and uninstall
install_and_uninstall() {
    sh install.sh &&
    cd build &&
    uninstall
    echo It is possible that some files left over at the following locations
    echo Please remove any files containing sierrabreezeenhanced in their name
    echo - /usr/lib/qt/plugins/org.kde.kdecoration3/
    echo - /usr/share/kservices5/
    echo - /usr/lib/
}

if [ ! -d "build" ]; then
    # If no build folder is found
    confirm "No installation found, (re)install and uninstall? [y/n]" && install_and_uninstall
else
    if test -f "$ORIGINAL_DIR/build/install_manifest.txt"; then
        # Remove normally
        echo Found $ORIGINAL_DIR/build/install_manifest.txt
        cd build &&
        uninstall
    else
        # If no install manifest found
        echo Did not find $ORIGINAL_DIR/build/install_manifest.txt
        confirm "(re)install and uninstall? [y/n]" && install_and_uninstall
    fi
fi

cd $ORIGINAL_DIR

# Clean up
rm -rf build
