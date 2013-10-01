if [ "$1" = "clean" ]; then
	rm testapp
	cd ../../testapp
	make clean
else
	cd ../../testapp
	make
	cd ../source/cpp
	ln -s ../../testapp/testapp testapp
fi
