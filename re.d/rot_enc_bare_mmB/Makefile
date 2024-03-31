
all:
	pio run

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
	rm -rf ./.pio

# rm -rf ./.pio/build

# do clean .pio
# clone direct to lib dir

