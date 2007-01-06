(module fluxus-input mzscheme
	(provide 
		key-pressed
		keys-down
		key-special-pressed
		keys-special-down
		mouse-x
		mouse-y
		register-down
		register-up)

(define keys '())
(define special-keys '())
(define mouse (vector 0 0 #f))
 
; utils funcs for using lists as sets
(define (set-remove a l)
  (if (null? l)
      '()
      (if (eq? (car l) a)
          (set-remove a (cdr l))
          (cons (car l) (set-remove a (cdr l))))))		  
		  
(define (set-add a l)
  (if (not (memq a l))
      (cons a l)
      l))			  

(define (set-contains a l)
  (if (not (memq a l))
      #f
      #t))		  
		  
(define (register-down key button special state x y mod)
	(if (not (eq? key -1))
		(set! keys (set-add key keys)))
	(if (not (eq? special -1))
		(set! special-keys (set-add special special-keys)))
	(cond 
		((and (eq? key 0) (eq? special -1))
			(if (zero? state)
				(vector-set! mouse 2 (+ button 1))
				(vector-set! mouse 2 0))
			(vector-set! mouse 0 x)
			(vector-set! mouse 1 y))))

(define (register-up key button special state x y mod)
	(if (not (eq? key -1))
		(set! keys (set-remove key keys)))
	(if (not (eq? special -1))
		(set! special-keys (set-remove special special-keys))))

(define (key-pressed s)
	(set-contains (car (string->list s)) keys))
	
(define (keys-down)
	keys)

(define (key-special-pressed k)
	(set-contains k special-keys))
	
(define (keys-special-down)
	special-keys)

(define (mouse-x)
	(vector-ref mouse 0))
	
(define (mouse-y)
	(vector-ref mouse 1))

(define (mouse-button)
	(vector-ref mouse 2))
		
	

)
