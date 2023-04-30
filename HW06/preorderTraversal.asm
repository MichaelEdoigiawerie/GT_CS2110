;;=============================================================
;;  CS 2110 - Spring 2023
;;  Homework 6 - Preorder Traversal
;;=============================================================
;;  Name: Michael Edoigiawerie
;;============================================================

;;  In this file, you must implement the 'PREORDER_TRAVERSAL' subroutine.

;;  Little reminder from your friendly neighborhood 2110 TA staff: don't run
;;  this directly by pressing 'Run' in complx, since there is nothing put at
;;  address x3000. Instead, call the subroutine by doing the following steps:
;;      * 'Debug' -> 'Simulate Subroutine Call'
;;      * Call the subroutine at the 'PREORDER_TRAVERSAL' label
;;      * Add the [root (addr), arr (addr), index] params separated by a comma (,) 
;;        (e.g. x4000, x4020, 0)
;;      * Proceed to run, step, add breakpoints, etc.
;;      * Remember R6 should point at the return value after a subroutine
;;        returns. So if you run the program and then go to 
;;        'View' -> 'Goto Address' -> 'R6 Value', you should find your result
;;        from the subroutine there (e.g. Node 8 is found at x4008)

;;  If you would like to setup a replay string (trace an autograder error),
;;  go to 'Test' -> 'Setup Replay String' -> paste the string (everything
;;  between the apostrophes (')) excluding the initial " b' ". If you are 
;;  using the Docker container, you may need to use the clipboard (found
;;  on the left panel) first to transfer your local copied string over.

.orig x3000
    ;; You do not need to write anything here
    HALT

;;  PREORDER_TRAVERSAL Pseudocode (see PDF for explanation and examples)
;;  - Nodes are blocks of size 3 in memory:
;;      * The data is located in the 1st memory location (offset 0 from the node itself)
;;      * The node's left child address is located in the 2nd memory location (offset 1 from the node itself)
;;      * The node's right child address is located in the 3rd memory location (offset 2 from the node itself)

;;  PREORDER_TRAVERSAL(Node root (addr), int[] arr (addr), int index) {
;;      if (root == 0) {
;;          return index;
;;      }
;;
;;      arr[index] = root.data;
;;      index++;
;;
;;      index = PREORDER_TRAVERSAL(root.left, arr, index);
;;      return PREORDER_TRAVERSAL(root.right, arr, index);
;;  }


PREORDER_TRAVERSAL ;; Do not change this label! Treat this as like the name of the function in a function header
    ;; Code your implementation for the PREORDER_TRAVERSAL subroutine here!

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
    LDR R0, R5, #4; R0 = root (addr)
    LDR R1, R5, #5; R1 = arr (addr)
    LDR R2, R5, #6; R2 = index

    IF ADD R0, R0, #0; if (root == null)
       BRnp ENDIF
       STR  R2, R5, #3; return index
       BR ENDSUBROUTINE
    ENDIF

    ADD R1, R1, R2; R1 = arr (addr) + index
    LDR R0, R0, #0; R0 = root.data
    STR R0, R1, #0; arr[index] = root.data

    ADD R2, R2, #1; index++
    
    LDR R1, R5, #5; R1 = arr (addr)
    LDR R0, R5, #4; R0 = root (addr)
    LDR R0, R0, #1; R0 = root.left (addr)

    ADD R6, R6, #-3; Prepare to Push 3 Arguments onto the new Stack Frame
    STR R0, R6, #0; Push root.left (addr) onto the Stack
    STR R1, R6, #1; Push arr (addr) onto the Stack
    STR R2, R6, #2; Push index onto the Stack
    JSR PREORDER_TRAVERSAL; PREORDER_TRAVERSAL(root.left, arr, index)
    LDR R2, R6, #0; index = PREORDER_TRAVERSAL(root.left, arr, index)
    ADD R6, R6, #4; Pop The Return Value and the 3 Arguments off of the Stack

    LDR R0, R5, #4; R0 = root (addr)
    LDR R0, R0, #2; R0 = root.right (addr)

    ADD R6, R6, #-3; Prepare to Push 3 Arguments onto the new Stack Frame
    STR R0, R6, #0; Push root.right (addr) onto the Stack
    STR R1, R6, #1; Push arr (addr) onto the Stack
    STR R2, R6, #2; Push index onto the Stack
    JSR PREORDER_TRAVERSAL; PREORDER_TRAVERSAL(root.right, arr, index)
    LDR R0, R6, #0; R0 = PREORDER_TRAVERSAL(root.right, arr, index)
    ADD R6, R6, #4; Pop The Return Value and the 3 Arguments off of the Stack

    STR R0, R5, #3; return PREORDER_TRAVERSAL(root.right, arr, index)

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

; Needed to Simulate Subroutine Call in Complx
STACK .fill xF000
.end

;;  Assuming the tree starts at address x4000, here's how the tree (see below and in the PDF) is represented in memory
;;
;;              4
;;            /   \
;;           2     8 
;;         /   \
;;        1     3 

.orig x4000 ;; 4    ;; node itself lives here at x4000
    .fill 4         ;; node.data (4)
    .fill x4004     ;; node.left lives at address x4004
    .fill x4008     ;; node.right lives at address x4008
.end

.orig x4004	;; 2    ;; node itself lives here at x4004
    .fill 2         ;; node.data (2)
    .fill x400C     ;; node.left lives at address x400C
    .fill x4010     ;; node.right lives at address x4010
.end

.orig x4008	;; 8    ;; node itself lives here at x4008
    .fill 8         ;; node.data (8)
    .fill 0         ;; node does not have a left child
    .fill 0         ;; node does not have a right child
.end

.orig x400C	;; 1    ;; node itself lives here at x400C
    .fill 1         ;; node.data (1)
    .fill 0         ;; node does not have a left child
    .fill 0         ;; node does not have a right child
.end

.orig x4010	;; 3    ;; node itself lives here at x4010
    .fill 3         ;; node.data (3)
    .fill 0         ;; node does not have a left child
    .fill 0         ;; node does not have a right child
.end

;;  Another way of looking at how this all looks like in memory
;;              4
;;            /   \
;;           2     8 
;;         /   \
;;        1     3 
;;  Memory Address           Data
;;  x4000                    4          (data)
;;  x4001                    x4004      (4.left's address)
;;  x4002                    x4008      (4.right's address)
;;  x4003                    Don't Know
;;  x4004                    2          (data)
;;  x4005                    x400C      (2.left's address)
;;  x4006                    x4010      (2.right's address)
;;  x4007                    Don't Know
;;  x4008                    8          (data)
;;  x4009                    0(NULL)
;;  x400A                    0(NULL)
;;  x400B                    Don't Know
;;  x400C                    1          (data)
;;  x400D                    0(NULL)
;;  x400E                    0(NULL)
;;  x400F                    Dont't Know
;;  x4010                    3          (data)
;;  x4011                    0(NULL)
;;  x4012                    0(NULL)
;;  x4013                    Don't Know
;;  
;;  *Note: 0 is equivalent to NULL in assembly

.orig x4020 ;; Result Array : You can change the block size for debugging!
    .blkw 5
.end