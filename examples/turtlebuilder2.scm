(colour #(1 1 1))

(define (poly n)
	(turtle-prim 1)
	(turtle-vert)
	(turtle-move 1)
	(turtle-turn #(0 90 0))
	(turtle-vert)
	(turtle-move 1)
	(turtle-turn #(0 90 0))
	(turtle-vert)
	(turtle-move 1)
	(turtle-vert)
	(turtle-turn (vector (* (gh n) 2) 0 0))
	;(hint-none)
	;(hint-wire)
	(turtle-build))

(define (shape n a)
	(poly n)
	(turtle-turn (vector (/ 360 a) 0 0))
	(if (eq? n 0)
		1
		(shape (- n 1) a)))

(define (meta-shape nn)
	(rotate (vector 0 40 0)) 
	(shape 10 10)
	(if (eq? nn 0)
		1
		(meta-shape (- nn 1))))

;;(turtle-reset)
(backfacecull 0)
(engine-callback "(run)")

(define (run)
	(clear)
	(turtle-reset)
	(engine-callback "(run)")
	(push)
	(push)
	(meta-shape 10)
	(pop))

(clear-colour #(0.3 0.3 0.3))
(blur 0.1)
(show-axis 0)
(shape 10 10)