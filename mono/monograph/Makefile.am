
if HOST_WIN32
export HOST_CC
endif

if JIT_SUPPORTED
if STATIC_MONO
monodir=$(top_builddir)

# This is needed for automake dependency generation
if INCLUDED_LIBGC
libgc_libs=$(monodir)/libgc/libmonogc.la
libgc_static_libs=$(monodir)/libgc/libmonogc-static.la
else
libgc_libs=$(LIBGC_LIBS)
libgc_static_libs=$(LIBGC_STATIC_LIBS)
endif

static_libs=	\
	$(monodir)/mono/metadata/libmonoruntime-static.la	\
	$(monodir)/mono/io-layer/libwapi.la	\
	$(monodir)/mono/utils/libmonoutils.la \
	$(GLIB_LIBS) $(LIBICONV) \
	$(libgc_static_libs)

runtime_lib=../mini/libmini-static.la $(static_libs)
else
runtime_lib=../mini/$(LIBMONO_LA)
endif
else
runtime_lib=../interpreter/libmint.la
endif

bin_PROGRAMS = monograph

INCLUDES = 				\
	-I$(top_srcdir)			\
	$(GLIB_CFLAGS)

monograph_LDADD = \
	$(runtime_lib)			\
	$(GLIB_LIBS)			\
	$(LLVM_LIBS)			\
	$(LIBICONV)			\
	-lm

if PLATFORM_DARWIN
monograph_LDFLAGS=-framework CoreFoundation
endif

GRAPHS=System.Object System.Enum System.Attribute System.ValueType System.Reflection.MemberInfo
OUT=$(GRAPHS:=.jpeg)

graphs: $(OUT)

%.jpeg: monograph
	./monograph -n -o $*.png corlib.dll $*
	convert -geometry '480x360>' $*.png $*.jpeg

EXTRA_DIST = ChangeLog

