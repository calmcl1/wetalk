CWD=$(shell pwd)

INCLUDE_DIR=$(CWD)/include
LIB_DIR=$(CWD)/lib
BUILD_DIR=$(CWD)/build

all: 

	mkdir $(BUILD_DIR)/tests -p
	
	g++ tests/jack_test.cpp $(INCLUDE_DIR)/jack_delay/mtdm.o \
	$(shell pkg-config --cflags --libs gstreamer-1.0 jack) \
	-I$(INCLUDE_DIR)/jack_delay \
	-o $(BUILD_DIR)/tests/jack_test
	
wetalk:
	mkdir $(BUILD_DIR) -p
	g++ *.cpp -I"$(CWD)" -L"$(SO_DIR)" \
	$(shell pkg-config --cflags --libs gstreamer-1.0) \
	-o "$(BUILD_DIR)/wetalk"
	

clean:
	rm -rf "$(BUILD_DIR)"