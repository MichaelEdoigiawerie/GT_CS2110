;;=============================================================
;;  CS 2110 - Spring 2023
;;  Homework 6 - Insertion Sort
;;=============================================================
;;  Name: Michael Edoigiawerie
;;============================================================

;;  In this file, you must implement the 'INSERTION_SORT' subroutine.

;;  Little reminder from your friendly neighborhood 2110 TA staff: don't run
;;  this directly by pressing 'Run' in complx, since there is nothing put at
;;  address x3000. Instead, call the subroutine by doing the following steps:
;;      * 'Debug' -> 'Simulate Subroutine Call'
;;      * Call the subroutine at the 'INSERTION_SORT' label
;;      * Add the [arr (addr), length] params separated by a comma (,) 
;;        (e.g. x4000, 5)
;;      * Proceed to run, step, add breakpoints, etc.
;;      * INSERTION_SORT is an in-place algorithm, so if you go to the address
;;        of the array by going to 'View' -> 'Goto Address' -> 'Address of
;;        the Array', you should see the array (at x4000) successfully 
;;        sorted after running the program (e.g [2,3,1,1,6] -> [1,1,2,3,6])

;;  If you would like to setup a replay string (trace an autograder error),
;;  go to 'Test' -> 'Setup Replay String' -> paste the string (everything
;;  between the apostrophes (')) excluding the initial " b' ". If you are 
;;  using the Docker container, you may need to use the clipboard (found
;;  on the left panel) first to transfer your local copied string over.

.orig x3000
    ;; You do not need to write anything here
    HALT

;;  INSERTION_SORT **RESURSIVE** Pseudocode (see PDF for explanation and examples)
;; 
;;  INSERTION_SORT(int[] arr (addr), int length) {
;;      if (length <= 1) {
;;        return;
;;      }
;;  
;;      INSERTION_SORT(arr, length - 1);
;;  
;;      int last_element = arr[length - 1];
;;      int n = length - 2;
;;  
;;      while (n >= 0 && arr[n] > last_element) {
;;          arr[n + 1] = arr[n];
;;          n--;
;;      }
;;  
;;      arr[n + 1] = last_element;
;;  }

INSERTION_SORT ;; Do not change this label! Treat this as like the name of the function in a function header
    ;; Code your implementation for the INSERTION_SORT subroutine here!
    ;; NOTE: Your implementation MUST be done recursively

    ;Stack Build Up
    ADD R6, R6, #-4; Push the Return Value, Return Address, Old Frame Pointer, and Local Variables onto the Stack
    STR R7, R6, #2; Save The Return Address to the Stack
    STR R5, R6, #1; Save The Old Frame Pointer to the Stack
    ADD R5, R6, #0; Make the Current Frame Pointer point to the First Local Variable
    ADD R6, R6, #-5; Push The Five General Purpose Registers Onto The Stack
    STR R0, R5, #-1; Save old value of R0 to the Stack
    STR R1, R5, #-2; Save old value of R1 to the Stack
    STR R2, R5, #-3; Save old value of R2 to the Stack
    STR R3, R5, #-4; Save old value of R3 to the Stack
    STR R4, R5, #-5; Save old value of R4 to the Stack

    ;Work of The Subroutine
    LDR R0, R5, #4; R0 = arr (addr)
    LDR R1, R5, #5; R1 = length

    IF  ADD R1, R1, #-1; if (length <= 1)
        BRp ENDIF
        BR ENDSUBROUTINE
    ENDIF

    ADD R6, R6, #-2; Reserve Space for Parameters
    STR R0, R6, #0; Push arr (addr)
    STR R1, R6, #1; Push (length - 1)
    JSR INSERTION_SORT; INSERTION_SORT(arr, length - 1)
    ADD R6, R6, #3; Pop the Return Value and the 2 Arguments off the Stack
 
    ADD R0, R0, R1; R0 = arr (addr) + (length - 1)
    LDR R0, R0, #0; R0 = arr[length - 1]
    ST R0, LASTELEMENT; last_element = R0

    ADD R1, R1, #-1; R1 = length - 2
    ST  R1, N; n = R1
    
    LDR R0, R5, #4; R0 = arr (addr)
    ADD R2, R0, R1; R2 = arr (addr) + n
    LDR R2, R2, #0; R2 = arr[n]
    
    LD  R0, LASTELEMENT; R0 = last_element
    NOT R0, R0; R0 = ~last_element
    ADD R0, R0, #1; R0 = -last_element

    WHILE   ADD R3, R2, R0; while(n >= 0 && arr[n] > last_element)
            BRnz  ENDWHILE
            ADD R1, R1, #0
            BRn  ENDWHILE
            
            ADD R1, R1, #1; R1 = n + 1
            LDR R0, R5, #4; R0 = arr (addr)
            ADD R0, R0, R1; R0 = arr (addr) + (n + 1)
            STR R2, R0, #0; arr[n + 1] = arr[n]
            
            LD  R1, N; R1 = n
            ADD R1, R1, #-1; R1--;
            ST  R1, N; n--;

            LDR R0, R5, #4; R0 = arr (addr)
            ADD R2, R0, R1; R2 = arr (addr) + n
            LDR R2, R2, #0; R2 = arr[n]

            LD  R0, LASTELEMENT; R0 = last_element
            NOT R0, R0; R0 = ~last_element
            ADD R0, R0, #1; R0 = -last_element
            BR  WHILE
    ENDWHILE
            ADD R1, R1, #1; R1 = n + 1
            LDR R0, R5, #4; R0 = arr (addr)
            ADD R0, R0, R1; R0 = arr (addr) + (n + 1)

            LD R1, LASTELEMENT; R1 = last_element
            STR R1, R0, #0; arr[n + 1] = last_element
    
    ENDSUBROUTINE

    ;Stack Teardown
    LDR R4, R5, #-5; Restore the old value of R4
    LDR R3, R5, #-4; Restore the old value of R3
    LDR R2, R5, #-3; Restore the old value of R2
    LDR R1, R5, #-2; Restore the old value of R1
    LDR R0, R5, #-1; Restore the old value of R0
    ADD R6, R5, #0;  Pop Saved Registers off the Stack and Restore The Stack Pointer
    LDR R5, R6, #1;  Restore The Frame Pointer
    LDR R7, R6, #2;  Restore The Return Address
    ADD R6, R6, #3;  Pop The Return Address, Frame Pointer, and Local Variables off the Stack
    RET

    LASTELEMENT .blkw  1
    N  .fill 0

;; Needed to Simulate Subroutine Call in Complx
STACK .fill xF000
.end

.orig x4000	;; Array : You can change these values for debugging!
    .fill 2
    .fill 3
    .fill 1
    .fill 1
    .fill 6
.end