bin_PROGRAMS = rootsh
rootsh_SOURCES = rootsh.c readconf.c username.c
if LOGTOSYSLOG
rootsh_SOURCES += write2syslog.c
endif
rootsh_LDADD = @LIBOBJS@
