s ?= serve.cpp
e ?= 2
CXXFLAGS := -std=c++17 -pthread -Wall -g  -fsanitize=thread
main: 	$(s)
		g++ $(s) $(CXXFLAGS) -o$(e)