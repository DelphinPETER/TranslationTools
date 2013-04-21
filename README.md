Translation Tools
======

How to add Translation Tools and/or translatable device
------

In your root source folder :
<blockquote>
    <p>curl https://raw.github.com/DelphinPETER/TranslationTools/cm-10.1/repo_file/TranslationTools.xml > .repo/local_manifests/TranslationTools.xml</p>
    <p>repo sync</p>
</blockquote>

When Translation Tools will download, you will find it in SOURCE_FOLDER/tools/TranslationTools/binary

If you want all translatable device

(WARNING : if you are already download device with "breakfast", maybe you need make a "repo init")
<blockquote>
    <p>curl https://raw.github.com/DelphinPETER/TranslationTools/cm-10.1/repo_file/device.xml > .repo/local_manifests/device.xml</p>
    <p>repo sync</p>
</blockquote>

To remove Translation Tools or device
-----

Remove Translation Tools :
<blockquote>
    <p>rm .repo/local_manifests/TranslationTools.xml</p>
    <p>repo sync</p>
</blockquote>

Remove device :
<blockquote>
    <p>rm .repo/local_manifests/device.xml</p>
    <p>repo sync</p>
</blockquote>
