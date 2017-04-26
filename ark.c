

U64 ClearFile[8] = 
{
	0xfefefefefefefefe, 0xfdfdfdfdfdfdfdfd, 0xfbfbfbfbfbfbfbfb,
	0xf7f7f7f7f7f7f7f7, 0xefefefefefefefef, 0xdfdfdfdfdfdfdfdf,
	0xbfbfbfbfbfbfbfbf, 0x7f7f7f7f7f7f7f7f
}; // when ANDed with another bitboard, clears the file

// left to right
U64 MaskFile[8] = 
{
	0x101010101010101, 0x202020202020202, 0x404040404040404, 
	0x808080808080808, 0x1010101010101010, 0x2020202020202020, 
	0x4040404040404040, 0x8080808080808080
};

U64 ClearRank[8] =
{
	0xffffffffffffff00, 0xffffffffffff00ff, 0xffffffffff00ffff,
	0xffffffff00ffffff, 0xffffff00ffffffff, 0xffff00ffffffffff,
	0xff00ffffffffffff, 0xffffffffffffff
};

// bottom to top
U64 MaskRank[8] =
{
	0xff, 0xff00, 0xff0000, 0xff000000, 0xff00000000,
	0xff0000000000, 0xff000000000000, 0xff00000000000000
};

	/* from top left to bottom right, like this slash: / */
U64 DiagonalMasks[15] = 
{
	0x100000000000000, 0x201000000000000, 0x402010000000000, 0x804020100000000, 
	0x1008040201000000, 0x2010080402010000, 0x4020100804020100, 0x8040201008040201,
	0x80402010080402, 0x804020100804, 0x8040201008, 0x80402010, 0x804020, 0x8040, 0x80
};

/* from bottom left to top right, like this backslash: \ */
U64 AntiDiagonalMasks[15] =
{
	0x1, 0x102, 0x10204, 0x1020408, 0x102040810, 0x10204081020, 0x1020408102040, 
	0x102040810204080, 0x204081020408000, 0x408102040800000, 0x810204080000000, 
	0x1020408000000000, 0x2040800000000000, 0x4080000000000000, 0x8000000000000000
};

