#pragma once

namespace mk::input {
	// Keyboard keys
	enum class KEY {
		// Note: Codes should match GLFW's key IDs:
		// https://www.glfw.org/docs/3.3/group__keys.html

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
		Z = 'Z',

		GRAVE     = '`',
		ESCAPE    = 256,
		ENTER     = 257,
		TAB       = 258,
		BACKSPACE = 259,
		INSERT    = 260,
		DELETE    = 261,

		// Arrow keys
		ARROW_RIGHT = 262,
		ARROW_LEFT  = 263,
		ARROW_DOWN  = 264,
		ARROW_UP    = 265,
		// Mods
		LEFT_SHIFT    = 340,
		LEFT_CONTROL  = 341,
		LEFT_ALT      = 342,
		LEFT_SUPER    = 343,
		RIGHT_SHIFT   = 344,
		RIGHT_CONTROL = 345,
		RIGHT_ALT     = 346,
		RIGHT_SUPER   = 347,

		// GUARDS
		_FIRST = SPACE,
		_LAST  = RIGHT_SUPER
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
