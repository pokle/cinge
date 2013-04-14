ldifsplit: ldifsplit.c
	clang -std=c11 -O4 ldifsplit.c -o ldifsplit
	#clang -std=c11 -g ldifsplit.c -o ldifsplit

clean:
	rm -rf ldifsplit ldifsplit.dSYM