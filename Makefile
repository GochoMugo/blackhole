help:
	@echo
	@echo "  clean                Clean builds"
	@echo "  cross                Cross-compile"
	@echo "  help                 Show this help info"
	@echo

clean:
	rm -rf build
	rm -f deps/iniparser/libiniparser.a
	rm -rf deps/libgit2/build
	rm -f cross-compile

cross: clean deps
	./cross-compile bash -c 'make _cross'
	rm -rf release && mkdir release
	cp build/bh release/bh
	cp script/cron-wrapper.sh release/bh-cron-wrapper
	{ echo 'install:' ; echo '	cp bh bh-cron-wrapper /bin' ; } > release/Makefile

_cross:
	dpkg --add-architecture arm64
	apt-get update
	apt install -y libssh2-1-dev libssh2-1-dev:arm64
	cp /usr/include/libssh2* /buildroot/aarch64-buildroot-linux-gnu/sysroot/usr/include/
	mkdir build && cd build && cmake .. -DCROSS=ON && make

deps:
	git submodule init
	git submodule update
	clib install --skip-cache `cat clib.json | jq '.dependencies | to_entries | map([.key,.value] | join("@")) | .[]' -r | tr '\n' ' '`
	docker run --rm dockcross/linux-arm64-full > cross-compile
	chmod +x cross-compile

.PHONY: clean cross _cross deps help
