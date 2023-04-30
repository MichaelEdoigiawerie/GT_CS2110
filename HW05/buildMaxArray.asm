;;=============================================================
;; CS 2110 - Spring 2023
;; Homework 5 - buildMaxArray
;;=============================================================
;; Name: Michael Edoigiawerie
;;=============================================================

;;  Pseudocode (see PDF for explanation)
;;  Pseudocode values are based on the labels' default values.
;;
;;	int A[] = {-2, 2, 1};
;;	int B[] = {1, 0, 3};
;;	int C[3];
;;	int length = 3;
;;
;;	int i = 0;
;;	while (i < length) {
;;		if (A[i] >= B[length - i - 1]) {
;;			C[i] = 1;
;;		}
;;		else {
;;			C[i] = 0;
;;		}
;;		i++;
;;	}

.orig x3000
	;; YOUR CODE HERE
		LD R0, I; R0 = i
		LD R1, LENGTH; R1 = length
		NOT R2, R1; R2 = ~length
		ADD R2, R2, #1; R2 = -length

AGAIN	ADD R3, R0, R2; while (i < length)
		BRzp ENDWHILE

		NOT R6, R0; R6 = ~i
		ADD R6, R6, #1; R6 = -i

		LD R1, LENGTH; R1 = length
		ADD R1, R1, R6; R1 = length - i
		ADD R1, R1, #-1; R1 = length - i - 1
		
		LD R4, A; R4 = array A
		ADD R4, R4, R0; R4 = array A + i
		LDR R4, R4, #0; R4 = A[i]

		LD R5, B; R5 = array B
		ADD R5, R5, R1; R5 = array B + (length - i - 1)
		LDR R5, R5, #0; R5 = B[length - i - 1]

		LD R7, C; R7 = array C
		ADD R7, R7, R0; C + i

		NOT R5, R5; R5 = ~B[length - i - 1]
		ADD R5, R5, #1; R5 = -B[length - i - 1]

	IF	ADD R1, R4, R5; if (A[i] - B[length - i - 1] >= 0)
		BRn ELSE 
		ST R0, I; I = R0
		AND R0, R0, #0; R0 = 0
		ADD R0, R0, #1; R0 = 1
		STR R0, R7, #0; C[i] = 1;
		LD R0, I; R0 = I
		BR ENDIF

	ELSE 
	     ST R0, I; I = R0
		 AND R0, R0, #0; R0 = 0
		 STR R0, R7, #0; C[i] = 0;
		 LD R0, I; R0 = I
	
	ENDIF   
			ADD R0, R0, #1; i++
	BR 	AGAIN

I	.fill 0

ENDWHILE
			HALT

;; Do not change these addresses! 
;; We populate A and B and reserve space for C at these specific addresses in the orig statements below.
A 		.fill x3200		
B 		.fill x3300		
C 		.fill x3400		
LENGTH 	.fill 3			;; Change this value if you decide to increase the size of the arrays below.
.end

;; Do not change any of the .orig lines!
;; If you decide to add more values for debugging, make sure to adjust LENGTH and .blkw 3 accordingly.
.orig x3200				;; Array A : Feel free to change or add values for debugging.
	.fill -2
	.fill 2
	.fill 1
.end

.orig x3300				;; Array B : Feel free change or add values for debugging.
	.fill 1
	.fill 0
	.fill 3
.end

.orig x3400
	.blkw 3				;; Array C: Make sure to increase block size if you've added more values to Arrays A and B!
.end