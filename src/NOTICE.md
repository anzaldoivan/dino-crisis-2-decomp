# NOTICE — the sources under `src/`

The C files under `src/` are a **reimplementation of the executable code of *Dino Crisis 2*** (Sony PlayStation (MIPS R3000A),
USA, SLUS-01279). The project wrote them so that the game's own compiler turns them back into the original machine code
byte for byte. They exist for **study, interoperability and preservation**. The purposes are to document how the game
works and to make its code readable. A further purpose is to keep the code buildable after the original tools and media
are gone.

*Dino Crisis 2* and its code are **© TODO(publisher, year)**. The original work is theirs. This project is not
affiliated with, sponsored by or endorsed by the rights holder.

**The project asserts no license over the contents of `src/`.** The contents derive from the copyrighted program. The
project publishes them as a reimplementation for the purposes above, in the manner of other matching decompilations of
commercial games. Nothing here grants anyone more rights in the original work than they already have. Nothing here
should be read as a license to distribute the game or its compiled code.

Do not distribute the compiled output of these sources. By design, that output is identical to the original binaries.
To distribute that output is to distribute the game's code.

This repository does **not** contain the game's executable, its disc or cartridge data, any disassembly listing, any
memory image, or the vendor's SDK. The build requires your own copy of the game (see the top-level `README.md`).

The rest of the repository is the project's own work: the tooling under `tools/`, the build system and the documentation
under `docs/`. The project licenses it under MIT for tools and documents (`LICENSE`). The project claims no rights over
the original game, whose code and assets remain Capcom's. Anyone commercialising a port should contact Capcom first.
`THIRD_PARTY.md` lists the third-party components with their licenses.
