(module fluxus-camera mzscheme
	(require fluxus-engine)
	(provide 
		input-camera
		camera-matrix)
		
	(define camera-matrix (mtranslate (vector 0 1 -10)))
	(define camera-position (vector 0 0 -10))
	(define camera-rot-now (vector 0 0 0 1))
	(define camera-rot-start (vector 0 0 0 1))
	(define click-mouse (vector 0 0))
	(define last-mouse (vector 0 0))
	(define last-button 0)

	; schemified from Artem's code in FluxusMain.cpp
	(define (on-unit-sphere mx my)
	  (let ((mag (+ (* mx mx) (* my my))))
    	(cond 
    	  ((> mag 1.0)
        	(let ((scale (/ 1.0 (sqrt mag))))
        	  (vector (* mx scale) (* my scale) 0)))
    	  (else
    	   (vector mx my (sqrt (- 1 mag)))))))


	(define (input-camera key button special state x y mod width height)
	  (if (and (zero? key) (eq? special -1)) ; = mouse event
    	  (cond 
        	((zero? state) ; click
        	 (vector-set! last-mouse 0 x)
        	 (vector-set! last-mouse 1 y)
        	 (set! last-button button)
        	 (set! camera-rot-start 
            	   (qmul camera-rot-now camera-rot-start))
        	 (set! camera-rot-now (vector 0 0 0 1))
        	 (vector-set! click-mouse 0    (- (/ x (/ (- width  1) 2)) 1))
        	 (vector-set! click-mouse 1 (- (- (/ y (/ (- height 1) 2)) 1))))
        	(else ; drag
        	 (cond
        	   ((eq? last-button 0) ; button 1
            	(let ((d (on-unit-sphere (vector-ref click-mouse 0) 
                                    	 (vector-ref click-mouse 1)))
                	  (m (on-unit-sphere (- (/ x (/ (- width  1) 2)) 1)
                                    	 (- (- (/ y (/ (- height 1) 2)) 1)))))
            	  (vector-set! camera-rot-now 0 (- (* (vector-ref d 1) (vector-ref m 2))
                                                    	  (* (vector-ref d 2) (vector-ref m 1))))
            	  (vector-set! camera-rot-now 1 (- (* (vector-ref d 2) (vector-ref m 0))
                                                    	  (* (vector-ref d 0) (vector-ref m 2))))
            	  (vector-set! camera-rot-now 2 (- (* (vector-ref d 0) (vector-ref m 1))
                                                    	  (* (vector-ref d 1) (vector-ref m 0))))
            	  (vector-set! camera-rot-now 3 (+ (* (vector-ref d 0) (vector-ref m 0))
                                                    	  (* (vector-ref d 1) (vector-ref m 1))
                                                    	  (* (vector-ref d 2) (vector-ref m 2))))

            	  (set! camera-rot-now (qnormalise camera-rot-now))))
        	   ((eq? last-button 1) ; button 2
            	(vector-set! camera-position 0 (+ (vector-ref camera-position 0)
                                                        	(/ (- x (vector-ref last-mouse 0)) 50.0)))
            	(vector-set! camera-position 1 (+ (vector-ref camera-position 1)
                                                    	 (- (/ (- y (vector-ref last-mouse 1)) 50.0)))))
        	   ((eq? last-button 2) ; button 3
            	(vector-set! camera-position 2 (+ (vector-ref camera-position 2)
                                                    	 (- (/ (- y (vector-ref last-mouse 1)) 50.0))))))
        	 (vector-set! last-mouse 0 x)
        	 (vector-set! last-mouse 1 y)
        	 (update-camera)))))


	(define (update-camera)
	  (set! camera-matrix
	   (mmul (mtranslate camera-position)
        	 (qtomatrix (qconjugate 
                    	 (qmul camera-rot-now
                        	   camera-rot-start))))))

)
