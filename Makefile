s ?= serve.cpp
e ?= front/2
CXXFLAGS := -std=c++20 -pthread -Wall -g  -fsanitize=thread
main: 	$(s)
		g++ $(s) $(CXXFLAGS) -o $(e)