.DEFAULT_GOAL := all
PYTHON ?= python3
.PHONY: all check release clean
all:
	$(PYTHON) standalone/build.py --profile portable --output build/FAT32.so
check:
	$(PYTHON) standalone/build.py --profile portable --output build/FAT32.so --check
release:
	$(PYTHON) standalone/release.py release
clean:
	rm -rf build dist
