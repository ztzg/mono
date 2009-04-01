
# Makefile for the trivial test suite.
#
# Please adapt MCS and RUNTIME values to your environnement.
#
# This Makefile is typically with command lines such as:
#
# make all
# gmake TOOL_PATH=mytool_path all

TOOL_PATH=/sw/st/gnu_compil/gnu/linux-rh-ws-3/.package/mono-1.9/bin
MCS=$(TOOL_PATH)/mcs
RUNTIME=$(TOOL_PATH)/mono

all: fp1 fp2 fp3 fp4 fp5 fp6 fp7 fp8 fp9 fp10 fp11 fp12

fp%.exe: fp%.cs
	$(MCS) -out:$@ $<

fp1: fp1.exe
	$(RUNTIME) --optimize=-inline $< ; test $$? = 23

fp2: fp2.exe
	$(RUNTIME) --optimize=-inline $< ; test $$? = 23

fp3: fp3.exe
	$(RUNTIME) --optimize=-inline $< ; test $$? = 23

fp4: fp4.exe
	$(RUNTIME) --optimize=-inline $< ; test $$? = 198

fp5: fp5.exe
	$(RUNTIME) --optimize=-inline $< ; test $$? = 27

fp6: fp6.exe
	$(RUNTIME) --optimize=-inline $< ; test $$? = 80

fp7: fp7.exe
	$(RUNTIME) --optimize=-inline $< ; test $$? = 30

fp8: fp8.exe
	$(RUNTIME) --optimize=-inline $< ; test $$? = 30

fp9: fp9.exe
	$(RUNTIME) --optimize=-inline $< ; test $$? = 47

fp10: fp10.exe
	$(RUNTIME) --optimize=-inline $< ; test $$? = 6

fp11: fp11.exe
	$(RUNTIME) --optimize=-inline $< ; test $$? = 5

fp12: fp12.exe
	$(RUNTIME) --optimize=-inline $< ; test $$? = 6

clean:
	rm -f *.exe