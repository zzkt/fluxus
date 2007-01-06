; this is the startup script for the fluxus canvas
; this script loads all the modules and sets things up so the 
; fluxus application works without having to worry about setup

; setup where to find the library module collections
; dunno why we have to do this as it is/should be standard???
(current-library-collection-paths 
	(path-list-string->path-list "/usr/local/lib/plt/collects" 
	(current-library-collection-paths)))

; the path to load extensions from
(define fluxus-extension-path 
	(build-path (path->string (car (current-library-collection-paths))) "fluxus-0.12"
               "compiled" "native" (system-library-subpath #f)))
			   
; load the binary module extensions
(load-extension (build-path fluxus-extension-path "fluxus-engine.so"))
(load-extension (build-path fluxus-extension-path "fluxus-audio.so"))
(load-extension (build-path fluxus-extension-path "fluxus-osc.so"))

; now require everything we want
(require fluxus-engine)
(require fluxus-audio)
(require fluxus-osc)

(require (lib "fluxus-canvas.ss" "fluxus-0.12"))
(require (lib "fluxus-input.ss" "fluxus-0.12"))
(require (lib "fluxus-camera.ss" "fluxus-0.12"))
(require (lib "fluxus-obj-import.ss" "fluxus-0.12"))

;-------------------------------------------------
; here is the hacking section
; todo: remove all below at some point

; need some things to get us a guile like environment for script compatibility...
(require (all-except (lib "misc.ss" "swindle") identity regexp-quote concat))

; override the built in time function for pre 0.12 compatibility
(define time flxtime)
