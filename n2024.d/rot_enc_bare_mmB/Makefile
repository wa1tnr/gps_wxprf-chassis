
all:
	@echo;echo;echo;echo;echo;echo
	@echo "  Rot Enc" | figlet ;echo
	@echo;echo;echo;date;echo;echo
	pio run

verbose:
	pio run -v

env:
	@echo  "    " export PS1=\\042 make \\044   \\042    \\073 source ./.profile__platformio \# lb

suite_msgs:
	@echo;echo;echo;  date;  echo;echo; ls --color /dev

suite:	suite_msgs
	@make distclean ; make; make run-no-whine

term:
	@echo "sleeping 2"
	@sleep 2
	@echo "running picocom"
	@sleep 2
	./picot.sh /dev/ttyACM0

no-whine:
	pio run -t upload 2>&1 | cat | ag -v rules

upload:
	pio run -t upload

install: upload

run:	install term

run-no-whine:	no-whine term

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

