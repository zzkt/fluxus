;-------------------------------------------------
; fluxus application -> fluxus engine
;-------------------------------------------------
; this script plugs the callbacks from the fluxus
; application into the fluxus engine module
;
; todo: put all globals in a container list

(load-extension "modules/fluxus-engine/fluxus-engine.so")
(require fluxus-engine)

;-------------------------------------------------
; every frame stuff 

(define fluxus-user-callback ())

(define fluxus-camera-matrix (mtranslate (vector 0 1 -10)))
(define fluxus-camera-position (vector 0 0 -10))
(define fluxus-camera-rot-now (vector 0 0 0 1))
(define fluxus-camera-rot-start (vector 0 0 0 1))

(define fluxus-click-mouse (vector 0 0))
(define fluxus-last-mouse (vector 0 0))
(define fluxus-last-button 0)

(define fluxus-width 0)
(define fluxus-height 0)

(define-syntax every-frame
  (syntax-rules ()
    ((every-frame expr)
     (set! fluxus-user-callback (lambda () expr)))))

;-------------------------------------------------
; camera

; schemified from Artem's code in FluxusMain.cpp
(define (on-unit-sphere mx my)
  (let ((mag (+ (* mx mx) (* my my))))
    (cond 
      ((> mag 1.0)
        (let ((scale (/ 1.0 (sqrt mag))))
          (vector (* mx scale) (* my scale) 0)))
      (else
       (vector mx my (sqrt (- 1 mag)))))))


(define (fluxus-input-camera key button special state x y mod)
  (if (and (zero? key) (eq? special -1)) ; = mouse event
      (cond 
        ((zero? state) ; click
         (vector-set! fluxus-last-mouse 0 x)
         (vector-set! fluxus-last-mouse 1 y)
         (set! fluxus-last-button button)
         (set! fluxus-camera-rot-start 
               (qmul fluxus-camera-rot-now fluxus-camera-rot-start))
         (set! fluxus-camera-rot-now (vector 0 0 0 1))
         (vector-set! fluxus-click-mouse 0    (- (/ x (/ (- fluxus-width  1) 2)) 1))
         (vector-set! fluxus-click-mouse 1 (- (- (/ y (/ (- fluxus-height 1) 2)) 1))))
        (else ; drag
         (cond
           ((eq? fluxus-last-button 0) ; button 1
            (let ((d (on-unit-sphere (vector-ref fluxus-click-mouse 0) 
                                     (vector-ref fluxus-click-mouse 1)))
                  (m (on-unit-sphere (- (/ x (/ (- fluxus-width  1) 2)) 1)
                                     (- (- (/ y (/ (- fluxus-height 1) 2)) 1)))))
              (vector-set! fluxus-camera-rot-now 0 (- (* (vector-ref d 1) (vector-ref m 2))
                                                      (* (vector-ref d 2) (vector-ref m 1))))
              (vector-set! fluxus-camera-rot-now 1 (- (* (vector-ref d 2) (vector-ref m 0))
                                                      (* (vector-ref d 0) (vector-ref m 2))))
              (vector-set! fluxus-camera-rot-now 2 (- (* (vector-ref d 0) (vector-ref m 1))
                                                      (* (vector-ref d 1) (vector-ref m 0))))
              (vector-set! fluxus-camera-rot-now 3 (+ (* (vector-ref d 0) (vector-ref m 0))
                                                      (* (vector-ref d 1) (vector-ref m 1))
                                                      (* (vector-ref d 2) (vector-ref m 2))))
              
              (set! fluxus-camera-rot-now (qnormalise fluxus-camera-rot-now))))
           ((eq? fluxus-last-button 1) ; button 2
            (vector-set! fluxus-camera-position 0 (+ (vector-ref fluxus-camera-position 0)
                                                        (/ (- x (vector-ref fluxus-last-mouse 0)) 50.0)))
            (vector-set! fluxus-camera-position 1 (+ (vector-ref fluxus-camera-position 1)
                                                     (- (/ (- y (vector-ref fluxus-last-mouse 1)) 50.0)))))
           ((eq? fluxus-last-button 2) ; button 3
            (vector-set! fluxus-camera-position 2 (+ (vector-ref fluxus-camera-position 2)
                                                     (- (/ (- y (vector-ref fluxus-last-mouse 1)) 50.0))))))
         (vector-set! fluxus-last-mouse 0 x)
         (vector-set! fluxus-last-mouse 1 y)
         (fluxus-update-camera)))))
      

(define (fluxus-update-camera)
  (set! fluxus-camera-matrix
   (mmul (mtranslate fluxus-camera-position)
         (qtomatrix (qconjugate 
                     (qmul fluxus-camera-rot-now
                           fluxus-camera-rot-start))))))


;-------------------------------------------------
; keyboard state

;-------------------------------------------------
; mouse state

;-------------------------------------------------
; callbacks - these are called directly from the
; fluxus application

; reshape function

(define (fluxus-reshape-callback x y)
  (set! fluxus-width x)
  (set! fluxus-height y)
  (reshape x y))

; input functions

(define (fluxus-input-callback key button special state x y mod)
  (fluxus-input-camera key button special state x y mod))

(define (fluxus-input-release-callback key button special state x y mod)
  0)

; the main callback every frame

(define (fluxus-frame-callback)
  (set-camera fluxus-camera-matrix)
  (begin-scene)
  (if (not (null? fluxus-user-callback))
      (fluxus-user-callback))
  (end-scene))

