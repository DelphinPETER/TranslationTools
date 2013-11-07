Translation Tools
=================

Translation Tools is a program, developed by the French CyanogenMod translator Delphin PETER, to assist CyanogenMod translators in translating CyanogenMod into their language. You can find more information at the [CyanogenMod wiki](http://wiki.cyanogenmod.org/w/Doc:_Translation_Tool).

Dependencies
------------

Please install the following packages before you launch Translation Tools. These packages are required.

Ubuntu 10.04 (x64), 12.04 (x86/x64):

    sudo apt-get install libqt4-gui libqt4-xml libqt4-core

Ubuntu 13.04 (x64):

    sudo apt-get install libqt5gui5 libqt5xml5 libqt5core5 libqt5widgets5

Fedora 19 (x64):

    yum install qt5-qtbase qt5-qtbase-x11

Installing
----------

You can install Translation Tools by adding it to the local manifests of your local CyanogenMod repo. Execute the following in the root of your local repo:

    curl https://raw.github.com/DelphinPETER/CyanogenMod_TranslatorRepo/cm-11.0/TranslationTools.xml > .repo/local_manifests/TranslationTools.xml
    repo sync

After sync'ing, you can find Translation Tools under tools/TranslationTools/binary.

There is a helper script available for launching Translation Tools, created by the German CyanogenMod translator Jacob Müller. You can find more information on [this page](https://github.com/jackmu95/tt).

Extra repositories
------------------

There are other repositories available that are not downloaded by default. Check out [this page](https://github.com/DelphinPETER/CyanogenMod_TranslatorRepo) for more information.

Translating Translation Tools
-----------------------------

The interface of Translation Tools is translatable. Use source/language_fr.ts (or any other language) as your base; language_fr contains both French and English texts. Submit your translation by creating a pull request.

Uninstalling
------------

You can simply uninstall by removing the local manifest and sync again.

    rm .repo/local_manifests/TranslationTools.xml
    repo sync
