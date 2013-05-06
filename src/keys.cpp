/**
 * Copyright (c) 2013, Dan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "keys.h"

bool Keys::keyboard[MAX_KEYS], Keys::old_keyboard[MAX_KEYS];

bool Keys::is_logged(int key) {
	return (key != VK_LBUTTON) && (key != VK_RBUTTON) && (key != VK_MBUTTON) && (key != VK_SHIFT) &&
		(key != VK_LSHIFT) && (key != VK_RSHIFT) && (key != VK_LCONTROL) && (key != VK_RCONTROL)  && (key != VK_LMENU) && (key != VK_RMENU);
}

bool Keys::is_special(int key) {
	return (key == VK_CONTROL) || (key == VK_MENU);
}

int Keys::get_name(int key, char *dest, int len) {
	// TODO: Use len parameter.
	memset(dest, 0, sizeof(char) * len);
	if ((key == VK_LBUTTON) || (key == VK_RBUTTON) || (key == VK_MBUTTON)) {
		sprintf(dest, "[MOUSE%d]", key);
	} else if (key == VK_BACK) {
		strcpy(dest, "[BCK]");
	} else if (key == VK_RETURN) {
		strcpy(dest, "[RET]");
	} else if (key == VK_SHIFT) {
		strcpy(dest, "[SHF]");
	} else if (key == VK_CONTROL) {
		strcpy(dest, "[CTRL]");
	} else if (key == VK_MENU) {
		strcpy(dest, "[ALT]");
	} else if (key == VK_CAPITAL) {
		strcpy(dest, "[CAPS]");
	} else if (key == VK_ESCAPE) {
		strcpy(dest, "[ESC]");
	} else if (key == VK_SPACE) {
		strcpy(dest, "[SPC]");
	} else if (key == VK_LEFT) {
		strcpy(dest, "[LEFT]");
	} else if (key == VK_UP) {
		strcpy(dest, "[UP]");
	} else if (key == VK_RIGHT) {
		strcpy(dest, "[RIGHT]");
	} else if (key == VK_DOWN) {
		strcpy(dest, "[DOWN]");
	} else if ((0x30 <= key) && (key <= 0x39)) { // 0-9
		sprintf(dest, "%d", key - 0x30);
	} else if ((0x41 <= key) && (key <= 0x5A)) { // A-Z
		bool isUpper = GetKeyState(VK_CAPITAL) ? !GetAsyncKeyState(VK_SHIFT) : !!GetAsyncKeyState(VK_SHIFT);
		if (isUpper) {
			sprintf(dest, "%c", 'A' - 0x41 + key);
		} else {
			sprintf(dest, "%c", 'a' - 0x41 + key);
		}
	} else if ((VK_NUMPAD0 <= key) && (key <= VK_NUMPAD9)) {
		sprintf(dest, "[NUM%d]", key - VK_NUMPAD0);
	} else {
		sprintf(dest, "[KEY:%d]", key);
	}
	return len;
}

int Keys::get_names(bool *keys, char *dest, int len) {
	// TODO: Use len parameter.
	memset(dest, 0, sizeof(char) * MAX_KEY_NAME);
	strcat(dest, "<");
	char *key_name = (char*) malloc(sizeof(char) * MAX_KEY_NAME);
	int j;
	for (j = MAX_KEYS - 1; !keys[j] && j != 0; --j);
	for (int i = 0; i != j; ++i) {
		if (keys[i]) {
			get_name(i, key_name, MAX_KEY_NAME);
			strcat(dest, key_name);
			strcat(dest, " + ");
		}
	}
	get_name(j, key_name, MAX_KEY_NAME);
	strcat(dest, key_name);
	free(key_name);
	strcat(dest, ">");
	return len;
}

bool *Keys::get_keyboard() {
	for (int i = 0; i != MAX_KEYS; ++i) {
		if (is_logged(i)) {
			keyboard[i] = GetAsyncKeyState(i) & 0x8000 ? true : false;
		}
	}
	return keyboard;
}

bool *Keys::get_old_keyboard() {
	return old_keyboard;
}

bool Keys::was_updated() {
	for (int i = 0; i != MAX_KEYS; ++i) {
		if (keyboard[i] != old_keyboard[i]) {
			return true;
		}
	}
	return false;
}

void Keys::update_keyboard() {
	memcpy(old_keyboard, keyboard, sizeof(bool) * MAX_KEYS);
}
