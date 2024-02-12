s := main.cpp serve.cpp epoll/epoll.cpp epoll/clock.cpp http/http.cpp http/httpparse.cpp http/httpresponse.cpp log/loger.cpp
e ?= f/demo
CXXFLAGS := -std=c++17 -pthread -Wall -g  # -fsanitize=thread
main: 	$(s)
		g++ $(s) $(CXXFLAGS) -o $(e)