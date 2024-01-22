s ?= serve.cpp
e ?= 2
CXXFLAGS := -std=c++20 -pthread -Wall -g  -fsanitize=thread
main: 	$(s)
		g++ $(s) $(CXXFLAGS) -o$(e)

#  g++ test.cpp ../include/picohttpparser/picotest/picotest.c 
# ../include/picohttpparser/picohttpparser.c 