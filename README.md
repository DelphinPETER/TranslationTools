Translation Tools
=================

Translation Tools is a program, developed by the French OmniRom translator Delphin PETER, to assist OmniRom translators in translating OmniRom into their language.

Dependencies
------------

Please install the following packages before you launch Translation Tools. These packages are required.

Ubuntu 10.04 (x64), 12.04 (x86/x64):

    sudo apt-get install libqt4-gui libqt4-xml libqt4-core

Ubuntu 13.04 (x64):

    sudo apt-get install libqt5gui5 libqt5xml5 libqt5core5 libqt5widgets5

Fedora 19 (x64):

    yum install qt5-qtbase qt5-qtbase-x11

Fedora 20 (x64):

    yum install qt5-qtx11extras qt5-qtbase

Installing
----------

You can install Translation Tools by adding it to the local manifests of your local repo. Choose the right version of your android Rom, adapt the following and execute it in the root of your local repo:

    curl https://raw.github.com/DelphinPETER/TranslatorRepo/ANDROID_VERSION/TranslationTools.xml > .repo/local_manifests/TranslationTools.xml
    repo sync

After sync'ing, you can find Translation Tools under tools/TranslationTools.

Translating Translation Tools
-----------------------------

The interface of Translation Tools is translatable. Use res/string.xml (or any other language) as your base. Submit your translation by creating a pull request.

Uninstalling
------------

You can simply uninstall by removing the local manifest and sync again.

    rm .repo/local_manifests/TranslationTools.xml
    repo sync
