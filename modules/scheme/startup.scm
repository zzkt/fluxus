; .-----..-.    .-. .-..-.  .-..-. .-..-----.
; | .---'| |    | | | || |  | || | | || .---'
; | `---.| |    | | | | \ \/ / | | | || `---.
; | .---'| |    | | | | / /\ \ | | | |`---. |
; | |    | `---.| `-' || |  | || `-' |.---' |
; `-'    `-----'`-----'`-'  `-'`-----'`-----'
; (c) 2007 dave griffiths dave@pawfal.org GPL

; boot up fluxus 
(load (string-append "/usr/local/lib/plt/collects/fluxus-" 
	fluxus-version "/fluxus-boot.scm"))
; the line above ensures the correct bootup script 
; is called for the right version of fluxus

; you can add your own commands here which will be run on startup
