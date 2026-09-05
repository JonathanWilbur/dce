DOCOUT := docout

RELEASE_NOTES_PS := $(sort $(wildcard RELEASE_NOTES/*.ps))
PORT_GUIDE_PS := $(sort $(wildcard doc/dce1.1-port-guide/*.ps))
ADMIN_GD_CORE_PS := $(sort $(wildcard doc/src/built_books/admin_gd_core/*.ps))
ADMIN_GD_INTRO_PS := $(sort $(wildcard doc/src/built_books/admin_gd_intro/*.ps))
APP_GD_CORE_PS := $(sort $(wildcard doc/src/built_books/app_gd_core/*.ps))
APP_GD_DIR_PS := $(sort $(wildcard doc/src/built_books/app_gd_dir/*.ps))
APP_GD_STYLE_PS := $(sort $(wildcard doc/src/built_books/app_gd_style/*.ps))
APP_REF_PS := $(sort $(wildcard doc/src/built_books/app_ref/*.ps))
COMMAND_REF_PS := $(sort $(wildcard doc/src/built_books/command_ref/*.ps))
DFA_ADMIN_GD_REF_PS := $(sort $(wildcard doc/src/built_books/dfa_admin_gdref/*.ps))
DFA_USERS_GD_PS := $(sort $(wildcard doc/src/built_books/dfa_users_gd/*.ps))
DFS_ADMIN_GDREF_PS := $(sort $(wildcard doc/src/built_books/dfs_admin_gdref/*.ps))
GDS_ADMIN_GDREF_PS := $(sort $(wildcard doc/src/built_books/gds_admin_gdref/*.ps))
INTRO_PS := $(sort $(wildcard doc/src/built_books/intro/*.ps))
PROB_GD_PS := $(sort $(wildcard doc/src/built_books/prob_gd/*.ps))

AES_DFS_PS = doc/aes/dfs/front.ps doc/aes/dfs/text.ps doc/aes/dfs/index.ps
AES_DFS_PDF = $(AES_DFS_PS:.ps=.pdf)
AES_DIRECTORY_PS = doc/aes/directory/front.ps doc/aes/directory/text.ps doc/aes/directory/index.ps
AES_DIRECTORY_PDF = $(AES_DIRECTORY_PS:.ps=.pdf)
AES_RPC_PS = doc/aes/rpc/front.ps doc/aes/rpc/text.ps doc/aes/rpc/index.ps
AES_RPC_PDF = $(AES_RPC_PS:.ps=.pdf)
AES_SECURITY_PS = doc/aes/security/front.ps doc/aes/security/text.ps doc/aes/security/index.ps
AES_SECURITY_PDF = $(AES_SECURITY_PS:.ps=.pdf)
AES_THREADS_PS = doc/aes/threads/front.ps doc/aes/threads/text.ps doc/aes/threads/index.ps
AES_THREADS_PDF = $(AES_THREADS_PS:.ps=.pdf)
AES_TIME_PS = doc/aes/time/front.ps doc/aes/time/text.ps doc/aes/time/index.ps
AES_TIME_PDF = $(AES_TIME_PS:.ps=.pdf)

PDF_FILES = \
	$(DOCOUT)/AES_DFS.pdf \
	$(DOCOUT)/AES_DIRECTORY.pdf \
	$(DOCOUT)/AES_RPC.pdf \
	$(DOCOUT)/AES_SECURITY.pdf \
	$(DOCOUT)/AES_TIME.pdf \
	$(DOCOUT)/AES_THREADS.pdf \
    $(DOCOUT)/RELEASE_NOTES.pdf \
	$(DOCOUT)/PORTING_GUIDE.pdf \
	$(DOCOUT)/ADMIN_GD_CORE.pdf \
	$(DOCOUT)/ADMIN_GD_INTRO.pdf \
	$(DOCOUT)/APP_GD_CORE.pdf \
	$(DOCOUT)/APP_GD_DIR.pdf \
	$(DOCOUT)/APP_GD_STYLE.pdf \
	$(DOCOUT)/APP_REF.pdf \
	$(DOCOUT)/COMMAND_REF.pdf \
	$(DOCOUT)/DFA_ADMIN_GD_REF.pdf \
	$(DOCOUT)/DFA_USERS_GD.pdf \
	$(DOCOUT)/DFS_ADMIN_GDREF.pdf \
	$(DOCOUT)/GDS_ADMIN_GDREF.pdf \
	$(DOCOUT)/INTRO.pdf \
	$(DOCOUT)/PROB_GD.pdf \
	$(DOCOUT)/SAG.pdf \
	$(DOCOUT)/USERS_GUIDE.pdf \
	$(DOCOUT)/DFS_MH_SERVER_TEST_PLAN.pdf \
	$(DOCOUT)/DFS_SEC_ENH_TEST_PLAN.pdf

.PHONY: pdf aespdf linux linux-test linux-clean
pdf: $(PDF_FILES)

$(DOCOUT):
	mkdir -p $(DOCOUT)

%.pdf: %.ps
	gs -sDEVICE=pdfwrite -o $@ $<

$(DOCOUT)/RELEASE_NOTES.pdf: $(RELEASE_NOTES_PS) $(DOCOUT)
	gs -sDEVICE=pdfwrite -o $@ $(RELEASE_NOTES_PS)

$(DOCOUT)/PORTING_GUIDE.pdf: $(PORT_GUIDE_PS) $(DOCOUT)
	gs -sDEVICE=pdfwrite -o $@ $(PORT_GUIDE_PS)

$(DOCOUT)/ADMIN_GD_CORE.pdf: $(ADMIN_GD_CORE_PS) $(DOCOUT)
	gs -sDEVICE=pdfwrite -o $@ $(ADMIN_GD_CORE_PS)

$(DOCOUT)/ADMIN_GD_INTRO.pdf: $(ADMIN_GD_INTRO_PS) $(DOCOUT)
	gs -sDEVICE=pdfwrite -o $@ $(ADMIN_GD_INTRO_PS)

$(DOCOUT)/APP_GD_CORE.pdf: $(APP_GD_CORE_PS) $(DOCOUT)
	gs -sDEVICE=pdfwrite -o $@ $(APP_GD_CORE_PS)

$(DOCOUT)/APP_GD_DIR.pdf: $(APP_GD_DIR_PS) $(DOCOUT)
	gs -sDEVICE=pdfwrite -o $@ $(APP_GD_DIR_PS)

$(DOCOUT)/APP_GD_STYLE.pdf: $(APP_GD_STYLE_PS) $(DOCOUT)
	gs -sDEVICE=pdfwrite -o $@ $(APP_GD_STYLE_PS)

$(DOCOUT)/APP_REF.pdf: $(APP_REF_PS) $(DOCOUT)
	gs -sDEVICE=pdfwrite -o $@ $(APP_REF_PS)

$(DOCOUT)/COMMAND_REF.pdf: $(COMMAND_REF_PS) $(DOCOUT)
	gs -sDEVICE=pdfwrite -o $@ $(COMMAND_REF_PS)

$(DOCOUT)/DFA_ADMIN_GD_REF.pdf: $(DFA_ADMIN_GD_REF_PS) $(DOCOUT)
	gs -sDEVICE=pdfwrite -o $@ $(DFA_ADMIN_GD_REF_PS)

$(DOCOUT)/DFA_USERS_GD.pdf: $(DFA_USERS_GD_PS) $(DOCOUT)
	gs -sDEVICE=pdfwrite -o $@ $(DFA_USERS_GD_PS)

$(DOCOUT)/DFS_ADMIN_GDREF.pdf: $(DFS_ADMIN_GDREF_PS) $(DOCOUT)
	gs -sDEVICE=pdfwrite -o $@ $(DFS_ADMIN_GDREF_PS)

$(DOCOUT)/GDS_ADMIN_GDREF.pdf: $(GDS_ADMIN_GDREF_PS) $(DOCOUT)
	gs -sDEVICE=pdfwrite -o $@ $(GDS_ADMIN_GDREF_PS)

$(DOCOUT)/INTRO.pdf: $(INTRO_PS) $(DOCOUT)
	gs -sDEVICE=pdfwrite -o $@ $(INTRO_PS)

$(DOCOUT)/PROB_GD.pdf: $(PROB_GD_PS) $(DOCOUT)
	gs -sDEVICE=pdfwrite -o $@ $(PROB_GD_PS)

# AES stands for "Application Environment Specification" in this context
# For some reason, we have to concatenate the PDFs.
# Processing the PS files in order seems to interfere with each other.
$(DOCOUT)/AES_DFS.pdf: $(AES_DFS_PDF) $(DOCOUT)
	gs -sDEVICE=pdfwrite -o $@ $(AES_DFS_PDF)

$(DOCOUT)/AES_DIRECTORY.pdf: $(AES_DIRECTORY_PDF) $(DOCOUT)
	gs -sDEVICE=pdfwrite -o $@ $(AES_DIRECTORY_PDF)

$(DOCOUT)/AES_RPC.pdf: $(AES_RPC_PDF) $(DOCOUT)
	gs -sDEVICE=pdfwrite -o $@ $(AES_RPC_PDF)

$(DOCOUT)/AES_SECURITY.pdf: $(AES_SECURITY_PDF) $(DOCOUT)
	gs -sDEVICE=pdfwrite -o $@ $(AES_SECURITY_PDF)

$(DOCOUT)/AES_TIME.pdf: $(AES_TIME_PDF) $(DOCOUT)
	gs -sDEVICE=pdfwrite -o $@ $(AES_TIME_PDF)

$(DOCOUT)/AES_THREADS.pdf: $(AES_THREADS_PDF) $(DOCOUT)
	gs -sDEVICE=pdfwrite -o $@ $(AES_THREADS_PDF)

$(DOCOUT)/SAG.pdf: ode/ode/doc/sag.ps $(DOCOUT)
	gs -sDEVICE=pdfwrite -o $@ ode/ode/doc/sag.ps

$(DOCOUT)/USERS_GUIDE.pdf: ode/ode/doc/UsersGuide.ps ode/ode/doc/UsersGuide.appendixA.ps $(DOCOUT)
	gs -sDEVICE=pdfwrite -o $@ ode/ode/doc/UsersGuide.ps ode/ode/doc/UsersGuide.appendixA.ps

$(DOCOUT)/DFS_MH_SERVER_TEST_PLAN.pdf: project/test.plans/dfs_mh_server.testplan.ps $(DOCOUT)
	gs -sDEVICE=pdfwrite -o $@ project/test.plans/dfs_mh_server.testplan.ps

$(DOCOUT)/DFS_SEC_ENH_TEST_PLAN.pdf: project/test.plans/dfs_sec_enh.testplan.ps $(DOCOUT)
	gs -sDEVICE=pdfwrite -o $@ project/test.plans/dfs_sec_enh.testplan.ps

cleandoc:
	rm -rf $(DOCOUT)
	rm $(AES_DFS_PDF)
	rm $(AES_DIRECTORY_PDF)
	rm $(AES_RPC_PDF)
	rm $(AES_SECURITY_PDF)
	rm $(AES_TIME_PDF)
	rm $(AES_THREADS_PDF)

# Linux userspace port (see linux/README.md). ODE does not run on GNU/Linux;
# this GNU Make target builds the ported threads, UUID, and tests.
linux:
	$(MAKE) -C linux

linux-test:
	$(MAKE) -C linux test

linux-clean:
	$(MAKE) -C linux clean
