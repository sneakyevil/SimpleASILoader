## Simple ASI Loader
- Simple hijackable dll that loads ASI modules/plugins.
- Any `.asi` file in process/plugins/scripts directory will be automatically loaded at launch of the hijacked dll (ASI Loader).

#### Supported DLLs
- version
- vorbisfile

#### Compiling
- Visual Studio 2019 or later
- Clang Compiler (LLVM)

#### Preprocessor Defines
- EXPORTS_ALL (Exports everything)
- EXPORTS_VERSION
- EXPORTS_VORBISFILE