; .-----..-.    .-. .-..-.  .-..-. .-..-----.
; | .---'| |    | | | || |  | || | | || .---'
; | `---.| |    | | | | \ \/ / | | | || `---.
; | .---'| |    | | | | / /\ \ | | | |`---. |
; | |    | `---.| `-' || |  | || `-' |.---' |
; `-'    `-----'`-----'`-'  `-'`-----'`-----'
; (c) 2007 dave griffiths dave@pawfal.org GPL

; this is the startup script for the fluxus canvas

; setup where to find the library module collections
; dunno why we have to do this as it is/should be standard???
(current-library-collection-paths 
	(path-list-string->path-list "/usr/local/lib/plt/collects" 
	(current-library-collection-paths)))

; first we need to load the binary module extensions
(load-extension (build-path (path->string (car (current-library-collection-paths))) "fluxus"
               "compiled" "native" (system-library-subpath #f) "fluxus-engine.so"))

; now require everything we want
(require fluxus-engine)
(require (lib "fluxus-canvas.ss" "fluxus"))
(require (lib "fluxus-obj-import.ss" "fluxus"))

;-------------------------------------------------
; here is the hacking section
; todo: remove all below at some point

; need some things to get us a guile like environment for script compatibility...
(require (all-except (lib "misc.ss" "swindle") identity regexp-quote concat))

; override the built in time function for pre 0.12 compatibility
(define time flxtime)
