#pragma once

namespace mk::input {
	// Keyboard keys
	enum class KEY {
		// misc
		SPACE         = ' ',
		APOSTROPHE    = '\'',
		COMMA         = ',',
		MINUS         = '-',
		PERIOD        = '.',
		SLASH         = '/',
		SEMICOLON     = ';',
		EQUAL         = '=',
		LEFT_BRACKET  = '[',
		BACKSLASH     = '\\',
		RIGHT_BRACKET = ']',
		// Digits
		NUM_0 = '0',
		NUM_1 = '1',
		NUM_2 = '2',
		NUM_3 = '3',
		NUM_4 = '4',
		NUM_5 = '5',
		NUM_6 = '6',
		NUM_7 = '7',
		NUM_8 = '8',
		NUM_9 = '9',
		// Letters
		A = 'A',
		B = 'B',
		C = 'C',
		D = 'D',
		E = 'E',
		F = 'F',
		G = 'G',
		H = 'H',
		I = 'I',
		J = 'J',
		K = 'K',
		L = 'L',
		M = 'M',
		N = 'N',
		O = 'O',
		P = 'P',
		Q = 'Q',
		R = 'R',
		S = 'S',
		T = 'T',
		U = 'U',
		V = 'V',
		W = 'W',
		X = 'X',
		Y = 'Y',
		Z = 'Z'
	};
	// Mouse buttons
	enum class MOUSE {
		BUTTON_1 = 0,  // LEFT
		BUTTON_2,      // RIGHT
		BUTTON_3,      // MIDDLE
		BUTTON_4,
		BUTTON_5,
		BUTTON_6,
		BUTTON_7,
		BUTTON_8,  // LAST
	};
	constexpr MOUSE MOUSE_LEFT   = MOUSE::BUTTON_1;
	constexpr MOUSE MOUSE_RIGHT  = MOUSE::BUTTON_2;
	constexpr MOUSE MOUSE_MIDDLE = MOUSE::BUTTON_3;
}
