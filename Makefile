all:	
	gcc level1.c -o level1
	gcc level2.c -o level2
	
clean:
	rm -f level1
	rm -f level2
	
