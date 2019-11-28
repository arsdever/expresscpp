LIBRARIES = logger expresscpp
LIB_PATH = ${ROOT}/libs
LIBRARY_FILES = $(LIBRARIES:%=$(LIB_PATH)/lib%.so)
INTERFACES = ${ROOT}/interfaces
CEXECUTABLE = ${EXEC_ROOT}/client
SEXECUTABLE = ${EXEC_ROOT}/server

.PHONY: test build_targets client server logger expresscpp clean

test: $(SEXECUTABLE) $(CEXECUTABLE) ${EXEC_ROOT}/exec.sh
	@echo Running the test
	${EXEC_ROOT}/exec.sh

${EXEC_ROOT}/exec.sh: ${EXEC_ROOT}
	@echo Generating the executer script
	@touch $@
	@echo "#!/bin/sh" >> $@
	@echo "cd ${EXEC_ROOT}" >> $@
	@echo "LD_LIBRARY_PATH=$(LIB_PATH) ./server &" >> $@
	@echo "LD_LIBRARY_PATH=$(LIB_PATH) ./client; echo \$$?" >> $@
	@chmod +x $@

clean:
	rm -fr ${ROOT}/tmp; rm -fr ${ROOT}/libs

$(SEXECUTABLE) $(CEXECUTABLE): $(INTERFACES) $(LIBRARY_FILES) ${EXEC_ROOT}
	cd ${ROOT}/expresscpp/test/$(notdir $@); qmake; make -j8


$(LIBRARY_FILES): LIB = $(patsubst lib%.so,%,$(notdir $@))
$(LIBRARY_FILES): $(LIB_PATH) ${ROOT}/$(LIB)
	cd ${ROOT}/$(LIB); qmake; make -j8

${EXEC_ROOT} $(LIB_PATH):
	@echo BUILDING $@
	mkdir -p $@

${ROOT}/expresscpp : ${ROOT}/logger
${ROOT}/logger ${ROOT}/expresscpp: $(INTERFACES)
	@echo BUILDING $@
	git clone https://github.com/arsdever/$(notdir @) $@

$(INTERFACES):
	@echo BUILDING $@
	git clone https://github.com/opensalad/$(notdir @) $@