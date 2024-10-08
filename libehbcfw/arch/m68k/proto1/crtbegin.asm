
    PUBLIC      main

    SECTION     .text
_start::
    JSR         main

.loop:
    BRA         .loop
