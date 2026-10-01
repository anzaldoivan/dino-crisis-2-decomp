# make-format.snippet.mk — appended to the project's Makefile by decomp-architect (install.py S9).
# `make format` rewrites every C source and header under src/ with the tracked .clang-format; `make format-check`
# is the same run in dry mode with warnings as errors (a CI-able check, no game bytes needed). Dotfiles under src/
# are excluded so a tool's live probe file is never formatted into the tree.

.PHONY: format format-check extract

format:
	find src -type f \( -name '*.c' -o -name '*.h' \) -not -name '.*' -print0 | xargs -0 -r clang-format -i --style=file

format-check:
	find src -type f \( -name '*.c' -o -name '*.h' \) -not -name '.*' -print0 | xargs -0 -r clang-format --dry-run --Werror --style=file

# make extract [OUT=dir] — our own disc extractor (tools/extract_disc.py) → $(OUT)/files + manifest; see docs/formats.md.
OUT ?= extracted/retail

extract:
	python3 tools/extract_disc.py --out $(OUT)

# ---- Byte-identical rebuild of the fleet (docs/ops/docker-host.md "Build"). Container only (splat, mipsel binutils).
# make [-j] split|build|expected [BASEDIR=dir] [ONLY="alias …"] · make clean (generated outputs only: asm/,
# build/overlays.mk, per alias build/<alias>/ + build/<alias>.{bin,bin.bad,elf,map,ld,override.yaml}; any other
# build/ entry survives). BASEDIR default: first existing of extracted/retail/files (host), .run/extracted/retail/files
# (container). expected: all N aliases green, then build/<alias>.bin -> expected/<alias>.bin (else rc!=0, untouched).
# Per alias (config/splat/<alias>.yaml, never edited): splat split with a generated build/<alias>.override.yaml
# (target_path under $(BASEDIR); splat merges later configs) → asm/<alias>/; assemble every object the splat .ld
# names (.s, .bin; a C unit: cpp → cc1 → maspsx → as, the pinned triple below, .i/.s beside its .o); link; objcopy (odd-size tail: shrink-only trim ≤ 3 B, only when size(build) > size(target): SUBALIGN(4)
# pads the end); `sha1sum -c config/check.<alias>.sha`. build/overlays.mk is generated from the YAML list.
# SPIMDISASM_SYMBOL_ALIGNMENT_REQUIRES_ALIGNED_SECTION: no `.align 3` on a jtbl in a non-8-aligned file.
.PHONY: split build expected clean FORCE

BASEDIR ?= $(firstword $(wildcard extracted/retail/files .run/extracted/retail/files) extracted/retail/files)
YAMLS   := $(sort $(wildcard config/splat/*.yaml))
ALIASES := $(patsubst config/splat/%.yaml,%,$(YAMLS))
ONLY    ?= $(ALIASES)
ifneq ($(filter-out $(ALIASES),$(ONLY)),)
$(error unknown alias in ONLY: $(filter-out $(ALIASES),$(ONLY)))
endif

SPLAT   := SPIMDISASM_SYMBOL_ALIGNMENT_REQUIRES_ALIGNED_SECTION=True /opt/splat/bin/python -m splat
CROSS   := mipsel-linux-gnu-
ASFLAGS := -march=r3000 -mabi=32 -G0 -no-pad-sections
# Pinned C triple (docs/ops/decomp-environment.md, T4): psyq4.6 cc1 via wibo → maspsx (aspsx version explicit) → as.
# Per-alias override hook: CPPFLAGS_<alias>, CFLAGS_<alias>, MASPSXFLAGS_<alias> (default empty, appended).
CPP         := $(CROSS)cpp
CC1         := /opt/cc/wibo/wibo /opt/cc/psyq4.6/CC1PSX.EXE
MASPSX      := python3 /opt/cc/maspsx/maspsx.py
CPPFLAGS    := -P -undef -nostdinc -D__GNUC__=2 -Iinclude
CFLAGS      := -quiet -O2 -G0 -mips1 -fno-builtin
MASPSXFLAGS := --aspsx-version=2.86 -G0

split: $(foreach a,$(ONLY),build/$(a)/split.stamp)
build: $(foreach a,$(ONLY),build/$(a).bin)
expected: $(foreach a,$(ALIASES),build/$(a).bin)
	@for a in $(ALIASES); do [ -f build/$$a.bin ] && [ ! -e build/$$a.bin.bad ] \
	  || { echo "REFUSED expected: build/$$a.bin missing or bad" >&2; exit 1; }; done
	@mkdir -p expected && for a in $(ALIASES); do cp build/$$a.bin expected/$$a.bin; done
	@echo "expected: $(words $(ALIASES)) binaries"
clean:
	@echo 'clean: asm/ build/overlays.mk + generated build/<alias>* for $(words $(ALIASES)) aliases'
	@rm -rf asm build/overlays.mk $(foreach a,$(ALIASES),build/$(a) $(addprefix build/$(a).,bin bin.bad bin.tmp elf map ld override.yaml override.yaml.tmp))
FORCE:

# $(1) alias, $(2) path under BASEDIR
define alias_rules
build/$(1).override.yaml: FORCE
	@mkdir -p build
	@printf 'options:\n  target_path: %s\n' '$(BASEDIR)/$(2)' > $$@.tmp
	@if cmp -s $$@.tmp $$@; then rm -f $$@.tmp; else mv $$@.tmp $$@; fi

build/$(1)/split.stamp: config/splat/$(1).yaml build/$(1).override.yaml
	@rm -rf asm/$(1) build/$(1) && mkdir -p build/$(1)
	@$(SPLAT) split $$^ > build/$(1)/split.log 2>&1 || { tail -5 build/$(1)/split.log; echo "FAILED split: $(1)"; exit 1; }
	@touch $$@

build/$(1).bin: build/$(1)/split.stamp $$(shell find asm/$(1) -name '*.s' 2>/dev/null) \
  $$(shell find src/$(1) -name '*.c' 2>/dev/null) $$(wildcard include/*.h)
	@rm -f $$@.bad
	@for o in $$$$(grep -o 'build/$(1)/[^ ]*\.o' build/$(1).ld | sort -u); do \
	  s=$$$${o#build/$(1)/}; s=$$$${s%.o}; mkdir -p $$$$(dirname $$$$o); \
	  case $$$$s in \
	    *.s) $(CROSS)as $(ASFLAGS) -I build/$(1)/include -o $$$$o $$$$s ;; \
	    *.c) $(CPP) $(CPPFLAGS) $(CPPFLAGS_$(1)) $$$$s -o $$$${o%.o}.i \
      && $(CC1) $(CFLAGS) $(CFLAGS_$(1)) $$$${o%.o}.i -o $$$${o%.o}.s \
      && $(MASPSX) $(MASPSXFLAGS) $(MASPSXFLAGS_$(1)) $$$${o%.o}.s > $$$${o%.o}.m.s \
      && $(CROSS)as $(ASFLAGS) -I build/$(1)/include -o $$$$o $$$${o%.o}.m.s ;; \
    *.bin) printf '.section .data\n.incbin "%s"\n' $$$$s | $(CROSS)as $(ASFLAGS) -o $$$$o - ;; \
	    *) false ;; \
	  esac || { echo "FAILED assemble: $(1): $$$$s"; exit 1; }; \
	done
	@# func_<ADDR> referenced (e.g. a `j` out of the overlay) but neither defined nor in undefined_funcs_auto
	@{ grep -rhoE 'func_[0-9A-F]{8}' asm/$(1) | sort -u; \
	  grep -rhoE '^ *[ag]label func_[0-9A-F]{8}' asm/$(1) | grep -oE 'func_[0-9A-F]{8}' | sort -u | sed p; \
	  grep -oE '^func_[0-9A-F]{8}' build/$(1)/undefined_funcs_auto.txt | sed p; } | sort | uniq -u \
	  | sed 's/^func_\(.*\)$$$$/func_\1 = 0x\1;/' > build/$(1)/undefined_jumps_auto.txt
	@$(CROSS)ld -nostdlib --no-check-sections -Map build/$(1).map -T build/$(1).ld \
	  -T build/$(1)/undefined_syms_auto.txt -T build/$(1)/undefined_funcs_auto.txt \
	  -T build/$(1)/undefined_jumps_auto.txt -o build/$(1).elf \
	  || { echo "FAILED link: $(1)"; exit 1; }
	@$(CROSS)objcopy -O binary build/$(1).elf $$@.tmp
	@t=$$$$(stat -c %s '$(BASEDIR)/$(2)'); b=$$$$(stat -c %s $$@.tmp); \
	  if [ $$$$b -gt $$$$t ] && [ $$$$((b - t)) -le 3 ]; then truncate -s $$$$t $$@.tmp; fi
	@mv $$@.tmp $$@; (cd build && sha1sum -c ../config/check.$(1).sha) \
	  || { mv $$@ $$@.bad; echo "FAILED sha1: $(1) (build/$(1).bin.bad)"; exit 1; }
endef

ifeq ($(filter-out clean format format-check extract,$(or $(MAKECMDGOALS),all)),)
else
include build/overlays.mk
endif

build/overlays.mk: $(YAMLS)
	@mkdir -p build
	@for y in $(YAMLS); do a=$$(basename $$y .yaml); \
	  p=$$(sed -n 's|^  target_path: extracted/retail/files/||p' $$y); \
	  [ -n "$$p" ] || { echo "FAILED: no target_path in $$y" >&2; exit 1; }; \
	  printf '$$(eval $$(call alias_rules,%s,%s))\n' $$a $$p; done > $@.tmp && mv $@.tmp $@
