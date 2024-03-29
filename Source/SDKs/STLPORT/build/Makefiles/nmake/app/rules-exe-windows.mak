# -*- makefile -*- Time-stamp: <03/10/26 23:11:03 ptr>
# $Id: rules-exe-windows.mak,v 1.1.2.1 2004/12/24 11:27:14 ptr Exp $

release-shared:	$(OUTPUT_DIR) $(PRG)
release-static:	$(OUTPUT_DIR_A) $(PRG_A)

dbg-shared:	$(OUTPUT_DIR_DBG) $(PRG_DBG)
dbg-static:	$(OUTPUT_DIR_A_DBG) $(PRG_A_DBG)

stldbg-shared:	$(OUTPUT_DIR_STLDBG) $(PRG_STLDBG)
stldbg-static:	$(OUTPUT_DIR_A_STLDBG) $(PRG_A_STLDBG)

$(PRG):	$(OBJ) $(LIBSDEP)
	$(LINK_cc_REL) $(LINK_OUTPUT_OPTION) $(OBJ) $(LDLIBS_REL) $(LDLIBS)

$(PRG_A): $(OBJ_A) $(LIBSDEP)
	$(LINK_cc_A_REL) $(LINK_OUTPUT_OPTION) $(OBJ_A) $(LDLIBS_A_REL) $(LDLIBS)

$(PRG_DBG): $(OBJ_DBG) $(LIBSDEP)
	$(LINK_cc_DBG) $(LINK_OUTPUT_OPTION) $(OBJ_DBG) $(LDLIBS_DBG) $(LDLIBS)

$(PRG_A_DBG): $(OBJ_A_DBG) $(LIBSDEP)
	$(LINK_cc_A_DBG) $(LINK_OUTPUT_OPTION) $(OBJ_A_DBG) $(LDLIBS_A_DBG) $(LDLIBS)

$(PRG_STLDBG): $(OBJ_STLDBG) $(LIBSDEP)
	$(LINK_cc_STLDBG) $(LINK_OUTPUT_OPTION) $(OBJ_STLDBG) $(LDLIBS_STLDBG) $(LDLIBS)

$(PRG_A_STLDBG): $(OBJ_A_STLDBG) $(LIBSDEP)
	$(LINK_cc_A_STLDBG) $(LINK_OUTPUT_OPTION) $(OBJ_A_STLDBG) $(LDLIBS_A_STLDBG) $(LDLIBS)
