if [ "$1" = "clean" ]; then
	rm testappp
	cd testapp
	make clean
	make
	cd ../ 
	ln -s ./testapp/testapp testappp
	if [ "$2" = "debug" ]; then
	gdb ./testappp
	else
	./testappp
	fi
else
	cd testapp
	make
	cd ../ 
	ln -s ./testapp/testapp testappp
	if [ "$1" = "debug" ]; then
	gdb ./testappp
	else
	./testappp
	fi
fi
