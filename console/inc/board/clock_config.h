// LSE (32.768 kHz RTC crystal) initialization notes:
// 1. RCC_BDCR is write-protected after reset. Set PWR_CR1.DBP = 1
//    BEFORE writing LSEON, LSEDRV, RTCSEL, or RTCEN — writes are
//    silently ignored otherwise.
// 2. LSEDRV[1:0] reset default is already Low (00), which is correct
//    for this crystal (0.5 uW max drive level — see ABS04W-6pF specs).
//    Do NOT raise this to Medium-low/Medium-high/High even if copying
//    LSE init boilerplate from reference code — those settings assume
//    a more robust crystal than this one.
