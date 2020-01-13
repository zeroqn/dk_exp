/*
 * ECMAScript execution environment
 */

#include <stdlib.h>

#include "duktape.h"

#define EE_ERR_ARGC_NUM 1
#define EE_ERR_INIT_CTX 2

static duk_ret_t native_print(duk_context *ctx) {
	duk_push_string(ctx, " ");
	duk_insert(ctx, 0);
	duk_join(ctx, duk_get_top(ctx) - 1);
	printf("%s\n", duk_safe_to_string(ctx, -1));
	return 0;
}

static duk_ret_t native_adder(duk_context *ctx) {
	int i;
	int n = duk_get_top(ctx);  /* #args */
	double res = 0.0;

	for (i = 0; i < n; i++) {
		res += duk_to_number(ctx, i);
	}

	duk_push_number(ctx, res);
	return 1;  /* one return value */
}

int main(int argc, char *argv[]) {
    char src[1024];
    FILE *file;
    size_t nread;

    file = fopen("../src/test.js", "r");
    if (!file) {
        return EE_ERR_INIT_CTX;
    }

    nread = fread(src, 1, sizeof(src), file);
    if (ferror(file) || 0 == nread) {
        return EE_ERR_INIT_CTX;
    }
    fclose(file);

    char json[1024];

    file = fopen("../src/args.json", "r");
    if (!file) {
        return EE_ERR_INIT_CTX;
    }

    nread = fread(json, 1, sizeof(json), file);
    if (ferror(file) || 0 == nread) {
        return EE_ERR_INIT_CTX;
    }
    fclose(file);

    duk_context *ctx = duk_create_heap_default();
    if (!ctx) {
        return EE_ERR_INIT_CTX;
    }

	duk_push_c_function(ctx, native_print, DUK_VARARGS);
	duk_put_global_string(ctx, "print");
    duk_push_c_function(ctx, native_adder, DUK_VARARGS);
    duk_put_global_string(ctx, "adder");

    // Eval src
    duk_push_string(ctx, "function");
    if (duk_pcompile_string(ctx, DUK_COMPILE_FUNCTION, src) != 0) {
        printf("compile failed: %s\n", duk_safe_to_string(ctx, -1));
        return EE_ERR_INIT_CTX;
    }

    // Parse json args
    duk_push_string(ctx, json);
    duk_json_decode(ctx, -1);

    // Call our funtion
    duk_call(ctx, 1);
    printf("function result: %s\n", duk_get_string(ctx, -1));

    duk_pop(ctx);
    duk_destroy_heap(ctx);

    return 0;
}
