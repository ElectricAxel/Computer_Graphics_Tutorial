# GNU Make solution makefile autogenerated by Premake
# Type "make help" for usage help

ifndef config
  config=debug
endif
export config

PROJECTS := framework Tut\ 08\ Gimbal\ Lock Tut\ 08\ Quaternion\ YPR Tut\ 08\ Camera\ Relative Tut\ 08\ Interpolation

.PHONY: all clean help $(PROJECTS)

all: $(PROJECTS)

framework: 
	@echo "==== Building framework ($(config)) ===="
	@${MAKE} --no-print-directory -C ../framework -f Makefile

Tut\ 08\ Gimbal\ Lock: framework
	@echo "==== Building Tut 08 Gimbal Lock ($(config)) ===="
	@${MAKE} --no-print-directory -C . -f Tut\ 08\ Gimbal\ Lock.make

Tut\ 08\ Quaternion\ YPR: framework
	@echo "==== Building Tut 08 Quaternion YPR ($(config)) ===="
	@${MAKE} --no-print-directory -C . -f Tut\ 08\ Quaternion\ YPR.make

Tut\ 08\ Camera\ Relative: framework
	@echo "==== Building Tut 08 Camera Relative ($(config)) ===="
	@${MAKE} --no-print-directory -C . -f Tut\ 08\ Camera\ Relative.make

Tut\ 08\ Interpolation: framework
	@echo "==== Building Tut 08 Interpolation ($(config)) ===="
	@${MAKE} --no-print-directory -C . -f Tut\ 08\ Interpolation.make

clean:
	@${MAKE} --no-print-directory -C ../framework -f Makefile clean
	@${MAKE} --no-print-directory -C . -f Tut\ 08\ Gimbal\ Lock.make clean
	@${MAKE} --no-print-directory -C . -f Tut\ 08\ Quaternion\ YPR.make clean
	@${MAKE} --no-print-directory -C . -f Tut\ 08\ Camera\ Relative.make clean
	@${MAKE} --no-print-directory -C . -f Tut\ 08\ Interpolation.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "   debug"
	@echo "   release"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   framework"
	@echo "   Tut 08 Gimbal Lock"
	@echo "   Tut 08 Quaternion YPR"
	@echo "   Tut 08 Camera Relative"
	@echo "   Tut 08 Interpolation"
	@echo ""
	@echo "For more information, see http://industriousone.com/premake/quick-start"
