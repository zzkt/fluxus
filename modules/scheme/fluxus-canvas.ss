; Copyright (C) 2007 Dave Griffiths
;
; This program is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 2 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program; if not, write to the Free Software
; Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

; this script plugs the callbacks from the fluxus
; application into the fluxus engine module
			   
(module fluxus-canvas mzscheme
	(require fluxus-engine)
	(require fluxus-audio)
	(require (lib "fluxus-input.ss" "fluxus-0.12"))
	(require (lib "fluxus-camera.ss" "fluxus-0.12"))
	(provide 
		fluxus-reshape-callback 
		fluxus-input-callback 
		fluxus-input-release-callback
		fluxus-frame-callback
		set-user-callback!
		every-frame
		clear)
		
;-------------------------------------------------
; every frame stuff 

(define user-callback '())

(define (set-user-callback! s)
	(set! user-callback s))
	
; define the every-frame syntax
(define-syntax every-frame
  (syntax-rules ()
    ((every-frame expr)
     (set-user-callback! (lambda () expr)))))

(define (clear)
	(set! user-callback '())
	(clear-engine))
	
(define width 0)
(define height 0)

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
	(register-down key button special state x y mod)
	(input-camera key button special state x y mod width height))

(define (fluxus-input-release-callback key button special state x y mod)
	(register-up key button special state x y mod))

; the main callback every frame

(define (fluxus-frame-callback)
  (set-camera (get-camera-transform))
  (begin-scene)
  (if (not (null? user-callback))
      (user-callback))
  (end-scene)
  (tick-physics)
  (update-audio))
  
)
