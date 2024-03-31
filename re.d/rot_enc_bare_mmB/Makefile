
all:
	pio run

verbose:
	pio run -v

env:
	@echo  "    " export PS1=\\042 make \\044   \\042    \\073 source ./.profile__platformio \# lb

term:
	@echo "sleeping 2"
	@sleep 2
	@echo "running picocom"
	@sleep 2
	./picot.sh /dev/ttyACM0

upload:
	pio run -t upload

install: upload

run:	install term

clean:
	pio run -t clean

distclean: clean
	rm -rf ./.pio/build

# can still cd ./.pio after make distclean but not make purge

purge:
	rm -rf ./.pio

# rm -rf ./.pio
#   do clean .pio/build not .pio/*
#   clone direct to lib dir

