filter_google_dns:
    ldh     [12]          ; 16 bit Eth proto value after MACs
    jne     #0x0800, fail ; 0x0800 = IPv4
    ldb     [23]          ; IP header one byte proto number
    jne     #17, fail     ; 17 = UDP, 6 = TCP
    ld      [30]          ; 32 bit IPv4 address value
    jne     #0x08080808, fail   ; 8.8.8.8
success:
    ret     #-1           ; -1 == 0xFFFFFFFF (maximum size)
fail:
    ret     #0            ; 0 is empty
