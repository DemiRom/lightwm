CC     = cl
LD     = link
RC     = rc
CFLAGS = /EHsc

EXE_SRCS = wm.c tiling.c error.c config.c keyboard.c
EXE_NAME = lightwm.exe
EXE_RC = wm_resources.obj

DBGDIR = debug
DBGEXE = $(DBGDIR)/$(EXE_NAME)
DBG_EXE_OBJS = $(addprefix $(DBGDIR)/, $(EXE_SRCS:.c=.obj))
DBGCFLAGS = $(CFLAGS) /DDEBUG /Zi

RELDIR = release
RELEXE = $(RELDIR)/$(EXE_NAME)
REL_EXE_OBJS = $(addprefix $(RELDIR)/, $(EXE_SRCS:.c=.obj))
RELCFLAGS = $(CFLAGS) /Ox
 

.PHONY: all clean debug prep release

# Default build
all: clean prep release

#
# Debug rules
#
debug: clean prep $(DBGEXE)

$(DBGEXE): $(DBG_EXE_OBJS) $(DBGDIR)/$(EXE_RC)
	$(CC) $(DBGCFLAGS) /Fe:$@ $^ /link user32.lib shell32.lib ole32.lib shlwapi.lib

$(DBGDIR)/%.obj: %.c
	$(CC) $(DBGCFLAGS) /c /Fo:$@ $<

$(DBGDIR)/%.obj: %.rc
	$(RC) /fo $@ $<

#
# Release rules
#
release: prep $(RELEXE)

$(RELEXE): $(REL_EXE_OBJS) $(RELDIR)/$(EXE_RESS)
	$(CC) $(RELCFLAGS) /Fe:$@ $^ /link user32.lib shell32.lib ole32.lib shlwapi.lib

$(RELDIR)/%.obj: %.c
	$(CC) $(RELCFLAGS) /c /Fo:$@ $<

$(RELDIR)/%.obj: %.rc
	$(RC) /fo $@ $<

#
# Prep and clean rules
#
prep:
	@echo off > temp.bat && \
	echo IF NOT EXIST $(DBGDIR) mkdir $(DBGDIR) >> temp.bat && \
	echo IF NOT EXIST $(RELDIR) mkdir $(RELDIR) >> temp.bat && \
	temp.bat && \
	del temp.bat

clean:
	@echo off > temp.bat && \
	echo IF EXIST $(DBGDIR) del /S /Q $(DBGDIR) >> temp.bat && \
	echo IF EXIST $(RELDIR) del /S /Q $(RELDIR) >> temp.bat && \
	echo IF EXIST $(DBGDIR) rd /S /Q $(DBGDIR) >> temp.bat && \
	echo IF EXIST $(RELDIR) rd /S /Q $(RELDIR) >> temp.bat && \
	temp.bat && \
	del temp.bat
