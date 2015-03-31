export LD_LIBRARY_PATH=./build:/usr/local/lib

MODE=debug
SCONSOPTS="-j2"
VALGRIND=$(shell echo 'valgrind -v --track-origins=yes --leak-check=full --show-reachable=yes --trace-children=yes --child-silent-after-fork=yes')

scons:
	@scons MODE=$(MODE) $(SCONSOPTS)

release:
	@scons MODE=release $(SCONSOPTS)

demo:
	@scons MODE=$(MODE) $(SCONSOPTS) demo

install:
	@scons $(SCONSOPTS) install

test:
	@ulimit -n 2048
	@scons MODE=$(MODE) $(SCONSOPTS) test
	@./build/unit_tests/test_reactive --log_level=error --detect_memory_leaks=1 --report_level=detailed --report_format=xml 2> xunit.xml
	@mkdir -p ./build/valgrind
	@$(VALGRIND) ./build/unit_tests/test_reactive 2> ./build/valgrind/test_reactive.log

massif: demo
	@valgrind --tool=massif --time-unit=B ./build/demo/demo_web_application

cov:
	@ulimit -n 2048
	@scons $(SCONSOPTS) cov
	@./build/unit_tests/test_reactive
	@gcovr -d -r .

demo_web_application: demo
	@./build/demo/demo_web_application

demo_web_application_class: demo
	@./build/demo/demo_web_application_class

demo_http_client: demo
	@./build/demo/demo_http_client

valgrind: demo
	@mkdir -p ./build/valgrind
	@$(VALGRIND) ./build/demo/demo_http_client 2> ./build/valgrind/demo_http_client.log

clean:
	@scons -c
	@find . -type f -name "*.o" -exec rm -f {} \;
	@find . -type f -name "*.os" -exec rm -f {} \;
	@find . -type f -name "*.gcov" -exec rm -f {} \;
	@find . -type f -name "*.gcno" -exec rm -f {} \;
	@find . -type f -name "*.gcda" -exec rm -f {} \;

.PHONY: test
