base:
	clang++ -fsanitize=undefined -fsanitize=bounds --std=c++20 main.cpp md5.cpp -o build/main.exe
	./build/main.exe