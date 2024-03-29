# Time-stamp: <04/04/30 23:41:02 ptr>
# $Id: targetdirs.mak,v 1.1.2.1 2004/12/24 11:26:35 ptr Exp $

!ifdef TARGET_PROC
TARGET_NAME=$(COMPILER_NAME)-$(TARGET_PROC)
!else
TARGET_NAME=$(COMPILER_NAME)
!endif

OUTPUT_DIR             = obj\$(TARGET_NAME)\shared$(EXTRA_DIRS)
OUTPUT_DIR_DBG         = obj\$(TARGET_NAME)\shared-g$(EXTRA_DIRS)
OUTPUT_DIR_STLDBG      = obj\$(TARGET_NAME)\shared-stlg$(EXTRA_DIRS)

# file to store generated dependencies for make:
DEPENDS_COLLECTION     = obj\$(TARGET_NAME)\.make.depend

# I use the same catalog, as for shared:
OUTPUT_DIR_A           = obj\$(TARGET_NAME)\static$(EXTRA_DIRS)
OUTPUT_DIR_A_DBG       = obj\$(TARGET_NAME)\static-g$(EXTRA_DIRS)
OUTPUT_DIR_A_STLDBG    = obj\$(TARGET_NAME)\static-stlg$(EXTRA_DIRS)

SRCROOT = $(SRCROOT:/=\)
!ifdef CROSS_COMPILING
TARGET_LIB_BIN_SUBFOLDER=$(TARGET_NAME)
!else
TARGET_LIB_BIN_SUBFOLDER=
!endif
INSTALL_LIB_DIR        = $(SRCROOT)\..\lib\$(TARGET_LIB_BIN_SUBFOLDER)
INSTALL_LIB_DIR_DBG    = $(SRCROOT)\..\lib\$(TARGET_LIB_BIN_SUBFOLDER)
INSTALL_LIB_DIR_STLDBG = $(SRCROOT)\..\lib\$(TARGET_LIB_BIN_SUBFOLDER)
INSTALL_STATIC_LIB_DIR = $(SRCROOT)\..\lib\$(TARGET_LIB_BIN_SUBFOLDER)
INSTALL_STATIC_LIB_DIR_DBG    = $(SRCROOT)\..\lib\$(TARGET_LIB_BIN_SUBFOLDER)
INSTALL_STATIC_LIB_DIR_STLDBG = $(SRCROOT)\..\lib\$(TARGET_LIB_BIN_SUBFOLDER)
INSTALL_BIN_DIR        = $(SRCROOT)\..\bin\$(TARGET_LIB_BIN_SUBFOLDER)
INSTALL_BIN_DIR_DBG    = $(SRCROOT)\..\bin\$(TARGET_LIB_BIN_SUBFOLDER)
INSTALL_BIN_DIR_STLDBG = $(SRCROOT)\..\bin\$(TARGET_LIB_BIN_SUBFOLDER)
INSTALL_STATIC_BIN_DIR        = $(SRCROOT)\..\bin\$(TARGET_LIB_BIN_SUBFOLDER)
INSTALL_STATIC_BIN_DIR_DBG    = $(SRCROOT)\..\bin\$(TARGET_LIB_BIN_SUBFOLDER)
INSTALL_STATIC_BIN_DIR_STLDBG = $(SRCROOT)\..\bin\$(TARGET_LIB_BIN_SUBFOLDER)


OUTPUT_DIRS = $(OUTPUT_DIR) $(OUTPUT_DIR_DBG) $(OUTPUT_DIR_STLDBG)
!if "$(OUTPUT_DIR_A)" != "$(OUTPUT_DIR)"
OUTPUT_DIRS = $(OUTPUT_DIRS) $(OUTPUT_DIR_A)
!endif
!if "$(OUTPUT_DIR_A_DBG)" != "$(OUTPUT_DIR_DBG)"
OUTPUT_DIRS = $(OUTPUT_DIRS) $(OUTPUT_DIR_A_DBG)
!endif
!if "$(OUTPUT_DIR_A_STLDBG)" != "$(OUTPUT_DIR_STLDBG)"
OUTPUT_DIRS = $(OUTPUT_DIRS) $(OUTPUT_DIR_A_STLDBG)
!endif


# shortcut: all install lib dirs are the same
INSTALL_LIB_DIRS = $(INSTALL_LIB_DIR)

# shortcut: all install bin dirs are the same
INSTALL_BIN_DIRS = $(INSTALL_BIN_DIR)

INSTALL_DIRS = $(INSTALL_LIB_DIRS) $(INSTALL_BIN_DIRS)

# replace slashes with backslashes; mkdir and copy don't like them.
INSTALL_DIRS = $(INSTALL_DIRS:/=\)


dirs:	$(OUTPUT_DIRS) $(INSTALL_DIRS)

output-dirs: $(OUTPUT_DIRS)
install-dirs: $(INSTALL_DIRS)

install-lib-dirs:	$(INSTALL_LIB_DIRS)
install-bin-dirs:	$(INSTALL_BIN_DIRS)

$(OUTPUT_DIRS):
	@if not exist $@ mkdir $@

$(INSTALL_DIRS):
	@if not exist $@ mkdir $@
