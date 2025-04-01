setup:
	bash scripts/setup.sh

osx: setup
	bash scripts/osx.sh

windows: setup
	bash scripts/windows.sh

build_osx: osx
	bash scripts/build.sh

build_windows: windows
	bash scripts/build.sh