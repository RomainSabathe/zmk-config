.PHONY: \
	help \
	format \
	zmk-sofle zmk-sofle-left zmk-sofle-right \
	zmk-chocofi zmk-chocofi-left zmk-chocofi-right \
	qmk-user-id75-from-zmk-callum-mods \
	qmk-user-id75-from-zmk-simple \
	qmk-user-id75-from-zmk \
	qmk-user-id75-via \
	qmk-user-id75-default \
	qmk-mt-id75-via \
	qmk-mt-id75-default \
	clean

help:
	@echo "Available targets:"
	@echo "  format                              Format the Sofle ZMK keymap"
	@echo "  zmk-sofle                           Build both Sofle halves"
	@echo "  zmk-sofle-left                      Build Sofle left half"
	@echo "  zmk-sofle-right                     Build Sofle right half"
	@echo "  zmk-chocofi                         Build both Chocofi halves"
	@echo "  zmk-chocofi-left                    Build Chocofi left half"
	@echo "  zmk-chocofi-right                   Build Chocofi right half"
	@echo "  qmk-user-id75-from-zmk-callum-mods  Build user/id75 from_zmk_callum_mods"
	@echo "  qmk-user-id75-from-zmk-simple       Build user/id75 from_zmk_simple"
	@echo "  qmk-user-id75-from-zmk              Build user/id75 from_zmk"
	@echo "  qmk-user-id75-via                   Build user/id75 via"
	@echo "  qmk-user-id75-default               Build user/id75 default"
	@echo "  qmk-mt-id75-via                     Build mt/id75 via"
	@echo "  qmk-mt-id75-default                 Build mt/id75 default"
	@echo "  clean                               Remove local build outputs"

format:
	utils/format_keymap.py

zmk-sofle:
	cd zmk && ./build.sh sofle

zmk-sofle-left:
	cd zmk && ./build.sh sofle left

zmk-sofle-right:
	cd zmk && ./build.sh sofle right

zmk-chocofi:
	cd zmk && ./build.sh chocofi

zmk-chocofi-left:
	cd zmk && ./build.sh chocofi left

zmk-chocofi-right:
	cd zmk && ./build.sh chocofi right

qmk-user-id75-from-zmk-callum-mods:
	cd qmk && ./build.sh user/id75 from_zmk_callum_mods

qmk-user-id75-from-zmk-simple:
	cd qmk && ./build.sh user/id75 from_zmk_simple

qmk-user-id75-from-zmk:
	cd qmk && ./build.sh user/id75 from_zmk

qmk-user-id75-via:
	cd qmk && ./build.sh user/id75 via

qmk-user-id75-default:
	cd qmk && ./build.sh user/id75 default

qmk-mt-id75-via:
	cd qmk && ./build.sh mt/id75 via

qmk-mt-id75-default:
	cd qmk && ./build.sh mt/id75 default

clean:
	rm -rf zmk/build qmk/build
