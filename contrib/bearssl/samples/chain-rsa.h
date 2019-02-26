/*
 * Copyright (c) 2016 Thomas Pornin <pornin@bolet.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining 
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be 
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "bearssl.h"

/*
 * A sample server certificate chain with a single intermediate CA.
 * Certificate key type: RSA
 * Signing algorithm for both certificates: RSA
 */

static const unsigned char CERT0[] = {
	0x30, 0x82, 0x03, 0x3C, 0x30, 0x82, 0x02, 0x24, 0xA0, 0x03, 0x02, 0x01,
	0x02, 0x02, 0x14, 0x58, 0xDA, 0xBA, 0x36, 0xCD, 0xED, 0xA0, 0xDA, 0x5C,
	0x10, 0x33, 0x73, 0x8E, 0x0C, 0x64, 0x92, 0x79, 0x92, 0xAF, 0x03, 0x30,
	0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0B,
	0x05, 0x00, 0x30, 0x27, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04,
	0x06, 0x13, 0x02, 0x43, 0x41, 0x31, 0x18, 0x30, 0x16, 0x06, 0x03, 0x55,
	0x04, 0x03, 0x13, 0x0F, 0x49, 0x6E, 0x74, 0x65, 0x72, 0x6D, 0x65, 0x64,
	0x69, 0x61, 0x74, 0x65, 0x20, 0x43, 0x41, 0x30, 0x1E, 0x17, 0x0D, 0x31,
	0x30, 0x30, 0x31, 0x30, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x5A,
	0x17, 0x0D, 0x33, 0x37, 0x31, 0x32, 0x33, 0x31, 0x32, 0x33, 0x35, 0x39,
	0x35, 0x39, 0x5A, 0x30, 0x21, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55,
	0x04, 0x06, 0x13, 0x02, 0x43, 0x41, 0x31, 0x12, 0x30, 0x10, 0x06, 0x03,
	0x55, 0x04, 0x03, 0x13, 0x09, 0x6C, 0x6F, 0x63, 0x61, 0x6C, 0x68, 0x6F,
	0x73, 0x74, 0x30, 0x82, 0x01, 0x22, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86,
	0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x82, 0x01,
	0x0F, 0x00, 0x30, 0x82, 0x01, 0x0A, 0x02, 0x82, 0x01, 0x01, 0x00, 0xD4,
	0x7A, 0x1D, 0x27, 0xBA, 0x2B, 0x3A, 0x67, 0xB2, 0x91, 0x6A, 0xFB, 0xE7,
	0x83, 0x44, 0xCA, 0xED, 0x1C, 0x75, 0xAD, 0xDD, 0x4D, 0x83, 0x62, 0xD6,
	0xAA, 0x68, 0x95, 0xB2, 0x24, 0x21, 0x7B, 0x15, 0xAE, 0x2A, 0x99, 0x68,
	0x15, 0xED, 0x66, 0xF0, 0xB8, 0x58, 0xE7, 0xD3, 0xF5, 0x2E, 0xC6, 0xD9,
	0x2A, 0x5E, 0xE7, 0x0E, 0x2E, 0xE7, 0xFC, 0x67, 0x59, 0xC0, 0xC8, 0x61,
	0x7D, 0x4B, 0xA4, 0x6F, 0xDD, 0x9F, 0xD9, 0xC8, 0x85, 0x87, 0x64, 0xC7,
	0xBA, 0x1A, 0x0F, 0x29, 0xD4, 0x96, 0xA8, 0x78, 0x9A, 0x6B, 0x62, 0x20,
	0xA9, 0x32, 0xD0, 0xEE, 0xA9, 0x8C, 0x28, 0x61, 0x47, 0xA2, 0x50, 0x2A,
	0x63, 0xF6, 0x21, 0xDE, 0xDA, 0xD8, 0xD5, 0xF0, 0x7F, 0xC5, 0x00, 0x82,
	0x70, 0xE6, 0xA3, 0xBF, 0x5C, 0x89, 0x27, 0x4F, 0x51, 0x92, 0x77, 0x03,
	0xC3, 0xB0, 0xCC, 0x2E, 0x3B, 0xEC, 0x23, 0xF2, 0x2F, 0x53, 0x41, 0xAF,
	0x89, 0x93, 0xFF, 0xD2, 0x80, 0xB1, 0x43, 0x97, 0xDE, 0xD6, 0x19, 0xA0,
	0x92, 0x12, 0x7A, 0x3D, 0x66, 0x79, 0xE1, 0xC1, 0xBC, 0xE1, 0x77, 0x70,
	0xA2, 0x8B, 0x3D, 0x46, 0x84, 0x53, 0x3F, 0xE4, 0x4E, 0x42, 0x41, 0x37,
	0x92, 0x1E, 0x1F, 0xFD, 0x38, 0xB3, 0xF7, 0xEF, 0x87, 0x39, 0x80, 0xD3,
	0x56, 0xCF, 0xF4, 0xE0, 0x13, 0xDE, 0x64, 0xB0, 0x72, 0xA4, 0x03, 0x84,
	0xC4, 0x41, 0xED, 0x6F, 0xFA, 0x3E, 0xE2, 0xCA, 0x04, 0x20, 0xD2, 0xD7,
	0xDC, 0x2C, 0x82, 0x2B, 0x7A, 0xE2, 0x6D, 0xA1, 0x1C, 0x48, 0xDB, 0xCF,
	0x89, 0x4F, 0x34, 0x97, 0x3D, 0x28, 0xA8, 0x53, 0xDA, 0xE7, 0xC1, 0xE1,
	0x73, 0x15, 0xA3, 0x30, 0x76, 0x7F, 0x8F, 0x23, 0x42, 0x14, 0x3D, 0x51,
	0x34, 0xD2, 0x5A, 0xAD, 0x3C, 0x9B, 0xCB, 0xC8, 0xFE, 0x7F, 0x6E, 0x8E,
	0x40, 0xF3, 0xBD, 0x02, 0x03, 0x01, 0x00, 0x01, 0xA3, 0x66, 0x30, 0x64,
	0x30, 0x1F, 0x06, 0x03, 0x55, 0x1D, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80,
	0x14, 0xC5, 0x01, 0xAD, 0x7C, 0xE6, 0x63, 0xC9, 0x9C, 0x17, 0x5F, 0xC5,
	0xA2, 0xA7, 0xE1, 0x50, 0x5F, 0xE8, 0xF5, 0xA8, 0xC6, 0x30, 0x1D, 0x06,
	0x03, 0x55, 0x1D, 0x0E, 0x04, 0x16, 0x04, 0x14, 0xC5, 0x01, 0xAD, 0x7C,
	0xE6, 0x63, 0xC9, 0x9C, 0x17, 0x5F, 0xC5, 0xA2, 0xA7, 0xE1, 0x50, 0x5F,
	0xE8, 0xF5, 0xA8, 0xC6, 0x30, 0x0C, 0x06, 0x03, 0x55, 0x1D, 0x13, 0x01,
	0x01, 0xFF, 0x04, 0x02, 0x30, 0x00, 0x30, 0x14, 0x06, 0x03, 0x55, 0x1D,
	0x11, 0x04, 0x0D, 0x30, 0x0B, 0x82, 0x09, 0x6C, 0x6F, 0x63, 0x61, 0x6C,
	0x68, 0x6F, 0x73, 0x74, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86,
	0xF7, 0x0D, 0x01, 0x01, 0x0B, 0x05, 0x00, 0x03, 0x82, 0x01, 0x01, 0x00,
	0x71, 0xB3, 0x5D, 0x21, 0xC2, 0x0E, 0xD7, 0xD0, 0xC6, 0xF8, 0x4A, 0x73,
	0x87, 0x4D, 0x22, 0x02, 0x27, 0xD0, 0xC7, 0xF8, 0xFD, 0x1A, 0x6D, 0x62,
	0x58, 0x82, 0x08, 0x76, 0x20, 0x07, 0xC0, 0x48, 0x92, 0xF3, 0xE9, 0x98,
	0x5A, 0xEB, 0x0D, 0x4E, 0x56, 0x95, 0x63, 0x68, 0x4D, 0xAB, 0xD4, 0x00,
	0x76, 0xE0, 0x37, 0xCF, 0x0B, 0x11, 0x61, 0x59, 0x5C, 0xDE, 0xF6, 0xA5,
	0x14, 0x59, 0xD9, 0x25, 0x9F, 0x59, 0xD9, 0xD3, 0x5E, 0x86, 0xAC, 0x1D,
	0xA6, 0x2C, 0x32, 0x42, 0x19, 0x32, 0x13, 0x40, 0x0B, 0x54, 0xCD, 0x67,
	0x26, 0xB6, 0xBD, 0xB4, 0x96, 0xA4, 0xCA, 0x1F, 0x7F, 0x37, 0xD6, 0xA8,
	0x75, 0xEB, 0x3A, 0x81, 0x51, 0x30, 0xB9, 0xF9, 0x4A, 0x01, 0x6F, 0xD1,
	0xD6, 0xED, 0x4F, 0xDF, 0x3F, 0x30, 0x60, 0x06, 0x67, 0x92, 0x8E, 0x61,
	0x85, 0x5F, 0x1A, 0xB5, 0x8C, 0xB3, 0x0F, 0x61, 0xA9, 0xFA, 0xDF, 0x5D,
	0xC4, 0x64, 0x00, 0xEA, 0x87, 0xB1, 0x33, 0x5F, 0x7D, 0xCB, 0xA3, 0x85,
	0x24, 0x6E, 0x2C, 0x65, 0x3B, 0xEA, 0x73, 0x3F, 0x86, 0xD3, 0xFD, 0xE8,
	0xE4, 0x02, 0xC6, 0x61, 0x6A, 0x26, 0x17, 0x52, 0x01, 0x25, 0x5C, 0x7F,
	0xA8, 0xCE, 0x69, 0x1F, 0xAC, 0x61, 0x2C, 0xB7, 0x04, 0xAF, 0xFD, 0xA4,
	0x7A, 0x99, 0xCB, 0x26, 0x15, 0x4C, 0xFF, 0x74, 0xD4, 0x73, 0x0C, 0x57,
	0x0C, 0x26, 0xEB, 0xD7, 0x2A, 0xDC, 0x5C, 0xC3, 0x41, 0xBB, 0xC2, 0xF3,
	0xCE, 0x16, 0xBD, 0x8E, 0x7B, 0xFB, 0xE9, 0xDF, 0xAB, 0x21, 0x0D, 0x91,
	0xB3, 0x8D, 0xD8, 0xCF, 0xC8, 0xF4, 0x49, 0xB9, 0xD0, 0xE3, 0x16, 0x70,
	0x3F, 0xB3, 0xDE, 0xD4, 0x07, 0x25, 0xFA, 0x46, 0x44, 0x52, 0x89, 0x4D,
	0x89, 0xC8, 0xE2, 0xCA, 0xB5, 0x33, 0x7F, 0xC7, 0x21, 0xBD, 0x28, 0xEF,
	0xFE, 0x66, 0x74, 0x03
};

static const unsigned char CERT1[] = {
	0x30, 0x82, 0x03, 0x34, 0x30, 0x82, 0x02, 0x1C, 0xA0, 0x03, 0x02, 0x01,
	0x02, 0x02, 0x14, 0x70, 0x0F, 0x60, 0xEE, 0xF0, 0x07, 0x9A, 0x9C, 0x69,
	0xAC, 0x97, 0x62, 0x26, 0x4F, 0x5D, 0x05, 0xB6, 0xF9, 0x8F, 0x48, 0x30,
	0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0B,
	0x05, 0x00, 0x30, 0x1C, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04,
	0x06, 0x13, 0x02, 0x43, 0x41, 0x31, 0x0D, 0x30, 0x0B, 0x06, 0x03, 0x55,
	0x04, 0x03, 0x13, 0x04, 0x52, 0x6F, 0x6F, 0x74, 0x30, 0x1E, 0x17, 0x0D,
	0x31, 0x30, 0x30, 0x31, 0x30, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x5A, 0x17, 0x0D, 0x33, 0x37, 0x31, 0x32, 0x33, 0x31, 0x32, 0x33, 0x35,
	0x39, 0x35, 0x39, 0x5A, 0x30, 0x27, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03,
	0x55, 0x04, 0x06, 0x13, 0x02, 0x43, 0x41, 0x31, 0x18, 0x30, 0x16, 0x06,
	0x03, 0x55, 0x04, 0x03, 0x13, 0x0F, 0x49, 0x6E, 0x74, 0x65, 0x72, 0x6D,
	0x65, 0x64, 0x69, 0x61, 0x74, 0x65, 0x20, 0x43, 0x41, 0x30, 0x82, 0x01,
	0x22, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01,
	0x01, 0x01, 0x05, 0x00, 0x03, 0x82, 0x01, 0x0F, 0x00, 0x30, 0x82, 0x01,
	0x0A, 0x02, 0x82, 0x01, 0x01, 0x00, 0xB3, 0xE8, 0x6B, 0xAF, 0x9C, 0x16,
	0x52, 0xE3, 0x81, 0x0C, 0x50, 0xAB, 0x25, 0xCE, 0xCC, 0x0D, 0xC7, 0xF2,
	0x1F, 0x7F, 0x50, 0xDF, 0x2C, 0x5C, 0x35, 0xD6, 0x62, 0x2E, 0x63, 0x27,
	0x41, 0xA7, 0xE4, 0x53, 0xA8, 0x4B, 0x27, 0xFA, 0x13, 0x91, 0xA3, 0xFA,
	0x09, 0x4A, 0x2F, 0x3B, 0x5E, 0xCF, 0x77, 0xB3, 0x8A, 0xC1, 0xCD, 0x49,
	0x95, 0x9C, 0x75, 0x0D, 0x64, 0x74, 0xEF, 0xE4, 0xD7, 0x4B, 0xB9, 0xA1,
	0x9B, 0x68, 0xD2, 0x30, 0x71, 0x48, 0xEA, 0xF7, 0x4B, 0x14, 0xDF, 0x3F,
	0x47, 0xA9, 0xD8, 0xBB, 0xEC, 0x8F, 0x28, 0xCC, 0xFA, 0xDF, 0xB4, 0x1F,
	0x94, 0x7C, 0x96, 0xFC, 0x08, 0x05, 0x28, 0xF9, 0xE8, 0xF4, 0x2F, 0x2F,
	0xEE, 0x62, 0x9C, 0x8A, 0x3A, 0xE0, 0x85, 0x58, 0x60, 0xB6, 0x0F, 0x2D,
	0x30, 0xB4, 0xC0, 0x41, 0x54, 0x91, 0x4C, 0x1F, 0x5F, 0xAD, 0xF1, 0x19,
	0xF0, 0xC0, 0x22, 0xA6, 0x7D, 0xD8, 0x3F, 0x79, 0x34, 0x59, 0x42, 0x7B,
	0x5B, 0xB5, 0x41, 0xC4, 0x64, 0x7F, 0x52, 0xCF, 0x3C, 0x37, 0x22, 0xA1,
	0x2F, 0x79, 0x25, 0x94, 0x24, 0x41, 0xC2, 0x3F, 0xFA, 0xC7, 0x75, 0xFB,
	0x48, 0xB5, 0x0D, 0x18, 0xA7, 0xF4, 0x54, 0xF3, 0x2E, 0x6E, 0xD8, 0x43,
	0x58, 0xC4, 0xAB, 0x50, 0xE8, 0x05, 0xAD, 0x91, 0xB6, 0x1E, 0x01, 0x75,
	0xB3, 0x54, 0x9C, 0xDE, 0xA0, 0x99, 0x15, 0xFB, 0xAC, 0xF1, 0x5C, 0x97,
	0x49, 0x51, 0xCC, 0xEF, 0x58, 0x12, 0x6F, 0x73, 0x6B, 0xB3, 0x34, 0x14,
	0x01, 0x0F, 0x5A, 0x9D, 0xFA, 0xAA, 0xD6, 0x93, 0xD3, 0xE2, 0xEA, 0xC3,
	0xAB, 0xBC, 0x4E, 0xED, 0xCC, 0x51, 0xA1, 0xB8, 0xF8, 0x94, 0xB6, 0xB4,
	0x2C, 0xA8, 0x86, 0x2B, 0x1F, 0xF6, 0x51, 0x43, 0x29, 0x52, 0x5E, 0x13,
	0x89, 0xB3, 0x6A, 0x78, 0x60, 0x4E, 0x4E, 0xC0, 0x1B, 0xA5, 0x02, 0x03,
	0x01, 0x00, 0x01, 0xA3, 0x63, 0x30, 0x61, 0x30, 0x1F, 0x06, 0x03, 0x55,
	0x1D, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80, 0x14, 0xC3, 0x0A, 0xBC, 0xB4,
	0x90, 0x63, 0x96, 0x92, 0x45, 0xBC, 0x27, 0xA0, 0xF0, 0x58, 0x89, 0x2A,
	0xD5, 0x78, 0x00, 0x12, 0x30, 0x1D, 0x06, 0x03, 0x55, 0x1D, 0x0E, 0x04,
	0x16, 0x04, 0x14, 0x7C, 0xCF, 0xA3, 0xC6, 0x29, 0xF7, 0xF3, 0xC5, 0xAA,
	0x19, 0xD0, 0xC0, 0x16, 0xEB, 0xE0, 0x40, 0x0F, 0xCE, 0x44, 0xA7, 0x30,
	0x0E, 0x06, 0x03, 0x55, 0x1D, 0x0F, 0x01, 0x01, 0xFF, 0x04, 0x04, 0x03,
	0x02, 0x00, 0x86, 0x30, 0x0F, 0x06, 0x03, 0x55, 0x1D, 0x13, 0x01, 0x01,
	0xFF, 0x04, 0x05, 0x30, 0x03, 0x01, 0x01, 0xFF, 0x30, 0x0D, 0x06, 0x09,
	0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0B, 0x05, 0x00, 0x03,
	0x82, 0x01, 0x01, 0x00, 0x54, 0x3B, 0xF4, 0xEA, 0xC6, 0xE4, 0xE8, 0xC0,
	0x5B, 0x12, 0xB2, 0xAD, 0xFA, 0xBD, 0xA9, 0xF4, 0x62, 0x03, 0xAD, 0x57,
	0x4C, 0x6F, 0x07, 0xA7, 0x47, 0x35, 0x75, 0x3E, 0x48, 0xA2, 0x5B, 0x71,
	0x10, 0x01, 0x8F, 0x93, 0x99, 0x9D, 0xA7, 0x47, 0xE3, 0x8E, 0x3E, 0xAF,
	0x2D, 0xBA, 0x06, 0xFE, 0xD4, 0xE4, 0x97, 0xBC, 0x10, 0x8D, 0xA6, 0xA5,
	0x3A, 0x3C, 0xE6, 0x21, 0xCB, 0x3B, 0xDA, 0x13, 0xE3, 0x4A, 0x23, 0xEB,
	0xBC, 0xBA, 0xA5, 0x90, 0x91, 0xAD, 0x55, 0x6E, 0xD5, 0xDD, 0x85, 0x4B,
	0x6B, 0x27, 0xD2, 0x39, 0x76, 0x06, 0x2E, 0xD4, 0x23, 0x5B, 0xBB, 0x55,
	0xB5, 0x49, 0x3F, 0x7D, 0x2F, 0x21, 0xCB, 0x96, 0x75, 0x19, 0x99, 0xA9,
	0x99, 0xF1, 0x0B, 0xBB, 0x3E, 0xC7, 0x10, 0xA7, 0x8B, 0x7A, 0x52, 0xB9,
	0x49, 0xE4, 0x14, 0x08, 0x78, 0xE7, 0x3D, 0xD9, 0x98, 0x57, 0xDA, 0xFF,
	0xDA, 0xB9, 0x8D, 0x66, 0x75, 0x80, 0xB5, 0x5D, 0xB9, 0xDF, 0x69, 0x65,
	0xDB, 0x16, 0xE7, 0x1E, 0xC4, 0x40, 0xE4, 0x46, 0x8A, 0x1E, 0x76, 0x32,
	0x16, 0xE7, 0x83, 0x67, 0xDA, 0x78, 0x56, 0xAB, 0x0F, 0x4C, 0x5C, 0xF1,
	0x70, 0xDA, 0x66, 0x20, 0xF4, 0xF8, 0x0F, 0xF6, 0xDC, 0x2F, 0x72, 0x97,
	0x6B, 0x78, 0x40, 0x79, 0x9D, 0x5B, 0x6D, 0xBE, 0x03, 0xCE, 0x70, 0x42,
	0xA8, 0x2A, 0x71, 0x11, 0xDF, 0x2E, 0x81, 0x01, 0xEA, 0x86, 0x12, 0x91,
	0x4F, 0xD0, 0x18, 0xED, 0x84, 0xA4, 0x48, 0x30, 0xB9, 0xB9, 0x7B, 0x56,
	0x3E, 0xEF, 0x8D, 0xE1, 0x1F, 0xFD, 0x90, 0xB2, 0x44, 0x85, 0x58, 0x6F,
	0xA7, 0x30, 0x74, 0x7C, 0xF1, 0xAE, 0x8C, 0x99, 0x3F, 0xF1, 0x4D, 0x57,
	0xC5, 0x95, 0x68, 0xC2, 0x36, 0xB2, 0xEC, 0xC2, 0x68, 0x7A, 0xC0, 0x81,
	0xA9, 0x55, 0x05, 0xE7, 0xCF, 0xAF, 0x50, 0xAC
};

static const br_x509_certificate CHAIN[] = {
	{ (unsigned char *)CERT0, sizeof CERT0 },
	{ (unsigned char *)CERT1, sizeof CERT1 }
};

#define CHAIN_LEN   2
