all:
	cd bash; bash init.sh
	gcc c/main.c -lmysqlclient -o build/main
