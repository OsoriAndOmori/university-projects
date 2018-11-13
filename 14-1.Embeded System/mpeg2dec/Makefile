EXEC = mpeg2dec
OBJS = mpeg2dec.o getpic.o motion.o getvlc.o gethdr.o getblk.o getbits.o \
	store.o recon.o spatscal.o idct.o idctref.o display.o systems.o subspic.o verify.o

all: $(EXEC)

#uses non LGPL librarys
$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -msoft-float -o $@ $(OBJS) $(LDLIBS) -lm

romfs:
	$(ROMFSINST) /bin/$(EXEC)

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o

$(OBJS):  config.h global.h mpeg2dec.h getvlc.h 

