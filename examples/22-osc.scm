
(define (osctest)
    (display (osc-peek))(newline))

(osc-source "4444")

(every-frame (osctest))
