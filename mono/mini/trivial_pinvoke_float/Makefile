
MCS=mcs
RUNTIME=mono
CC=gcc

all: fp1 fp2 fp3 fp4 fp5 fp6 fp7 fp8 fp9 fp10 fp11 fp12

fp%.exe: fp%.cs fp.so
	$(MCS) -out:$@ $<

fp.so: fp.c
	$(CC) -shared $< -o $@

fp1: fp1.exe
	$(RUNTIME) $< ; test $$? = 23

fp2: fp2.exe
	$(RUNTIME) $< ; test $$? = 23

fp3: fp3.exe
	$(RUNTIME) $< ; test $$? = 23

fp4: fp4.exe
	$(RUNTIME) $< ; test $$? = 198

fp5: fp5.exe
	$(RUNTIME) $< ; test $$? = 27

fp6: fp6.exe
	$(RUNTIME) $< ; test $$? = 80

fp7: fp7.exe
	$(RUNTIME) $< ; test $$? = 30

fp8: fp8.exe
	$(RUNTIME) $< ; test $$? = 30

fp9: fp9.exe
	$(RUNTIME) $< ; test $$? = 47

fp10: fp10.exe
	$(RUNTIME) $< ; test $$? = 6

fp11: fp11.exe
	$(RUNTIME) $< ; test $$? = 5

fp12: fp12.exe
	$(RUNTIME) $< ; test $$? = 6

clean:
	rm -f *.exe