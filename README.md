# Paste

executing this simulates low-level paste


function aa(){
./compile.sh ./src/send.c ./test/send.o;
./test/send.o "aaa" "bbb" "333"
}; aa
./bin/test.o "aaa" "bbb" "333"
./test/test.o "aaa" "bbb" "333"
./test/main.o "aaa" "bbb" "333"
./test/send.o "aaa" "bbb" "333"