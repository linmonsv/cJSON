
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

/* Create a bunch of objects as demonstration. */
static int print_preallocated(cJSON *root)
{
	/* declarations */
	char *out = NULL;
	char *buf = NULL;
	char *buf_fail = NULL;
	size_t len = 0;
	size_t len_fail = 0;

	/* formatted print */
	out = cJSON_Print(root);

	/* create buffer to succeed */
	/* the extra 5 bytes are because of inaccuracies when reserving memory */
	len = strlen(out) + 5;
	buf = (char*)malloc(len);
	if (buf == NULL)
	{
		printf("Failed to allocate memory.\n");
		exit(1);
	}

	/* create buffer to fail */
	len_fail = strlen(out);
	buf_fail = (char*)malloc(len_fail);
	if (buf_fail == NULL)
	{
		printf("Failed to allocate memory.\n");
		exit(1);
	}

	/* Print to buffer */
	if (!cJSON_PrintPreallocated(root, buf, (int)len, 1)) {
		printf("cJSON_PrintPreallocated failed!\n");
		if (strcmp(out, buf) != 0) {
			printf("cJSON_PrintPreallocated not the same as cJSON_Print!\n");
			printf("cJSON_Print result:\n%s\n", out);
			printf("cJSON_PrintPreallocated result:\n%s\n", buf);
		}
		free(out);
		free(buf_fail);
		free(buf);
		return -1;
	}

	/* success */
	printf("%s\n", buf);

	/* force it to fail */
	if (cJSON_PrintPreallocated(root, buf_fail, (int)len_fail, 1)) {
		printf("cJSON_PrintPreallocated failed to show error with insufficient memory!\n");
		printf("cJSON_Print result:\n%s\n", out);
		printf("cJSON_PrintPreallocated result:\n%s\n", buf_fail);
		free(out);
		free(buf_fail);
		free(buf);
		return -1;
	}

	free(out);
	free(buf_fail);
	free(buf);
	return 0;
}

int main(void)
{
	/* print the version */
	printf("Version: %s\n", cJSON_Version());

	cJSON *root = NULL;
	cJSON *order_list = NULL;
	cJSON *fmt = NULL;
	cJSON *records = NULL;
	cJSON *ext = NULL;

	/* Our "Order_Recv" datatype: */
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "app_id", cJSON_CreateString("app_id"));
	cJSON_AddItemToObject(root, "charset", cJSON_CreateString("GBK"));
	cJSON_AddItemToObject(root, "format", cJSON_CreateString("json"));
	cJSON_AddItemToObject(root, "timestamp", cJSON_CreateString("2016-12-10 15:09:20"));
	cJSON_AddItemToObject(root, "version", cJSON_CreateString("1.0"));
	cJSON_AddItemToObject(root, "sign_type", cJSON_CreateString("sha1withrsa"));
	cJSON_AddItemToObject(root, "sign", cJSON_CreateString("sha1withrsa"));

	cJSON_AddItemToObject(root, "order_list", order_list = cJSON_CreateArray());
	cJSON_AddItemToArray(order_list, fmt = cJSON_CreateObject());
	cJSON_AddStringToObject(fmt, "city_id", "440100");
	{
		char *record[] = { "Iu51KIJZMAeBRB...." , "YwDHwD1j1HW612...." };
		cJSON_AddItemToObject(fmt, "record", records = cJSON_CreateStringArray(record, 2));
	}

	cJSON_AddStringToObject(fmt, "order_desc", "XXXX");
	cJSON_AddNumberToObject(fmt, "charge_type", 1);
	cJSON_AddNumberToObject(fmt, "exp_type", 0);
	cJSON_AddNumberToObject(fmt, "order_time", 1485240745);
	cJSON_AddStringToObject(fmt, "open_id", "000284D4999D1256");
	cJSON_AddNumberToObject(fmt, "total_fee", 100);
	cJSON_AddNumberToObject(fmt, "pay_fee", 100);
	cJSON_AddStringToObject(fmt, "mch_trx_id", "B2E2A01DD92C9EDA");
	cJSON_AddItemToObject(fmt, "ext", ext = cJSON_CreateObject());
	cJSON_AddStringToObject(ext, "out_station_name", "B station");
	cJSON_AddStringToObject(ext, "in_station_name", "A station");
	cJSON_AddNumberToObject(ext, "out_station_id", 123456780);
	cJSON_AddNumberToObject(ext, "in_station_id", 123456789);

	/* Print to text */
	if (print_preallocated(root) != 0) {
		cJSON_Delete(root);
		exit(EXIT_FAILURE);
	}
	cJSON_Delete(root);

	return 0;
}
