#include <stdio.h>
#include <stdint.h>
// https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf  33p ~
unsigned char key192[24] = {0x8e, 0x73, 0xb0, 0xf7, 0xda, 0x0e, 0x64, 0x52, 0xc8, 0x10, 0xf3, 0x2b,
0x80, 0x90, 0x79, 0xe5, 0x62, 0xf8, 0xea, 0xd2, 0x52, 0x2c, 0x6b, 0x7b};

unsigned char key256[32] = {0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 };

static uint8_t Sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

static const uint8_t round_Constant[10] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

void AES192_KeySchedule_2(uint8_t key[], uint8_t roundKey[][16])
{
    
	for (int j = 0; j < 16; j++) //word 0~3
		roundKey[0][j] = key[j];

    for (int j = 0; j < 8; j++) //word 4~5
		roundKey[1][j] = key[j + 16];

	// word 6
	roundKey[1][8] = roundKey[0][0] ^ round_Constant[0] ^ Sbox[roundKey[1][5]];
    roundKey[1][9] = roundKey[0][1] ^ Sbox[roundKey[1][6]];
	roundKey[1][10] = roundKey[0][2] ^ Sbox[roundKey[1][7]];
	roundKey[1][11] = roundKey[0][3] ^ Sbox[roundKey[1][4]];

    //word 7~11
    roundKey[1][12] = roundKey[0][4] ^ roundKey[1][8];
    roundKey[1][13] = roundKey[0][5] ^ roundKey[1][9];
    roundKey[1][14] = roundKey[0][6] ^ roundKey[1][10];
    roundKey[1][15] = roundKey[0][7] ^ roundKey[1][11];
    for (int i = 0; i < 8; i++) {
		roundKey[2][i] = roundKey[0][i + 8] ^ roundKey[1 + i/4][(i + 12) % 16];
    }
    for (int i = 0; i < 8; i++) {
        roundKey[2][i + 8] = roundKey[1][i] ^ roundKey[2][i + 4];
    }

    // word 12
    roundKey[3][0] = roundKey[1][8] ^ round_Constant[1] ^ Sbox[roundKey[2][13]];
    roundKey[3][1] = roundKey[1][9] ^ Sbox[roundKey[2][14]];
	roundKey[3][2] = roundKey[1][10] ^ Sbox[roundKey[2][15]];
	roundKey[3][3] = roundKey[1][11] ^ Sbox[roundKey[2][12]];

    //word 13~17
	roundKey[3][4] = roundKey[1][12] ^ roundKey[3][0];
	roundKey[3][5] = roundKey[1][13] ^ roundKey[3][1];
	roundKey[3][6] = roundKey[1][14] ^ roundKey[3][2];
	roundKey[3][7] = roundKey[1][15] ^ roundKey[3][3];
    for (int i = 0; i < 8; i++) { 
		roundKey[3][i + 8] = roundKey[2][i] ^ roundKey[3][i + 4];
    }
    for (int i = 0; i < 8; i++) {
        roundKey[4][i] = roundKey[2][i + 8] ^ roundKey[3 + i/4][(i + 12) % 16];
    }


    // word 18
    roundKey[4][8] = roundKey[3][0] ^ round_Constant[2] ^ Sbox[roundKey[4][5]];
    roundKey[4][9] = roundKey[3][1] ^ Sbox[roundKey[4][6]];
	roundKey[4][10] = roundKey[3][2] ^ Sbox[roundKey[4][7]];
	roundKey[4][11] = roundKey[3][3] ^ Sbox[roundKey[4][4]];

    //word 19~23
    for (int i = 0; i < 8; i++) {
		roundKey[4 + i/4][(i + 12) % 16] = roundKey[3][i + 4] ^ roundKey[4][i + 8];
    }
    for (int i = 0; i < 8; i++) {
        roundKey[5][i + 4] = roundKey[3 + i/4][(i + 12) % 16] ^ roundKey[5][i];
    }
    roundKey[5][12] = roundKey[4][4] ^ roundKey[5][8];
    roundKey[5][13] = roundKey[4][5] ^ roundKey[5][9];
    roundKey[5][14] = roundKey[4][6] ^ roundKey[5][10];
    roundKey[5][15] = roundKey[4][7] ^ roundKey[5][11];

    // word 24
    roundKey[6][0] = roundKey[4][8] ^ round_Constant[3] ^ Sbox[roundKey[5][13]];
    roundKey[6][1] = roundKey[4][9] ^ Sbox[roundKey[5][14]];
	roundKey[6][2] = roundKey[4][10] ^ Sbox[roundKey[5][15]];
	roundKey[6][3] = roundKey[4][11] ^ Sbox[roundKey[5][12]];

    //word 25~29
    for (int i = 0; i < 8; i++) {
		roundKey[6][i + 4] = roundKey[4 + i/4][(i + 12) % 16] ^ roundKey[6][i];
    }
    for (int i = 0; i < 8; i++) {
        roundKey[6 + i/4][(i + 12) % 16] = roundKey[5][i + 4] ^ roundKey[6][i + 8];
    }
    roundKey[7][4] = roundKey[5][12] ^ roundKey[7][0];
    roundKey[7][5] = roundKey[5][13] ^ roundKey[7][1];
    roundKey[7][6] = roundKey[5][14] ^ roundKey[7][2];
    roundKey[7][7] = roundKey[5][15] ^ roundKey[7][3];

    // word 30
    roundKey[7][8] = roundKey[6][0] ^ round_Constant[4] ^ Sbox[roundKey[7][5]];
    roundKey[7][9] = roundKey[6][1] ^ Sbox[roundKey[7][6]];
	roundKey[7][10] = roundKey[6][2] ^ Sbox[roundKey[7][7]];
	roundKey[7][11] = roundKey[6][3] ^ Sbox[roundKey[7][4]];

    //word 31~35
    for (int i = 0; i < 8; i++) {
		roundKey[7 + i/4][(i + 12) % 16] = roundKey[6][i + 4] ^ roundKey[7][i + 8];
    }
    for (int i = 0; i < 8; i++) {
        roundKey[8][i + 4] = roundKey[6 + i/4][(i + 12) % 16] ^ roundKey[8][i];
    }
    roundKey[8][12] = roundKey[7][4] ^ roundKey[8][8];
    roundKey[8][13] = roundKey[7][5] ^ roundKey[8][9];
	roundKey[8][14] = roundKey[7][6] ^ roundKey[8][10];
	roundKey[8][15] = roundKey[7][7] ^ roundKey[8][11];

    // word 36
    roundKey[9][0] = roundKey[7][8] ^ round_Constant[5] ^ Sbox[roundKey[8][13]];
    roundKey[9][1] = roundKey[7][9] ^ Sbox[roundKey[8][14]];
	roundKey[9][2] = roundKey[7][10] ^ Sbox[roundKey[8][15]];
	roundKey[9][3] = roundKey[7][11] ^ Sbox[roundKey[8][12]];

    //word 37~41
    for (int i = 0; i < 8; i++) {
		roundKey[9][i + 4] = roundKey[7 + i/4][(i + 12) % 16] ^ roundKey[9][i];
    }
    for (int i = 0; i < 8; i++) {
        roundKey[9 + i/4][(i + 12) % 16] = roundKey[8][i + 4] ^ roundKey[9][i + 8];
    }
    roundKey[10][4] = roundKey[8][12] ^ roundKey[10][0];
    roundKey[10][5] = roundKey[8][13] ^ roundKey[10][1];
	roundKey[10][6] = roundKey[8][14] ^ roundKey[10][2];
	roundKey[10][7] = roundKey[8][15] ^ roundKey[10][3];

    // word 42
    roundKey[10][8] = roundKey[9][0] ^ round_Constant[6] ^ Sbox[roundKey[10][5]];
    roundKey[10][9] = roundKey[9][1] ^ Sbox[roundKey[10][6]];
	roundKey[10][10] = roundKey[9][2] ^ Sbox[roundKey[10][7]];
	roundKey[10][11] = roundKey[9][3] ^ Sbox[roundKey[10][4]];

    //word 43~47
    roundKey[10][12] = roundKey[9][4] ^ roundKey[10][8];
    roundKey[10][13] = roundKey[9][5] ^ roundKey[10][9];
	roundKey[10][14] = roundKey[9][6] ^ roundKey[10][10];
	roundKey[10][15] = roundKey[9][7] ^ roundKey[10][11];
    for (int i = 0; i < 8; i++) { 
		roundKey[11][i] = roundKey[9][i + 8] ^ roundKey[10 + i/4][(i + 12) % 16];
    }
    for (int i = 0; i < 8; i++) {
        roundKey[11][i + 8] = roundKey[10][i] ^ roundKey[11][i + 4];
    }

    // word 48
    roundKey[12][0] = roundKey[10][8] ^ round_Constant[7] ^ Sbox[roundKey[11][13]];
    roundKey[12][1] = roundKey[10][9] ^ Sbox[roundKey[11][14]];
	roundKey[12][2] = roundKey[10][10] ^ Sbox[roundKey[11][15]];
	roundKey[12][3] = roundKey[10][11] ^ Sbox[roundKey[11][12]];

    //word 49~53
    roundKey[12][4] = roundKey[10][12] ^ roundKey[12][0];
    roundKey[12][5] = roundKey[10][13] ^ roundKey[12][1];
	roundKey[12][6] = roundKey[10][14] ^ roundKey[12][2];
	roundKey[12][7] = roundKey[10][15] ^ roundKey[12][3];
    for (int i = 0; i < 8; i++) { 
		roundKey[12][i + 8] = roundKey[11][i] ^ roundKey[12][i + 4];
    }

}
void AES256_KeySchedule_2(uint8_t key[], uint8_t roundKey[][16])
{
    
	for (int j = 0; j < 16; j++) //word 0~3
		roundKey[0][j] = key[j];
    for (int j = 0; j < 16; j++) //word 4~7
		roundKey[1][j] = key[j + 16];
    
    // word 8
	roundKey[2][0] = roundKey[0][0] ^ round_Constant[0] ^ Sbox[roundKey[1][13]];//G func
	roundKey[2][1] = roundKey[0][1] ^ Sbox[roundKey[1][14]];
	roundKey[2][2] = roundKey[0][2] ^ Sbox[roundKey[1][15]];
	roundKey[2][3] = roundKey[0][3] ^ Sbox[roundKey[1][12]];
    //word 9~11
	for (int i = 0; i < 12; i++)
		roundKey[2][i + 4] = roundKey[2][i] ^ roundKey[0][i + 4];
    // word 12
    roundKey[3][0] = roundKey[1][0] ^ Sbox[roundKey[2][12]];
    roundKey[3][1] = roundKey[1][1] ^ Sbox[roundKey[2][13]];
    roundKey[3][2] = roundKey[1][2] ^ Sbox[roundKey[2][14]];
    roundKey[3][3] = roundKey[1][3] ^ Sbox[roundKey[2][15]];
    // word 13~15
	for (int i = 0; i < 12; i++)
		roundKey[3][i + 4] = roundKey[3][i] ^ roundKey[1][i + 4];
    
    // word 16
    roundKey[4][0] = roundKey[2][0] ^ round_Constant[1] ^ Sbox[roundKey[3][13]];//G func
	roundKey[4][1] = roundKey[2][1] ^ Sbox[roundKey[3][14]];
	roundKey[4][2] = roundKey[2][2] ^ Sbox[roundKey[3][15]];
	roundKey[4][3] = roundKey[2][3] ^ Sbox[roundKey[3][12]];
    // word 17~19
	for (int i = 0; i < 12; i++)
		roundKey[4][i + 4] = roundKey[4][i] ^ roundKey[2][i + 4];
    // word 20
    roundKey[5][0] = roundKey[3][0] ^ Sbox[roundKey[4][12]];
    roundKey[5][1] = roundKey[3][1] ^ Sbox[roundKey[4][13]];
    roundKey[5][2] = roundKey[3][2] ^ Sbox[roundKey[4][14]];
    roundKey[5][3] = roundKey[3][3] ^ Sbox[roundKey[4][15]];
    // word 21~23
    for (int i = 0; i < 12; i++)
		roundKey[5][i + 4] = roundKey[5][i] ^ roundKey[3][i + 4];
    
    // word 24
    roundKey[6][0] = roundKey[4][0] ^ round_Constant[2] ^ Sbox[roundKey[5][13]];//G func
	roundKey[6][1] = roundKey[4][1] ^ Sbox[roundKey[5][14]];
	roundKey[6][2] = roundKey[4][2] ^ Sbox[roundKey[5][15]];
	roundKey[6][3] = roundKey[4][3] ^ Sbox[roundKey[5][12]];
    // word 25~27
	for (int i = 0; i < 12; i++)
		roundKey[6][i + 4] = roundKey[6][i] ^ roundKey[4][i + 4];
    // word 28
    roundKey[7][0] = roundKey[5][0] ^ Sbox[roundKey[6][12]];
    roundKey[7][1] = roundKey[5][1] ^ Sbox[roundKey[6][13]];
    roundKey[7][2] = roundKey[5][2] ^ Sbox[roundKey[6][14]];
    roundKey[7][3] = roundKey[5][3] ^ Sbox[roundKey[6][15]];
    // word 29~31
    for (int i = 0; i < 12; i++)
		roundKey[7][i + 4] = roundKey[7][i] ^ roundKey[5][i + 4];
    
    // word 32
    roundKey[8][0] = roundKey[6][0] ^ round_Constant[3] ^ Sbox[roundKey[7][13]];//G func
	roundKey[8][1] = roundKey[6][1] ^ Sbox[roundKey[7][14]];
	roundKey[8][2] = roundKey[6][2] ^ Sbox[roundKey[7][15]];
	roundKey[8][3] = roundKey[6][3] ^ Sbox[roundKey[7][12]];
    // word 33~35
	for (int i = 0; i < 12; i++)
		roundKey[8][i + 4] = roundKey[8][i] ^ roundKey[6][i + 4];
    // word 36
    roundKey[9][0] = roundKey[7][0] ^ Sbox[roundKey[8][12]];
    roundKey[9][1] = roundKey[7][1] ^ Sbox[roundKey[8][13]];
    roundKey[9][2] = roundKey[7][2] ^ Sbox[roundKey[8][14]];
    roundKey[9][3] = roundKey[7][3] ^ Sbox[roundKey[8][15]];
    // word 37~39
    for (int i = 0; i < 12; i++)
		roundKey[9][i + 4] = roundKey[9][i] ^ roundKey[7][i + 4];
    
    // word 40
    roundKey[10][0] = roundKey[8][0] ^ round_Constant[4] ^ Sbox[roundKey[9][13]];//G func
	roundKey[10][1] = roundKey[8][1] ^ Sbox[roundKey[9][14]];
	roundKey[10][2] = roundKey[8][2] ^ Sbox[roundKey[9][15]];
	roundKey[10][3] = roundKey[8][3] ^ Sbox[roundKey[9][12]];
    // word 41~43
	for (int i = 0; i < 12; i++)
		roundKey[10][i + 4] = roundKey[10][i] ^ roundKey[8][i + 4];
    // word 44
    roundKey[11][0] = roundKey[9][0] ^ Sbox[roundKey[10][12]];
    roundKey[11][1] = roundKey[9][1] ^ Sbox[roundKey[10][13]];
    roundKey[11][2] = roundKey[9][2] ^ Sbox[roundKey[10][14]];
    roundKey[11][3] = roundKey[9][3] ^ Sbox[roundKey[10][15]];
    // word 45~47
    for (int i = 0; i < 12; i++)
		roundKey[11][i + 4] = roundKey[11][i] ^ roundKey[9][i + 4];
    
    // word 48
    roundKey[12][0] = roundKey[10][0] ^ round_Constant[5] ^ Sbox[roundKey[11][13]];//G func
	roundKey[12][1] = roundKey[10][1] ^ Sbox[roundKey[11][14]];
	roundKey[12][2] = roundKey[10][2] ^ Sbox[roundKey[11][15]];
	roundKey[12][3] = roundKey[10][3] ^ Sbox[roundKey[11][12]];
    // word 49~51
	for (int i = 0; i < 12; i++)
		roundKey[12][i + 4] = roundKey[12][i] ^ roundKey[10][i + 4];
    // word 52
    roundKey[13][0] = roundKey[11][0] ^ Sbox[roundKey[12][12]];
    roundKey[13][1] = roundKey[11][1] ^ Sbox[roundKey[12][13]];
    roundKey[13][2] = roundKey[11][2] ^ Sbox[roundKey[12][14]];
    roundKey[13][3] = roundKey[11][3] ^ Sbox[roundKey[12][15]];
    // word 53~55
    for (int i = 0; i < 12; i++)
		roundKey[13][i + 4] = roundKey[13][i] ^ roundKey[11][i + 4];
    
    // word 56
    roundKey[14][0] = roundKey[12][0] ^ round_Constant[6] ^ Sbox[roundKey[13][13]];//G func
	roundKey[14][1] = roundKey[12][1] ^ Sbox[roundKey[13][14]];
	roundKey[14][2] = roundKey[12][2] ^ Sbox[roundKey[13][15]];
	roundKey[14][3] = roundKey[12][3] ^ Sbox[roundKey[13][12]];
    // word 57~59
	for (int i = 0; i < 12; i++)
		roundKey[14][i + 4] = roundKey[14][i] ^ roundKey[12][i + 4];
}

int main()
{
    uint8_t rk[15][16] = {0,};
    AES256_KeySchedule_2(key256, rk);

    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            if (j != 0 && j % 4 == 0)
                printf(" ");
            printf("%02x", rk[i][j]);
        }
        printf("\n");
    }
}