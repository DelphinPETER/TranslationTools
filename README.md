Translation Tools
======


How to add Translation Tools repository to your source folder :

Simply in your root source folder :
<blockquote>
    <p>wget -P .repo/local_manifests/  https://raw.github.com/DelphinPETER/TranslationTools/cm-10.1/repo_file/TranslationTools.xml</p>
    <p>repo sync</p>
</blockquote>

When TranslationTools will download you will find in SOURCE_FOLDER/tools/TranslationTools/binary

To remove Translation Tools in your source folder :

<blockquote>
    <p>rm .repo/local_manifests/TranslationTools.xml</p>
    <p>repo sync</p>
</blockquote>
