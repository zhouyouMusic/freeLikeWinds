all:
	gcc -g -fno-stack-protector src/*.c  python_module/tcPython_pc.c  src/*.so  -I include/ -L curl/lib/ -lcurl  -L lib/ -lvoice_pc -L lib/ -lvwkasemantic -lasound  -lm -lpthread -lmad_pc -L lib/ -lspeex_pc -o json  -I/usr/include/python2.7/ -L/usr/lib/ -lpython2.7  `pkg-config --cflags --libs gtk+-2.0`



clean:
	rm json
