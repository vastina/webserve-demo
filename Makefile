s ?= serve.cpp
e ?= f/demo
CXXFLAGS := -std=c++17 -pthread -Wall -g  # -fsanitize=thread
main: 	$(s)
		g++ $(s) $(CXXFLAGS) -o $(e)