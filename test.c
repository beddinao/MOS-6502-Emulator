#include <stdio.h>
#include <stdint.h>


int     main() {
    uint16_t    a = 34918;  // 10001000 01100110
                            // 136      102
    uint8_t     fn = a >> 8;
    uint8_t     sn = a & 0x00FF;
    uint16_t    b = fn << 8 | sn;
    
    printf("fn %i, sn %i = b %i\n", fn, sn, b);
    //  fn 136, sn 102 = b 34918
    
    
    
    uint8_t     sr = 202;  // 1100 1010
                           // NV-B DIZC
    
    uint8_t     N = (sr >> 7) & 1,
                V = (sr >> 6) & 1,
                B = (sr >> 4) & 1,
                D = (sr >> 3) & 1,
                I = (sr >> 2) & 1,
                Z = (sr >> 1) & 1,
                C = sr & 1;
    
    uint8_t     asr = N<<7|V<<6|0<<5|B<<4|D<<3|I<<2|Z<<1|C;
    
    printf("sr %i, asr %i, %i%i%i%i %i%i%i%i\n", sr, asr, N, V, 0, B, D, I, Z, C);
    //  sr 202, asr 202, 1100 1010
    
    
    // 1001 1100 (156)
    // NV-B DIZC
    
    uint8_t     fs = 4;
    fs = (1 << 7);
    
    
    printf("fs %i\n", fs);
}
