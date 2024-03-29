include .env

CFLAGS = -std=c++17 -O2 -I. -I$(TINYOBJ_PATH)
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

vulkantest: *.cpp *.hpp
	g++ $(CFLAGS) -o vulkantest *.cpp $(LDFLAGS)

.PHONY: test clean

test: vulkantest
	./vulkantest

clean:
	rm -f vulkantest