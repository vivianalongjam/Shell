run:
	gcc ./External/cat.c -o ./Out/mycat
	gcc ./External/date.c -o ./Out/mydate
	gcc ./External/ls.c -o ./Out/myls
	gcc ./External/mkdir.c -o ./Out/mymkdir
	gcc ./External/rm.c -o ./Out/myrm
	gcc shell.c && ./a.out