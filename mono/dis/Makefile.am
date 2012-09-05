INCLUDES = $(GLIB_CFLAGS) -I$(top_srcdir)

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

if DISABLE_EXECUTABLES
bin_PROGRAMS =
else
if SUPPORT_BOEHM
bin_PROGRAMS = monodis
endif
endif

noinst_LIBRARIES = libmonodis.a

libmonodis_a_SOURCES =	\
	get.c		\
	get.h		\
	dis-cil.c	\
	dis-cil.h	\
	util.c		\
	util.h

monodis_SOURCES =	\
	dump.c		\
	dump.h		\
	main.c		\
	meta.h		\
	declsec.c	\
	declsec.h

monodis_LDADD = 			\
	libmonodis.a			\
	$(runtime_lib)			\
	$(LLVM_LIBS)			\
	$(GLIB_LIBS)			\
	$(LIBICONV)

if PLATFORM_DARWIN
monodis_LDFLAGS=-framework CoreFoundation
endif

man_MANS = monodis.1

EXTRA_DIST = $(man_MANS)
