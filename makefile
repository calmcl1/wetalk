CWD=$(shell pwd)

INCLUDE_DIR=$(CWD)/include
LIB_DIR=$(CWD)/lib
BUILD_DIR=$(CWD)/build

all:
	mkdir $(BUILD_DIR) -p
	g++ *.cpp -I"$(CWD)" -L"$(SO_DIR)" -o "$(BUILD_DIR)/wetalk" \
	$(shell pkg-config --cflags --libs gstreamer-1.0)
	
clean:
	rm -rf "$(BUILD_DIR)"