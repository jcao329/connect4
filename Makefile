# NOTE:
#   - HWROOT and DESTDIR are passed in by the parent Makefile

CFLAGS=-std=c11 -pedantic -Wall -ggdb #-pg
OPT_CFLAGS=$(CFLAGS) -O3 -g # -pg
LIBS=-lcunit

SUPPORTFILES= board.h board.c \
	      computer_agent.h computer_agent.c \
	      player_agent.h player_agent.c agent.h  \
	      random_agent.h random_agent.c

TESTFILES=agent_test.h agent_test.c $(SUPPORTFILES)

GAMEFILES=game.c $(SUPPORTFILES)
INSTALLDIR=$(DESTDIR)/connect4
TESTS=board_test.c random_test.c computer_test.c
INSTALLDATA=connect-4.data.bz2
TESTDATA=connect-4.data
FILTER_CODE=$(HWROOT)/filter_code.py

.PRECIOUS: %.data

.PHONY: all
all: game 

test_board: board_test
	./board_test

test_random: random_test
	./random_test

test_computer: computer_test
	./computer_test

# Use this target to run all the unit tests
.PHONY: test
test: $(patsubst %.c,%,$(TESTS))
	for t in $^; do ./$$t ; done

%.data: %.data.bz2
	bunzip2 < $< >$@

%_test: %_test.c $(TESTFILES) $(TESTDATA)
	gcc -o $@ $(OPT_CFLAGS)  $(filter %.c,$^) $(LIBS)

game: $(GAMEFILES)
	gcc -o $@ $(CFLAGS) $(filter %.c,$^) $(LIBS)

opt_game: $(GAMEFILES)
	gcc -o $@ $(OPT_CFLAGS) $(filter %.c,$^) $(LIBS)


.PHONY: clean
clean:
	-rm $(patsubst %.c,%,$(TESTS)) game opt_game dump_final


###############################################################
# The rest of this file is for internal use; please ignore
##############################################################
