// Copyright 2021 - 2022 Preda Diana 314CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "structs.h"
#include "utils.h"

#define MAX_SIZE_STR 256

// calculate the number of dedications introduced
int data_structure_cnt (void *arr, int len)
{
	int cnt = 0;
	void *byte = arr;
	head *header = NULL;

	while (len > 0) {
		header = byte;
		len -= sizeof(head);
		len -= header->len;
		byte = byte + sizeof(head) + header->len;
		
		cnt++;
	}
	return cnt;
}

// populate data_structure with given info
data_structure* read (int data_type)
{
	data_structure *data;
	data = malloc(sizeof(data_structure));
	DIE(NULL == data, "malloc failed");

	data->header = malloc(sizeof(head));
	DIE(NULL == data->header, "malloc failed");

	char *name_first;
	char *name_second;

	int money01 = 0, money02 = 0;
	int total_len = 0;

	name_first = malloc(MAX_SIZE_STR);
	name_second = malloc(MAX_SIZE_STR);

	scanf("%s", name_first);
	scanf("%d", &money01);
	scanf("%d", &money02);
	scanf("%s", name_second);

	int str_len_1 = strlen(name_first) + 1;
	int str_len_2 = strlen(name_second) + 1;

	if (data_type == 1) {
		int int_len_1 = sizeof(int8_t);
		int int_len_2 = sizeof(int8_t);

		// calculate the number of bytes introduced
		total_len = str_len_1 + str_len_2 + int_len_1 + int_len_2;

		data->data = malloc(total_len);
		DIE(NULL == data->data, "malloc failed");

		void *jesus = data->data;

		total_len = 0;

		// copy given info into the structure
		memcpy(jesus + total_len, name_first, strlen(name_first) + 1);
		total_len += strlen(name_first) + 1;

		memcpy(jesus + total_len, &money01, sizeof(int8_t));
		total_len += sizeof(int8_t);

		memcpy(jesus + total_len, &money02, sizeof(int8_t));
		total_len += sizeof(int8_t);

		memcpy(jesus + total_len, name_second, strlen(name_second) + 1);
		total_len += strlen(name_second) + 1;

	} else if (data_type == 2) {
		int int_len_1 = sizeof(int16_t);
		int int_len_2 = sizeof(int32_t);
		
		// calculate the number of bytes introduced
		total_len = str_len_1 + str_len_2 + int_len_1 + int_len_2;

		data->data = malloc(total_len);
		DIE(NULL == data->data, "malloc failed");

		void *jesus = data->data;
		
		total_len = 0;

		// copy given info into the structure
		memcpy(jesus + total_len, name_first, strlen(name_first) + 1);
		total_len += strlen(name_first) + 1;

		memcpy(jesus + total_len, &money01, sizeof(int16_t));
		total_len += sizeof(int16_t);

		memcpy(jesus + total_len, &money02, sizeof(int32_t));
		total_len += sizeof(int32_t);

		memcpy(jesus + total_len, name_second, strlen(name_second) + 1);
		total_len += strlen(name_second) + 1;

	} else {
		int int_len_1 = sizeof(int32_t);
		int int_len_2 = sizeof(int32_t);
		
		// calculate the number of bytes introduced
		total_len = str_len_1 + str_len_2 + int_len_1 + int_len_2;

		data->data = malloc(total_len);
		DIE(NULL == data->data, "malloc failed");

		void *jesus = data->data;

		total_len = 0;

		// copy given info into the structure
		memcpy(jesus + total_len, name_first, strlen(name_first) + 1);
		total_len += strlen(name_first) + 1;

		memcpy(jesus + total_len, &money01, sizeof(int32_t));
		total_len += sizeof(int32_t);

		memcpy(jesus + total_len, &money02, sizeof(int32_t));
		total_len += sizeof(int32_t);

		memcpy(jesus + total_len, name_second, strlen(name_second) + 1);
		total_len += strlen(name_second) + 1;
	}

	// copy given info into the structure
	memcpy(&data->header->type, &data_type, sizeof(unsigned char));
	memcpy(&data->header->len, &total_len, sizeof(int));

	// free alocated space
	free(name_first);
	free(name_second);

	return data;
}

int add_last(void **arr, int *len, data_structure *data)
{
	// get type
	int data_type;
	scanf("%d", &data_type);

	// get data
	data = read(data_type);

	// check if it's the first time we add data
	if (*arr == NULL) {
		void *aux = malloc(sizeof(head) + data->header->len);
		DIE(NULL == aux, "malloc failed");
		*arr = aux;

	} else {
		void *aux = realloc(*arr, *len + sizeof(head) + data->header->len);
		DIE(NULL == aux, "realloc failed");
		*arr = aux;
	}

	void *jesus = *arr + *len;

	// populate the array
	memcpy(jesus, data->header, sizeof(head));
	jesus += sizeof(head);
	memcpy(jesus, data->data, data->header->len);

	*len += sizeof(head) + data->header->len;

	free(data->header);
	free(data->data);
	free(data);

	return 0;
}

int add_at(void **arr, int *len, data_structure *data, int index)
{
	// make use of the add_last function if needed
	if (index >= data_structure_cnt(*arr, *len)) {
		add_last(arr, len, data);
		return 0;
	}

	// get type
	int data_type;
	scanf("%d", &data_type);

	// get data
	data = read(data_type);

	// add more space
	void *aux = realloc(*arr, *len + sizeof(head) + data->header->len);
	DIE(NULL == aux, "malloc failed");
	*arr = aux;

	int counter = 0;
	int size = 0;

	head *header = *arr;
	void *jesus = *arr;

	// move until reaching the given index
	while (counter < index) {
		header = jesus;
		size += sizeof(head) + header->len;
		jesus += sizeof(head) + header->len;
		counter++;
	}

	// move things so that we can introduce the new data
	memmove(*arr + size + sizeof(head) + data->header->len, *arr + size, *len - size);
	
	jesus = *arr + size;

	// introduce the new data
	memcpy(jesus, data->header, sizeof(head));
	jesus += sizeof(head);
	memcpy(jesus, data->data, data->header->len);

	*len += sizeof(head) + data->header->len;

	// free data
	free(data->header);
	free(data->data);
	free(data);

	return 0;
}

void print(void *arr, int len)
{
	void *byte = arr;
	head *header = arr;

	// print untill there is nothing to be printed 
	while (len > 0) {
		header = byte;

		// check for type
		if (header->type == 1) {
			printf("Tipul %d\n", header->type);

			// use an addition pointer to move towards the array
			void* ptr = (char*)byte + sizeof(head);
			
			char *string = ptr;
			ptr += strlen(string) + 1;
			int8_t num1 = *(int8_t*)ptr;
			ptr += sizeof(int8_t);
			int8_t num2 = *(int8_t*)ptr;
			ptr += sizeof(int8_t);
			char *string2 = ptr;

			printf("%s pentru %s\n", string, string2);

			printf("%"PRId8"\n", num1);
			printf("%"PRId8"\n", num2); 
			
		// do the same for the others
		} else if (header->type == 2) {
			printf("Tipul %d\n", header->type);

			void* ptr = (char*)byte + sizeof(head);
			
			char *string = ptr;
			ptr += strlen(string) + 1;
			int16_t num1 = *(int16_t*)ptr;
			ptr += sizeof(int16_t);
			int32_t num2 = *(int32_t*)ptr;
			ptr += sizeof(int32_t);
			char *string2 = ptr;

			printf("%s pentru %s\n", string, string2);

			printf("%"PRId16"\n", num1);
			printf("%"PRId32"\n", num2);
			
		// here too
		} else {
				printf("Tipul %d\n", header->type);

			void* ptr = (char*)byte + sizeof(head);
			
			char *string = ptr;
			ptr += strlen(string) + 1;
			int32_t num1 = *(int32_t*)ptr;
			ptr += sizeof(int32_t);
			int32_t num2 = *(int32_t*)ptr;
			ptr += sizeof(int32_t);
			char *string2 = ptr;

			printf("%s pentru %s\n", string, string2);

			printf("%"PRId32"\n", num1);
			printf("%"PRId32"\n", num2);		
		}

		// decrease size
		len -= sizeof(head);
		len -= header->len;
		// move forward
		byte = byte + sizeof(head) + header->len;

		// we need space
		printf("\n");
	}
}

void find(void *arr, int len, int index) 
{
	// validate index
	if (index < 0 || index >= data_structure_cnt(arr, len)) {
		return ;
	}
	int counter = 0;
	
	void* byte = arr;
	head *header = arr;

	// move until reaching the given index
	while (counter != index) {
		header = byte;
		byte = byte + sizeof(head) + header->len;
		counter++;
	}

	header = byte;

	// print the dedication we are looking for
	print(header, 1);

}

int delete_at(void **arr, int *len, int index)
{
	int counter = 0;
	int size = 0;

	head *header = *arr;
	void *jesus = *arr;

	while (counter < index) {
		header = jesus;
		size += sizeof(head) + header->len;
		jesus += sizeof(head) + header->len;
		counter++;
	}

	header = jesus;
	int aux = header->len;

	// get rid of date, move something over it
	memmove(*arr + size, *arr + size + sizeof(head) + header->len, *len - size - sizeof(head) - header->len);

	// realloc space (less this time)

	void *auxi = realloc(*arr, *len - sizeof(head) - aux);

	// decrease space
	*len = *len - sizeof(head) - aux;
	DIE(NULL == auxi && *len != 0, "realloc failed");
	*arr = auxi;

	return 0;
}

int main() {
	// vector of bytes
	void *arr = NULL;
	// number of bytes
	int len = 0;
	int index;

	char *action = malloc(sizeof(char) * 10);
	DIE(NULL == action, "malloc failed");

	data_structure *data = NULL;

	do {
		scanf("%s", action);

		if (strcmp(action, "insert") == 0) {
			add_last(&arr, &len, data);

		} else if (strcmp(action, "insert_at") == 0) {
			// find were to insert
			scanf("%d", &index);
			add_at(&arr, &len, data, index);

		} else if (strcmp(action, "delete_at") == 0) {
			// find what to delete
			scanf("%d", &index);
			delete_at(&arr, &len, index);

		} else if (strcmp(action, "find") == 0) {
			// find what you are looking for
			scanf("%d", &index);
			find(arr, len, index);

		} else if (strcmp(action, "print") == 0) {
			print(arr, len);

		}
	} while (strcmp(action, "exit") != 0);
	
	// free the vector
	free(arr);
	free(action);

	return 0;
}
