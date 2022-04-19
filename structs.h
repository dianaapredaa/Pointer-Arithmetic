// Copyright 2021 - 2022 Preda Diana 314CA

#ifndef STRUCTS__H_
#define STRUCTS_H_

typedef struct head {
	unsigned char type;
	unsigned int len;
} head;

typedef struct data_structure {
	head *header;
	void *data;
} data_structure;

#endif  // STRUCTS_H_