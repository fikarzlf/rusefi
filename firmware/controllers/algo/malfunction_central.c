/**
 * @file malfunction_central.c
 * @brief This data structure holds current malfunction codes
 *
 * @date Dec 20, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "malfunction_central.h"

static error_codes_set_s error_codes_set;

/**
 * @return -1 if code not found
 */
static int find_position(obd_code_e e_code)							// Search if code is present 
{
	// cycle for searching element equal seaching code
	for (int t = 0; t < error_codes_set.count; t++)
		if (error_codes_set.error_codes[t] == e_code)
			return t;			// we found position where this code is present
	return -1;														// -1 if code not found
}

void initMalfunctionCentral(void) {
	error_codes_set.count = 0;
}

void addError(obd_code_e errorCode) {
	if (error_codes_set.count < MAX_ERROR_CODES_COUNT && find_position(errorCode) == -1) {
		error_codes_set.error_codes[error_codes_set.count] = errorCode;
		error_codes_set.count++;
	}
}

void removeError(obd_code_e errorCode) {
	int pos = find_position(errorCode);
	if (pos >= 0) {
		for (int t = pos; t < error_codes_set.count; t++) 					// shift all right elements to one pos left
			error_codes_set.error_codes[t] = error_codes_set.error_codes[t + 1];
	error_codes_set.error_codes[--error_codes_set.count] = (obd_code_e)0;				// place 0
	}
}

void setError(int flag, obd_code_e errorCode) {
	if (flag)
		addError(errorCode);
	else
		removeError(errorCode);
}

void getErrorCodes(error_codes_set_s * copy) {
	copy->count = error_codes_set.count;
	for (int i = 0; i < copy->count; i++)
		copy->error_codes[i] = error_codes_set.error_codes[i];
}

bool hasErrorCodes(void) {
	return error_codes_set.count > 0;
}
