;;=============================================================
;; CS 2110 - Spring 2023
;; Homework 5 - palindrome
;;=============================================================
;; Name: Michael Edoigiawerie
;;=============================================================

;;  NOTE: Let's decide to represent "true" as a 1 in memory and "false" as a 0 in memory.
;;
;;  Pseudocode (see PDF for explanation)
;;  Pseudocode values are based on the labels' default values.
;;
;;  String str = "aibohphobia";
;;  boolean isPalindrome = true
;;  int length = 0;
;;  while (str[length] != '\0') {
;;		length++;
;;	}
;; 	
;;	int left = 0
;;  int right = length - 1
;;  while(left < right) {
;;		if (str[left] != str[right]) {
;;			isPalindrome = false;
;;			break;
;;		}
;;		left++;
;;		right--;
;;	}
;;	mem[mem[ANSWERADDR]] = isPalindrome;

.orig x3000
	;; YOUR CODE HERE
	LD R0, LENGTH; R0 = length
	LD R1, isPalindrome; R1 = isPalindrome

	LD R2, STRING; R2 = String Addr
	ADD R2, R2, R0; R2 = String Addr + length

WHILE	LDR R2, R2, #0; R2 = str[length], while (str[length] != '\0')
		BRz	ENDWHILE
		ADD	R0, R0, #1; length++
		LD R2, STRING; R2 = String Addr
		ADD R2, R2, R0; R2 = String Addr + length
		BR	WHILE
	
ENDWHILE
	ADD R1, R0, #-1; R1 = length - 1
	LD R0, LEFT; R0 = left

	ST	R1, RIGHT; right = R1
	NOT R1, R1; R1 = ~right
	ADD R1, R1, #1; R1 = -right

WHILE1	ADD R3, R0, R1; while (left < right)
		BRzp ENDWHILE1
		
		LD R2, STRING; R2 = String Addr
		ADD R2, R2, R0; R2 = String Addr + left
		LDR R2, R2, #0; R2 = str[left]
		ST R2, STRLEFT; str[left] = R2

		LD R1, RIGHT; R1 = RIGHT
		LD R2, STRING; R2 = String Addr
		ADD R4, R2, R1; R4 = String Addr + right
		LDR R4, R4, #0; str[right]

		NOT R4, R4; R4 = ~str[right]
		ADD R4, R4, #1; R4 = -str[right]

		LD R2, STRLEFT; R2 = str[left]

		IF	ADD R4, R2, R4; if (str[left] != str[right])
			BRz	ENDIF
			LD R1, isPalindrome; R1 = isPalindrome
			AND R1, R1, #0; R1 = 0
			ST R1, isPalindrome; isPalindrome = R1
			BR ENDWHILE1
		
		ENDIF
			ADD R0, R0, #1; left++
			LD R1, RIGHT; R1 = right
			ADD R1, R1, #-1; right--
			ST R1, RIGHT; right = R1
			NOT R1, R1; R1 = ~right
			ADD R1, R1, #1; R1 = -right
			BR WHILE1

ENDWHILE1
			LD R0, isPalindrome; R0 = isPalindrome
			STI R0, ANSWERADDR; mem[mem[ANSWERADDR]] = isPalindrome
			HALT

LENGTH .fill 0
isPalindrome .fill 1
LEFT	.fill 0
RIGHT	.fill 0
STRLEFT	.fill 0

;; Do not change these values!
STRING	.fill x4004
ANSWERADDR 	.fill x5005
.end

;; Do not change any of the .orig lines!
.orig x4004				   
	.stringz "aibohphobia" ;; Feel free to change this string for debugging.
.end

.orig x5005
	ANSWER  .blkw 1
.end