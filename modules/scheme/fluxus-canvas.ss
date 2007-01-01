; this script plugs the callbacks from the fluxus
; application into the fluxus engine module
			   
(module fluxus-canvas mzscheme
	(require fluxus-engine)
	(require fluxus-audio)
	(require (lib "fluxus-camera.ss" "fluxus-0.12"))
	(provide 
		fluxus-reshape-callback 
		fluxus-input-callback 
		fluxus-input-release-callback
		fluxus-frame-callback
		set-user-callback!
		every-frame)
		
;-------------------------------------------------
; every frame stuff 

(define user-callback ())

(define (set-user-callback! s)
	(set! user-callback s))
	
; define the every-frame syntax
(define-syntax every-frame
  (syntax-rules ()
    ((every-frame expr)
     (set-user-callback! (lambda () expr)))))


(define width 0)
(define height 0)

;-------------------------------------------------
; keyboard state

;-------------------------------------------------
; mouse state

;-------------------------------------------------
; callbacks - these are called directly from the
; fluxus application

; reshape function

(define (fluxus-reshape-callback x y)
  (set! width x)
  (set! height y)
  (reshape x y))

; input functions

(define (fluxus-input-callback key button special state x y mod)
  (input-camera key button special state x y mod width height))

(define (fluxus-input-release-callback key button special state x y mod)
  0)

; the main callback every frame

(define (fluxus-frame-callback)
  (set-camera-transform (minverse camera-matrix))
  (begin-scene)
  (if (not (null? user-callback))
      (user-callback))
  (end-scene)
  (tick-physics)
  (update-audio))

)
