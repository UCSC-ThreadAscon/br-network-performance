#pragma once

/**
 * The statement to print when a CoAP request is received is as follows:
 * 
 *    "Received [uint32_t] bytes from [IPv6 address].".
 *
 * The substring "Received " is 9 bytes.
 * The substring " bytes from " is 12 bytes.
 * The substring "." is 1 byte.
 *
 * The 32 bit integer will be represented as a string.
 * 2^32 - 1 = 4294967295 has 10 digits, and thus, the
 * string representation of the uint32_t will make up 10 bytes.
 *
 * Furthermore, an IPv6 string representation is made up of 16 bytes.
 *
 * Thus, the total string size is  9 + 12 + 1 + 10 + 40 = 72 bytes.
*/
#define PRINT_STATEMENT_SIZE 72