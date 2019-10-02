
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

SDIR = ./

ODIR = $(SDIR)/obj
_OBJ = lab0.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

TARGET = lab0

_DIST = lab0-605082988.tar.gz
_DISTSRC = $(SDIR)/*.c Makefile README backtrace.png

.PHONY: clean dist

default: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS) | ODIRCREATE
	$(CC) $(CFLAGS) -c -o $@ $<

ODIRCREATE:
	@mkdir -p $(ODIR)

clean:
	rm -rf $(ODIR)/ $(SDIR)/*~ $(TARGET) *~ $(_DIST)

dist: clean
	tar -czvf $(_DIST) $(_DISTSRC)

check: $(TARGET)
	@printf "\nTesting lab0\n\n"
	@printf "Testing basic functionality... "
	@cat /dev/urandom | tr -dc "\t\n [:alnum:]" | head -c10000 > test_file
	@cat test_file | ./$(TARGET) > out
	@if diff -u out test_file > /dev/null; then \
		printf "\t\t\tSUCCESS: Output file matches input file\n"; \
	else \
		printf "\t\t\tFAIL: Output file NOT the same as input\n"; \
	fi;
	@printf "Testing support for input file argument... "
	@ ./$(TARGET) --input=test_file 1>/dev/null 2>/dev/null; \
	if [[ $$? -eq 0 ]]; then \
		printf "\tSUCCESS\n"; \
	else \
		printf "\tFAIL: Input argument failed\n"; \
	fi;
	@printf "Testing support for output file argument... "
	@ ./$(TARGET) --output=newout < test_file 1>/dev/null 2>/dev/null; \
	if [[ $$? -eq 0 ]]; then \
		printf "\tSUCCESS\n"; \
	else \
		printf "\tFAIL: Input argument failed\n"; \
	fi;
	@rm -f newout
	@printf "Testing unrecognized argument handling... "
	@ ./$(TARGET) --unrecognized-option 1>/dev/null 2>/dev/null; \
	if [[ $$? -ne 1 ]]; then \
		printf "\tFAIL: Incorrect exit code\n"; \
	else \
		printf "\tSUCCESS: Correct exit code\n"; \
	fi;
	@printf "Testing input file read permission error... "
	@chmod -r test_file
	@ ./$(TARGET) --input=test_file 1>/dev/null 2>/dev/null; \
	if [[ $$? -ne 2 ]]; then \
		printf "\tFAIL: Incorrect exit code\n"; \
	else \
		printf "\tSUCCESS: Correct exit code\n"; \
	fi;
	@printf "Testing output file write permission error... "
	@chmod +r test_file
	@touch newout
	@chmod -w newout
	@ ./$(TARGET) --input=test_file --output=newout 1>/dev/null 2>/dev/null; \
	if [[ $$? -ne 3 ]]; then \
		printf "\tFAIL: Incorrect exit code\n"; \
	else \
		printf "\tSUCCESS: Correct exit code\n"; \
	fi;
	@printf "Testing segmentation fault catch..."
	@ ./$(TARGET) --catch --segfault 1>/dev/null 2>/dev/null; \
	if [[ $$? -ne 4 ]]; then \
		printf "\t\tFAIL: Incorrect exit code\n"; \
	else \
		printf "\t\tSUCCESS: Correct exit code\n"; \
	fi;
	@printf "Testing core dump..."
	-@ ./$(TARGET) --dump-core 1>/dev/null 2>/dev/null; \
	if [[ $$? -ne 139 ]]; then \
		printf "\t\tFAIL: Incorrect exit code\n"; \
	else \
		printf "\t\tSUCCESS: Correct exit code\n"; \
	fi;
	@printf "Testing output file creation... "
	@ rm -f newout
	@ ./$(TARGET) --input=test_file --output=newout 1>/dev/null 2>/dev/null; \
	if [[ $$? -ne 0 ]]; then \
		printf "\t\tFAIL: File creation failed\n"; \
	else \
		printf "\t\tSUCCESS: File creation succeeded\n"; \
	fi;
	@printf "Testing non existent input file... "
	@ ./$(TARGET) --input=test_file1827382738173 --output=newout 1>/dev/null 2>/dev/null; \
	if [[ $$? -ne 2 ]]; then \
		printf "\t\tFAIL: Incorrect exit code\n"; \
	else \
		printf "\t\tSUCCESS: Correct exit code\n"; \
	fi;
	@rm -f test_file out newout
