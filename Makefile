s ?= serve.cpp
e ?= f/2
CXXFLAGS := -std=c++17 -pthread -Wall -g  # -fsanitize=thread
main: 	$(s)
		g++ $(s) $(CXXFLAGS) -o $(e)