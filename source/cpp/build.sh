if [ "$1" = "clean" ]; then
	rm testappp
	cd testapp
	make clean
else
	cd testapp
	make
	cd ../ 
	ln -s ./testapp/testapp testappp
fi
