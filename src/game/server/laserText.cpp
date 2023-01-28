/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include "laserText.h"
#include <cstring>
#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>

static const bool T = true;
static const bool F = false;
static const bool asciiTable[256][5][3] = {
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 0
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 1
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 2
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 3
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 4
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 5
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 6
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 7
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 8
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 9
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 10
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 11
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 12
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 13
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 14
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 15
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 16
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 17
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 18
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 19
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 20
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 21
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 22
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 23
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 24
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 25
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 26
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 27
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 28
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 29
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 30
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 31
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 32
	{{F, T, F}, {F, T, F}, {F, T, F}, {F, F, F}, {F, T, F}}, // ascii 33 !
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 34
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 35
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 36
	{{T, F, T}, {T, F, F}, {F, T, F}, {F, F, T}, {T, F, T}}, // ascii 37 %
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 38
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 39
	{{F, T, F}, {T, F, F}, {T, F, F}, {T, F, F}, {F, T, F}}, // ascii 40 (
	{{F, T, F}, {F, F, T}, {F, F, T}, {F, F, T}, {F, T, F}}, // ascii 41 )
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 42
	{{F, F, F}, {F, T, F}, {T, T, T}, {F, T, F}, {F, F, F}}, // ascii 43 +
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, T, F}, {T, F, F}}, // ascii 44 ,
	{{F, F, F}, {F, F, F}, {T, T, T}, {F, F, F}, {F, F, F}}, // ascii 45 -
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, T, F}}, // ascii 46 .
	{{F, F, F}, {F, F, T}, {F, T, F}, {T, F, F}, {F, F, F}}, // ascii 47 /
	{{T, T, T}, {T, F, T}, {T, F, T}, {T, F, T}, {T, T, T}}, // 0
	{{F, T, F}, {T, T, F}, {F, T, F}, {F, T, F}, {F, T, F}}, // 1
	{{T, T, T}, {F, F, T}, {T, T, T}, {T, F, F}, {T, T, T}}, // 2
	{{T, T, T}, {F, F, T}, {T, T, T}, {F, F, T}, {T, T, T}}, // 3
	{{T, F, T}, {T, F, T}, {T, T, T}, {F, F, T}, {F, F, T}}, // 4
	{{T, T, T}, {T, F, F}, {T, T, T}, {F, F, T}, {T, T, T}}, // 5
	{{T, T, T}, {T, F, F}, {T, T, T}, {T, F, T}, {T, T, T}}, // 6
	{{T, T, T}, {F, F, T}, {F, F, T}, {F, F, T}, {F, F, T}}, // 7
	{{T, T, T}, {T, F, T}, {T, T, T}, {T, F, T}, {T, T, T}}, // 8
	{{T, T, T}, {T, F, T}, {T, T, T}, {F, F, T}, {T, T, T}}, // 9
	{{F, F, F}, {F, T, F}, {F, F, F}, {F, T, F}, {F, F, F}}, // :
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 59
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 60
	{{F, F, F}, {T, T, T}, {F, F, F}, {T, T, T}, {F, F, F}}, // ascii 61 =
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 62
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 63
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 64
	{{T, T, T}, {T, F, T}, {T, T, T}, {T, F, T}, {T, F, T}}, // A
	{{T, F, F}, {T, F, F}, {T, T, T}, {T, F, T}, {T, T, T}}, // B
	{{T, T, T}, {T, F, F}, {T, F, F}, {T, F, F}, {T, T, T}}, // C
	{{F, F, T}, {F, F, T}, {T, T, T}, {T, F, T}, {T, T, T}}, // D
	{{T, T, T}, {T, F, F}, {T, T, T}, {T, F, F}, {T, T, T}}, // E
	{{T, T, T}, {T, F, F}, {T, T, T}, {T, F, F}, {T, F, F}}, // F
	{{T, T, T}, {T, F, T}, {T, T, T}, {F, F, T}, {F, T, T}}, // G
	{{T, F, T}, {T, F, T}, {T, T, T}, {T, F, T}, {T, F, T}}, // H
	{{T, F, F}, {T, F, F}, {T, F, F}, {T, F, F}, {T, F, F}}, // I
	{{F, F, T}, {F, F, T}, {T, F, T}, {T, F, T}, {T, T, T}}, // J
	{{T, F, F}, {T, F, F}, {T, F, T}, {T, T, F}, {T, F, T}}, // K
	{{T, F, F}, {T, F, F}, {T, F, F}, {T, F, F}, {T, T, T}}, // L
	{{T, F, T}, {T, T, T}, {T, T, T}, {T, F, T}, {T, F, T}}, // M
	{{F, F, F}, {F, F, F}, {T, T, T}, {T, F, T}, {T, F, T}}, // N
	{{F, F, F}, {F, F, F}, {T, T, T}, {T, F, T}, {T, T, T}}, // O
	{{T, T, T}, {T, F, T}, {T, T, T}, {T, F, F}, {T, F, F}}, // P
	{{T, T, T}, {T, F, T}, {T, T, T}, {F, F, T}, {F, F, T}}, // Q
	{{F, F, F}, {T, F, T}, {T, T, F}, {T, F, F}, {T, F, F}}, // R
	{{F, T, T}, {T, F, F}, {F, T, F}, {F, F, T}, {T, T, F}}, // S
	{{T, T, T}, {F, T, F}, {F, T, F}, {F, T, F}, {F, T, F}}, // T
	{{F, F, F}, {F, F, F}, {T, F, T}, {T, F, T}, {T, T, T}}, // U
	{{F, F, F}, {F, F, F}, {T, F, T}, {T, T, T}, {F, T, F}}, // V
	{{T, F, T}, {T, F, T}, {T, T, T}, {T, T, T}, {T, T, T}}, // W
	{{F, F, F}, {F, F, F}, {T, F, T}, {F, T, F}, {T, F, T}}, // X
	{{T, F, T}, {T, F, T}, {T, T, T}, {F, T, F}, {F, T, F}}, // Y
	{{T, T, T}, {F, F, T}, {F, T, F}, {T, F, F}, {T, T, T}}, // Z
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 91
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 92
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 93
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 94
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 95
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 96
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 97
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 98
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 99
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 100
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 101
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 102
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 103
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 104
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 105
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 106
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 107
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 108
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 109
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 110
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 111
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 112
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 113
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 114
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 115
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 116
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 117
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 118
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 119
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 120
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 121
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 122
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 123
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 124
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 125
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 126
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 127
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 128
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 129
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 130
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 131
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 132
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 133
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 134
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 135
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 136
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 137
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 138
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 139
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 140
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 141
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 142
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 143
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 144
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 145
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 146
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 147
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 148
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 149
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 150
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 151
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 152
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 153
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 154
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 155
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 156
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 157
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 158
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 159
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 160
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 161
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 162
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 163
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 164
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 165
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 166
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 167
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 168
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 169
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 170
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 171
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 172
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 173
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 174
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 175
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 176
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 177
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 178
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 179
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 180
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 181
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 182
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 183
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 184
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 185
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 186
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 187
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 188
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 189
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 190
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 191
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 192
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 193
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 194
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 195
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 196
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 197
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 198
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 199
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 200
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 201
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 202
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 203
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 204
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 205
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 206
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 207
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 208
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 209
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 210
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 211
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 212
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 213
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 214
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 215
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 216
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 217
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 218
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 219
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 220
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 221
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 222
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 223
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 224
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 225
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 226
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 227
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 228
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 229
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 230
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 231
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 232
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 233
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 234
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 235
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 236
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 237
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 238
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 239
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 240
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 241
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 242
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 243
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 244
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 245
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 246
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 247
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 248
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 249
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 250
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 251
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 252
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 253
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}}, // ascii 254
	{{F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}, {F, F, F}} // ascii 255
};

CLaserText::CLaserText(CGameWorld *pGameWorld, vec2 Pos, int Owner, int pAliveTicks, char *pText, int pTextLen) :
	CEntity(pGameWorld, CGameWorld::ENTTYPE_LASER)
{
	m_Pos = Pos;
	m_Owner = Owner;
	GameWorld()->InsertEntity(this);

	m_CurTicks = Server()->Tick();
	m_StartTick = Server()->Tick();
	m_AliveTicks = pAliveTicks;

	m_TextLen = pTextLen;
	m_Text = new char[pTextLen];
	memcpy(m_Text, pText, pTextLen);

	m_CharNum = 0;

	for(int i = 0; i < m_TextLen; ++i)
	{
		for(int n = 0; n < 5; ++n)
		{
			for(int j = 0; j < 3; ++j)
			{
				if(asciiTable[(unsigned char)m_Text[i]][n][j])
				{
					++m_CharNum;
				}
			}
		}
	}

	m_Chars = new CLaserChar *[m_CharNum];

	m_PosOffsetCharPoints = 15.0;
	m_PosOffsetChars = m_PosOffsetCharPoints * 3.5;

	int charCount = 0;
	for(int i = 0; i < m_TextLen; ++i)
	{
		makeLaser(m_Text[i], i, charCount);
	}
}

CLaserText::CLaserText(CGameWorld *pGameWorld, vec2 Pos, int Owner, int pAliveTicks, char *pText, int pTextLen, float pCharPointOffset, float pCharOffsetFactor) :
	CEntity(pGameWorld, CGameWorld::ENTTYPE_LASER)
{
	m_Pos = Pos;
	m_Owner = Owner;
	GameWorld()->InsertEntity(this);

	m_CurTicks = Server()->Tick();
	m_StartTick = Server()->Tick();
	m_AliveTicks = pAliveTicks;

	m_TextLen = pTextLen;
	m_Text = new char[pTextLen];
	memcpy(m_Text, pText, pTextLen);

	m_CharNum = 0;

	for(int i = 0; i < m_TextLen; ++i)
	{
		for(int n = 0; n < 5; ++n)
		{
			for(int j = 0; j < 3; ++j)
			{
				if(asciiTable[(unsigned char)m_Text[i]][n][j])
				{
					++m_CharNum;
				}
			}
		}
	}

	m_Chars = new CLaserChar *[m_CharNum];

	m_PosOffsetCharPoints = pCharPointOffset;
	m_PosOffsetChars = m_PosOffsetCharPoints * pCharOffsetFactor;

	int charCount = 0;
	for(int i = 0; i < m_TextLen; ++i)
	{
		makeLaser(m_Text[i], i, charCount);
	}
}

void CLaserText::Reset()
{
	//	GameServer()->m_World.DestroyEntity(this);
	this->Destroy();
}

void CLaserText::Tick()
{
	//	if(++m_CurTicks - m_StartTick > m_AliveTicks) GameServer()->m_World.DestroyEntity(this);
	if(++m_CurTicks - m_StartTick > m_AliveTicks)
		Reset();
}

void CLaserText::TickPaused()
{
}

inline unsigned char NeighboursVert(const bool pCharVert[3], int pVertOff)
{
	unsigned char neighbours = 0;
	if(pVertOff > 0)
	{
		if(pCharVert[pVertOff - 1])
			++neighbours;
	}
	if(pVertOff < 2)
	{
		if(pCharVert[pVertOff + 1])
			++neighbours;
	}

	return neighbours;
}

inline unsigned char NeighboursHor(const bool pCharHor[5][3], int pHorOff, int pVertOff)
{
	unsigned char neighbours = 0;
	if(pHorOff > 0)
	{
		if(pCharHor[pHorOff - 1][pVertOff])
			++neighbours;
	}
	if(pHorOff < 4)
	{
		if(pCharHor[pHorOff + 1][pVertOff])
			++neighbours;
	}

	return neighbours;
}

void CLaserText::makeLaser(char pChar, int pCharOffset, int &charCount)
{
	unsigned short tail[5][3];
	unsigned char neighbourCount[5][3];

	for(int n = 0; n < 5; ++n)
	{
		for(int j = 0; j < 3; ++j)
		{
			if(asciiTable[(unsigned char)pChar][n][j])
			{
				neighbourCount[n][j] = 0;
				neighbourCount[n][j] += NeighboursVert(asciiTable[(unsigned char)pChar][n], j);
				neighbourCount[n][j] += NeighboursHor(asciiTable[(unsigned char)pChar], n, j);
				tail[n][j] = 0;
			}
			else
				tail[n][j] = (unsigned short)-1;
		}
	}

	for(int n = 0; n < 5; ++n)
	{
		for(int j = 0; j < 3; ++j)
		{
			if(asciiTable[(unsigned char)pChar][n][j])
			{
				//additional x, y offset to draw a line
				int x = j, y = n;
				unsigned char maxNeighbour = 0;
				//forced line draw
				bool forceLine = false;

				if(j > 0)
				{
					if(asciiTable[(unsigned char)pChar][n][j - 1])
					{
						if(tail[n][j - 1] != 0)
						{
							if(tail[n][j - 1] != (n << 8 | j))
							{
								forceLine = true;
								tail[n][j] = (n << 8 | (j - 1));
								x = j - 1;
								y = n;
							}
						}
						else if(neighbourCount[n][j - 1] > maxNeighbour)
						{
							maxNeighbour = neighbourCount[n][j - 1];
							x = j - 1;
							y = n;
						}
					}
				}
				if(!forceLine && j < 2)
				{
					if(asciiTable[(unsigned char)pChar][n][j + 1])
					{
						if(tail[n][j + 1] != 0)
						{
							if(tail[n][j + 1] != (n << 8 | j))
							{
								forceLine = true;
								tail[n][j] = (n << 8 | (j + 1));
								x = j + 1;
								y = n;
							}
						}
						else if(neighbourCount[n][j + 1] > maxNeighbour)
						{
							maxNeighbour = neighbourCount[n][j + 1];
							x = j + 1;
							y = n;
						}
					}
				}
				if(!forceLine && n > 0)
				{
					if(asciiTable[(unsigned char)pChar][n - 1][j])
					{
						if(tail[n - 1][j] != 0)
						{
							if(tail[n - 1][j] != (n << 8 | j))
							{
								forceLine = true;
								tail[n][j] = ((n - 1) << 8 | (j));
								x = j;
								y = n - 1;
							}
						}
						else if(neighbourCount[n - 1][j] > maxNeighbour)
						{
							maxNeighbour = neighbourCount[n - 1][j];
							x = j;
							y = n - 1;
						}
					}
				}
				if(!forceLine && n < 4)
				{
					if(asciiTable[(unsigned char)pChar][n + 1][j])
					{
						if(tail[n + 1][j] != 0)
						{
							if(tail[n + 1][j] != (n << 8 | j))
							{
								forceLine = true;
								tail[n][j] = ((n + 1) << 8 | (j));
								x = j;
								y = n + 1;
							}
						}
						else if(neighbourCount[n + 1][j] > maxNeighbour)
						{
							x = j;
							y = n + 1;
						}
					}
				}

				if(!forceLine)
				{
					tail[n][j] = (y << 8 | x);
				}

				CLaserChar *pObj = (m_Chars[charCount] = new CLaserChar(GameWorld()));

				pObj->m_Pos.x = m_Pos.x + pCharOffset * m_PosOffsetChars + j * m_PosOffsetCharPoints;
				pObj->m_Pos.y = m_Pos.y + n * m_PosOffsetCharPoints;
				pObj->m_Frompos.x = m_Pos.x + pCharOffset * m_PosOffsetChars + x * m_PosOffsetCharPoints;
				pObj->m_Frompos.y = m_Pos.y + y * m_PosOffsetCharPoints;

				++charCount;
			}
		}
	}
}

void CLaserText::Snap(int SnappingClient)
{
	if(NetworkClipped(SnappingClient))
		return;

	for(int i = 0; i < m_CharNum; ++i)
	{
		CNetObj_Laser *pObj = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_Chars[i]->getID(), sizeof(CNetObj_Laser)));
		if(!pObj)
			return;

		pObj->m_X = m_Chars[i]->m_Pos.x;
		pObj->m_Y = m_Chars[i]->m_Pos.y;
		pObj->m_FromX = m_Chars[i]->m_Frompos.x;
		pObj->m_FromY = m_Chars[i]->m_Frompos.y;
		pObj->m_StartTick = Server()->Tick();
	}
}
