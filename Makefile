CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

VulkanTest: *.cpp *.hpp
	g++ $(CFLAGS) -o vulkantest *.cpp $(LDFLAGS)

.PHONY: test clean

test: vulkantest
	./vulkantest

clean:
	rm -f vulkantest