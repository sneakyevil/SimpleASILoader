## Simple ASI Loader
- Simple hijackable dll that loads ASI modules/plugins.
- Any `.asi` file in process/plugins/scripts directory will be automatically loaded at launch of the hijacked dll (ASI Loader).

## Supported DLLs
<table>
  <tr>
    <th>Name</th>
    <th>x86</th>
    <th>x64</th>
  </tr>
  <tr>
    <td>cryptbase</td>
    <td>-</td>
    <td>-</td>
  </tr>
  <tr>
    <td>dinput8</td>
    <td><a href="https://github.com/sneakyevil/SimpleASILoader/releases/download/dinput8_x86/dinput8.dll">Get</a></td>
    <td><a href="https://github.com/sneakyevil/SimpleASILoader/releases/download/dinput8_x64/dinput8.dll">Get</a></td>
  </tr>
  <tr>
    <td>version</td>
    <td><a href="https://github.com/sneakyevil/SimpleASILoader/releases/download/version_x86/version.dll">Get</a></td>
    <td><a href="https://github.com/sneakyevil/SimpleASILoader/releases/download/version_x64/version.dll">Get</a></td>
  </tr>
  <tr>
    <td>vorbisfile</td>
    <td><a href="https://github.com/sneakyevil/SimpleASILoader/releases/download/vorbisfile/vorbisfile.dll">Get</a></td>
    <td>-</td>
  </tr>
</table>

## Compiling
- Visual Studio 2019 or later
- Clang Compiler (LLVM)

## Preprocessor Defines
- EXPORTS_ALL (Exports everything)
- EXPORTS_CRYPTBASE
- EXPORTS_DINPUT8
- EXPORTS_VERSION
- EXPORTS_VORBISFILE