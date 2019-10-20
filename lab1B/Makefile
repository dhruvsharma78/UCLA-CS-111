# NAME: Dhruv Sharma
# EMAIL: dhruvsharma78@ucla.edu
# ID: 605082988

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

SDIR = ./

ODIR = $(SDIR)/obj
_OBJ = file.o command.o commands.o optionparsing.o simpsh.o signal.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

TARGET = simpsh

UID = 605082988

_DIST = lab1-$(UID).tar.gz
_DISTSRC = $(SDIR)/*.c $(SDIR)/*.h Makefile README

.PHONY: clean dist

default: $(TARGET)

debug: CFLAGS +=  -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -fsanitize=bounds
debug: default

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS) | ODIRCREATE
	$(CC) $(CFLAGS) -c -o $@ $<

ODIRCREATE:
	@mkdir -p $(ODIR)

clean:
	rm -rf $(ODIR)/ $(SDIR)/*~ $(TARGET) *~ $(_DIST)

dist:
	tar -czvf $(_DIST) $(_DISTSRC)

check: $(TARGET)
	@printf "Testing simpsh (Lab 1A)\n"
	@printf "Testing bogus options..."
	@./$(TARGET) --bogus >testout 2>testerr; \
	if [ $$? == 1 ] && [ -s testerr ] && [ ! -s testout ]; then \
		printf "\t\tSUCCESS\n"; \
	else \
		printf "\t\tFAIL\n"; \
		rm -rf testout testerr; \
		exit 1; \
	fi;
	@printf "Testing file opening options..."
	@touch testf_1
	@touch testf_2
	@touch testf_3
	@chmod u=r testf_1
	@chmod u=w testf_2
	@chmod u=rw testf_3
	@sleep 0.2
	@./$(TARGET) --rdonly testf_1 --wronly testf_2 --wronly testf_3 \
	--rdonly testf_1 --wronly testf_2 1>/dev/null 2>&1; \
	sleep 1; \
	if [ $$? == 0 ]; then \
		printf "\t\tSUCCESS\n"; \
	else \
		printf "\t\tFAIL\n"; \
		chmod u+a testf_1 testf_2 testf_3; \
		rm -rf testf_1 testf_2 testf_3; \
		exit 1; \
	fi;
	@printf "Testing file opening errors..."
	@chmod u-r testf_1
	@chmod u-w testf_2
	@touch t1out t1err t2out t2err
	@sleep 0.3
	@./$(TARGET) --rdonly testf_1 1>t1out 2>t1err; \
	RESONE=$$?; \
	./$(TARGET) --wronly testf_2 1>t2out 2>t2err; \
	RESTWO=$$?; \
	sleep 1; \
	if [ $$RESONE == 1 ] && [ ! -s t1out ] && [ -s t1err ] \
	&& [ $$RESTWO == 1 ] && [ ! -s t2out ] && [ -s t2err ]; then \
		printf "\t\tSUCCESS\n"; \
	else \
		printf "\t\tFAIL\n"; \
		chmod u+rw testf_1 testf_2 testf_3; \
		rm -rf testf_1 testf_2 testf_3 testout testerr; \
		rm -rf t1out t1err t2out t2err; \
		exit 1; \
	fi;
	@printf "Testing command functionality..."
	@chmod u=rw testf_1
	@chmod u=w testf_2
	@echo "test stringhiodr s oisf iosijf sijrf" > testf_1
	@chmod u=r testf_1
	@sleep 0.3
	@./$(TARGET) --rdonly testf_1 --wronly testf_2 --command 0 1 1 cat; \
	EC=$$?; \
	chmod u=r testf_2; \
	cmp -s testf_1 testf_2; \
	sleep 1; \
	if [ $$? == 0 ] && [ $$EC == 0 ]; then \
		printf "\tSUCCESS\n"; \
	else \
		rm -rf testf_1 testf_2 testf_3 testout testerr; \
		rm -rf t1out t1err t2out t2err; \
		printf "\tFAIL\n"; \
		exit 1; \
	fi;
	@printf "Testing pipe functionality..."
	@touch test8out.txt
	@touch test8err.txt
	@echo "Random test string 02938y4 2hfw fji f9q0u3e fohaelf h792y hf" > a5.txt
	@cp a5.txt a0.txt
	@./$(TARGET) --rdonly a5.txt --wronly test8err.txt --pipe --wronly test8out.txt \
	--command 0 3 1 cat --command 2 4 1 cat >c8out.txt 2>&1 ; \
	RC=$$? ; \
	sleep 1 ; \
	if [ $$RC == 0 ] && cmp -s a0.txt test8out.txt && [ ! -s test8err.txt ] && [ ! -s c8out.txt ]; then \
		printf "\t\tSUCCESS\n"; \
	else \
		rm -rf test8out.txt test8err.txt c8out.txt a0.txt a5.txt; \
		rm -rf testf_1 testf_2 testf_3 testout testerr; \
		rm -rf t1out t1err t2out t2err; \
		printf "\t\tFAIL\n"; \
		exit 1; \
	fi;
	@printf "Testing catch, default, abort...\n"
	@./$(TARGET) --catch 11 --default 11 --abort >c12out.txt 2>&1 ; \
	if [ $$? == 139 ] && [ ! -s c12out.txt ]; then \
		printf "\t\t\t\t\tSUCCESS\n"; \
	else \
		rm -rf test8out.txt test8err.txt c8out.txt a0.txt a5.txt; \
		rm -rf testf_1 testf_2 testf_3 testout testerr; \
		rm -rf t1out t1err t2out t2err c12out.txt; \
		printf "\t\t\t\t\tFAIL\n"; \
		exit 1; \
	fi;
	@printf "Testing wait funtionality..."
	@cat a0.txt | wc -c > test17outstd.txt
	@cp a5.txt a6.txt 
	@./$(TARGET) --rdonly a6.txt --creat --wronly test17out.txt --creat --rdwr test17err.txt \
	--pipe --command 0 4 2 cat --command 3 1 2 wc -c --close 3 --close 4 \
	--wait >c17out.txt 2>c17err.txt ; \
	RC=$$?; \
	sleep 2; \
	if [ $$RC == 0 ] && [ -e test17out.txt ] && [ -e test17err.txt ] \
	  && cmp -s test17outstd.txt test17out.txt && [ ! -s test17err.txt ] \
	  && [ ! -s c17err.txt ] && grep -q "exit 0 cat" c17out.txt \
	  && grep -q "exit 0 wc -c" c17out.txt && wc -l < c17out.txt | grep -q "2"; then \
		printf "\t\tSUCCESS\n"; \
	else \
		rm -rf test8out.txt test8err.txt c8out.txt a0.txt a5.txt; \
		rm -rf testf_1 testf_2 testf_3 testout testerr; \
		rm -rf t1out t1err t2out t2err c12out.txt; \
		rm -rf a6.txt test17outstd.txttest17 out.txt test17err.txt c17out.txt c17err.txt; \
		printf "\t\tFAIL\n"; \
		exit 1; \
	fi;
	@printf "\nAll tests successful\n"
	@rm -rf test8out.txt test8err.txt c8out.txt a0.txt a5.txt
	@rm -rf testf_1 testf_2 testf_3 testout testerr
	@rm -rf t1out t1err t2out t2err c12out.txt
	@rm -rf a6.txt test17outstd.txt test17out.txt test17err.txt c17out.txt c17err.txt
