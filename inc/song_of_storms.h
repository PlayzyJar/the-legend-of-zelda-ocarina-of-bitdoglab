#define DO 0
#define REb 1
#define RE 2
#define MIb 3
#define MI 4
#define FA 5
#define SOLb 6
#define SOL 7
#define LAb 8
#define LA 9
#define SIb 10
#define SI 11

#define do 12
#define reb 13
#define re 14
#define mib 15
#define mi 16
#define fa 17
#define solb 18
#define sol 19
#define lab 20
#define la 21
#define sib 22
#define si 23

const float notas[24] = {
    261.625519,
    277.182648,
    293.664734,
    311.126984,
    329.627533,
    349.228241,
    369.994385,
    391.995392,
    415.304688,
    440,
    466.163788,
    493.883301,

    523.251099,
    554.365234,
    587.329529,
    622.253906,
    659.255127,
    698.456482,
    739.988831,
    783.990845,
    830.609375,
    880,
    932.327576,
    987.766602};

const int musica[] = {
    RE, FA, re, RE, FA, re,
    MI, FA, MI, FA, MI, DO, LA,
    LA, RE, FA, SOL, LA,
    LA, RE, FA, SOL, MI
};

const int times_on[] = {
    160000, // RE
    150000, // FA
    385000, // rE
    160000, // RE
    150000, // FA
    385000, // re
    265000, // MI
    150000, // FA
    150000, // MI
    150000, // FA
    150000, // MI
    150000, // DO
    385000, // LA
    285000, // LA
    400000, // RE
    150000, // FA
    150000, // SOL
    385000, // LA
    285000, // LA
    400000, // RE
    150000, // FA
    150000, // SO
    385000  // MI
};

const int times_off[] = {
    55000,  // RE
    55000,  // FA
    265000, // re
    55000,  // RE
    55000,  // FA
    265000, // re
    200000, // MI
    55000,  // FA
    55000,  // MI
    55000,  // FA
    55000,  // MI
    55000,  // DO
    265000, // LA
    100000, // LA
    100000, // RE
    55000,  // FA
    55000,  // SOL
    265000, // LA
    100000, // LA
    100000, // RE
    55000,  // FA
    55000,  // SOL
    265000  // MI
};