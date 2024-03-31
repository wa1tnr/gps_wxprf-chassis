commit 2689f00d43e413f6156a0128f5c9ea7a7cc9b64a
Date:   Tue Dec  7 14:01:46 UTC 2021

    The (rotary-encoder centric) program is now responsive
    as expected, given the hardware (no debounce circuitry).

    Handles the negative case (rotary enc counter goes negative).

    Future improvements likely to lead to hardware upgrade
    (hardware debounce of the encoder pins).

    Specifically, twin ISR's for the two encoder knob pins
    seemed to slow the system down - which was unexpected.

    It seems to 'work better' with them in a single ISR.

    No clear idea why that would be the case, at this juncture.

END.
