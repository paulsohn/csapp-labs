/*
 * rotateRight - Rotate x to the right by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateRight(0x87654321,4) = 0x18765432
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 25
 *   Rating: 3
 */
int rotateRight(int x, int n){
	int allset = ~0;
	int size_minus_n = 33 + ~n;
	int mask = ~(allset << size_minus_n);
	return ((x >> n) & mask) | (x << size_minus_n);
}

/*
 * rotateRight - Rotate x to the left by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateRight(0x87654321,4) = 0x76543218
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 25
 *   Rating: 3
 */
int rotateLeft(int x, int n){
	int allset = ~0;
	int size_minus_n = 33 + ~n;
	int mask = ~(allset << n);
	
	return (x << n) | ( (x >> size_minus_n) & mask );
}
