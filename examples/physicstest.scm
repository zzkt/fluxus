(define (sphere)
    (push)
    (colour (vector (flxrnd) (flxrnd) (flxrnd)))    
    (specular (vector (flxrnd) (flxrnd) (flxrnd)))
    (ambient (vector (flxrnd) (flxrnd) (flxrnd)))
    (shinyness (*(flxrnd)100))
    (scale (vector 0.5 0.5 1.5))
    (let ((ob (build-cube)))    
        (active-box ob))
    (pop))

(define (line n)
    (rotate (vector 0 (* (flxrnd) 90) 0))
    (translate (vector (* 0.01 (flxrnd)) (+ 1 (* 0.01 (flxrnd))) (* 0.001 (flxrnd)) ))
    (sphere)
    (if (eq? 0 n)
        1
        (line (- n 1))))

(clear)
(collisions 1)
(ground-plane (vector 0 1 0) 0)
(show-axis 1)
(clear-colour (vector 0.5 0.1 0.7))
(line 10)
(desiredfps 500)

    
