;; gcd.tm 
;; - calculate the greatest common divisor of two positive integers
;; > input: two strings of 1's, represents the two integer with their length
;; > output: one string of 1's, whose length represents the gcd of the input
;; - the result will be on the first tape

;; the state set 
#Q = {initial,halt_accept,READ_do,READ_cl,READ_end,RET_do,RET_cl,RET_end,SW_LT_do,SW_LT_cl,SW_LT_cl2,SW_LT_end}

;; the input symbol set 
#S = {0,1}

;; the tape symbol set
#G = {0,1,_,a,b,c,d,e,f,g,t}

;; the initial state 
#q0 = initial

;; the blank symbol
#B = _

;; the final state set 
#F = {halt_accept}

;; the tape number
;; > tape0: input and output
;; > tape1: register $r1, always represents an positive integer
;; > tape2: register $r2, always represents an positive integer
;; > tape3: a stack for procedures, head3 points to argument and return value register $a
;; > tape4: the return site of a procedure call
#N = 5

;;; << begin of transfer functions >> ;;;

;; <MAIN> {
;; > Precond:  initial state of the turing machine
;; > Func:     the subtraction-based version of Euclidean algorithm
;; > Postcond: result on tape0, state=halt_accept

initial ***** ***0a ****r READ_do              ; <callsite:a> call 'READ(0)', 
READ_end ****a ***1b r***r READ_do             ; <callsite:b> call 'READ(1)'
READ_end ****b ***0c *l**r RET_do              ; <callsite:c>  call 'RET(0)'
RET_end ****c ***1d **l*r RET_do               ; <callsite:d>  call 'RET(1)'

RET_end ****d ****t ****r EUCLID_do               ; <callsite:t> call 'EUCLID()'
EUCLID_end *1**t 1***t rr*** EUCLID_end
EUCLID_end *_**t ****_ ***** halt_accept
;; } <MAIN>


;; <READ> {
;; > Precond:  state=READ_do, head0 on the number to be read, head($a+1) is the position to be wrote
;; > Func:     read the number to $r1 or $r2, selected by $a
;; > Postcond: state=READ_end, tape0 is consumed, all heads point to the end of each string

;; $a=0, read the number onto tape1
READ_do 1**0* _1*** rr*** READ_do             ; meet '1', then copy to the tape1, move right
READ_do ***0* _**** ***** READ_cl             ; otherwise, end of the number, then turn to 'READ_cl'

;; $a=1, read the second number onto tape2
READ_do 1**1* _*1** r*r** READ_do             ; meet '1', then copy to the tape2, move right
READ_do ***1* _**** ***** READ_cl             ; otherwise, end of the number, then turn to 'READ_cl' 

READ_cl ***** ***** ****l READ_end            ; pop callstack

;; } <READ>


;; <RET> {
;; > Precond:  state=RET_do, head($a+1) is non-blank
;; > Func:     selected by $a, move head to point to the next cell of the first blank on the left
;; > Postcond: state=RET_end, the selected head on the first position, $a not modified

;; case $a=0
RET_do *_*0* ***** *r*** RET_cl               ; meet '_', head1 move right, finishing 'RET'
RET_do ***0* ***** *l*** RET_do               ; otherwise, head1 move left

;; case $a=1
RET_do **_1* ***** **r** RET_cl               ; meet '_', head2 move right, finishing 'RET'
RET_do ***1* ***** **l** RET_do               ; otherwise, head2 move left

RET_cl ***** ***** ****l RET_end              ; pop callstack

;; } <RET>


;; <SW_LT> {
;; > Precond:  state=SW_LT_do, $r1=a, $r2=b 
;; > Func:     swap $r1, $r2 if $r1 < $r2
;; > Postcond: state=SW_LT_end, $r1, $r2 remains

SW_LT_do *11** ***** *rr** SW_LT_do                    ; if not blank, continue
SW_LT_do *1_** ***** **l** SW_LT_cl                 ; blank on $r2, do nothing
SW_LT_do *_1** *1_** *rr** SW_LT_do                 ; blank on $r1, continue swaping
SW_LT_do *__** ***** *ll** SW_LT_cl                 ; otherwise, blank on both, return true

;; cleaning, recover $r1, $r2
SW_LT_cl *11** ***** ***** SW_LT_cl2                ; return to a position so that both heads are 1 
SW_LT_cl ***** ***** *ll** SW_LT_cl

SW_LT_cl2 ***** ***0e ****r RET_do               ; <callsite:e> call RET(0)
RET_end ****e ***1f ****r RET_do               ; <callsite:f> call RET(1)
RET_end ****f ***__ ****l SW_LT_end              ; pop callstack

;; } <SW_LT>


;; <SUB> {
;; > Precond:  state=SUB_do, $r1=a, $r2=b, a>b
;; > Postcond: state=SUB_end, $r1=a-b, $r2=b

SUB_do *11** *_*** *rr** SUB_do             ; subtraction 
SUB_do *1_** ***** **l** SUB_cl             ; finishing

SUB_cl ***** ***1g ****r RET_do             ; <callsite:g> call RET(1)
RET_end ****g ***__ ****l SUB_end           ; pop callstack

;; } <SUB>

;; <EQ> {
;; > Precond:  state=EQ_do, $r1=a, $r2=b
;; > Postcond: state=EQ_end, $a=(eq? a b), $r1, $r2 remains

EQ_do *11** ***** *rr** EQ_do 
EQ_do *1_** ***0* *ll** EQ_cl 
EQ_do *_1** ***0* *ll** EQ_cl
EQ_do *__** ***1* *ll** EQ_cl

EQ_cl *__** ***** *rr*l EQ_end
EQ_cl ***** ***** *ll** EQ_cl

;; } <EQ>

;; <EUCLID> {
;; > Precond:  state=EUCLID_do, $r1=a, $r2=b, $a=(eq? a b)
;; > Func:     perform the Euclidean algorithm
;; > Postcond: state=EUCLID_end, $r1=$r2=gcd(a, b)

EUCLID_do ***** ****h ****r EQ_do             ; <callsite:h> call EQ(), test (eq? a b)
EQ_end ***1h ****_ ****l EUCLID_end            ; if a = b, return

; otherwise, $r1=max(a,b), $r2=min(a,b), then call EUCLID($r1-$r2, $r2, $r1?$r2)
EQ_end ***0h ****i ****r SW_LT_do              ; <callsite:i> call SW_LT()
SW_LT_end ****i ****j ****r SUB_do              ; <callsite:j> call SUB()
SUB_end ****j ****_ ***** EUCLID_do             ; tail recursion, call EUCLID()

;; } <EUCLID>

;;; << end of file >> ;;;
