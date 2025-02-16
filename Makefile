# Variables
CFLAGS = gcc -Wall -Wextra -Werror -std=c11
INFO = report.info
HTML = $(REPORT)/index.html
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
EXCLUDE =  --exclude '$(TESTS)/*'
UNAME = $(shell uname -s)

ifeq ($(UNAME), Linux)
    LIB = -lcheck -lsubunit -lm
else
    LIB = -lcheck
endif

# Folders
TESTS = test
REPORT = report
GCOV = gcov
OBJ = obj
DEST_DIR = s21_viewer
QMAKE = qmake
MAKE = make

# Folders
SRC_DIR = 3DViewer
BUILD_DIR = 3DViewer/s21_viewer
TEST_DIR = 3DViewer/test/

# Executable files
S21_TEST = $(DEST_DIR)/s21_test
S21_GCOV = $(DEST_DIR)/s21_gcov

# Source and objective library files
SRC_NOTATION = $(SRC_DIR)/*.c
OBJ_NOTATION = $(OBJ)/*.o
LIB_NOTATION = $(DEST_DIR)/s21_lib.a

# Executable files
EXECUTABLE = 3DViewer
TEST_EXECUTABLE = s21_test

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
TEST_FILES = $(wildcard $(TEST_DIR)/*.c)

INST_DIR=$(DEST_DIR)
DIST_DIR=$(DEST_DIR)-1.0

# Targets
all :  tests gcov_report install

$(LIB_NOTATION) :
	mkdir -p $(DEST_DIR)
	mkdir -p $(OBJ)
	$(CFLAGS) -c $(SRC_NOTATION)
	ar rcs $(LIB_NOTATION) *.o
	ranlib $(LIB_NOTATION)
	mv *.o $(OBJ)

tests : $(LIB_NOTATION)
	$(CFLAGS) $(SRC_NOTATION) $(TEST_FILES) -o $(S21_TEST) $(LIB)
	./$(S21_TEST)

gcov_report :
	$(CFLAGS) $(SRC_NOTATION) $(TEST_FILES) -o $(S21_GCOV) $(LIB) $(GCOV_FLAGS)
	./$(S21_GCOV)
	lcov -c -d . -t "$(REPORT)" -o $(INFO) $(EXCLUDE)
	genhtml -o $(REPORT) $(INFO)

	mkdir -p $(GCOV)

	open $(REPORT)/index.html

open :
	open $(HTML)

style :
	cp ../materials/linters/.clang-format .
	clang-format -n $(SRC_DIR)/*.c $(SRC_DIR)/*.h $(SRC_DIR)/$(TESTS)/*.c

uninstall:
	rm -r $(BUILD_DIR)

install:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && $(QMAKE) ../3DViewer.pro && $(MAKE)
	open ./3DViewer/s21_viewer

	

dist:
	mkdir -p $(DIST_DIR)
	cp -R $(SRC_DIR) $(DIST_DIR)
	tar --totals -cvf $(DIST_DIR).tar $(DIST_DIR)
	rm -rf $(DIST_DIR)

dvi:
	open ./manual.md

leaks :
	CK_FORK=no leaks -atExit -- ./$(S21_TEST)

valgrind :
	CK_FORK=no valgrind  --track-origins=yes --tool=memcheck --leak-check=yes --show-leak-kinds=all ./$(S21_TEST)

clean :
	rm -rf $(S21_TEST) $(S21_GCOV) $(REPORT) $(OBJ) $(GCOV) *.{a,o,gc*,out,dSYM} .clang-format $(DIST_DIR).tar $(BUILD_DIR)
	rm -rf  *.tar manual
	rm -rf s21_viewer *.info .clang-format
rebuild :
	make clean
	make install

cppcheck:
	cppcheck --std=c11 --enable=all --suppress=missingIncludeSystem  --suppress=unusedFunction --suppress=invalidPointerCast ./

.PHONY : all clean rebuild $(LIB_NOTATION) test dvi dist gcov_report open style leaks valgrind
