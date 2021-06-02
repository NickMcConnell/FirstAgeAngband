/* parse/readstore */

#include "unit-test.h"
#include "object.h"
#include "store.h"
#include "init.h"

int setup_tests(void **state) {
	z_info = mem_zalloc(sizeof(struct angband_constants));
	z_info->store_inven_max = 24;
	*state = init_parse_stores();
	return !*state;
}

int teardown_tests(void *state) {
	struct store *s = parser_priv(state);
	struct owner *o = s->owners, *o_next;
	while (o) {
		o_next = o->next;
		string_free(o->name);
		mem_free(o);
		o = o_next;
	}
	string_free((char *)s->name);
	mem_free(s);
	parser_destroy(state);
	mem_free(z_info);
	return 0;
}

int test_store0(void *state) {
	enum parser_error r = parser_parse(state, "store:foobar");
	struct store *s;

	eq(r, PARSE_ERROR_NONE);
	s = parser_priv(state);
	require(s);
	eq(s->sidx, 0);
	require(streq(s->name, "foobar"));
	ok;
}

int test_slots0(void *state) {
	enum parser_error r = parser_parse(state, "slots:2:33");
	struct store *s;

	eq(r, PARSE_ERROR_NONE);
	s = parser_priv(state);
	require(s);
	eq(s->normal_stock_min, 2);
	eq(s->normal_stock_max, 33);
	ok;
}

int test_owner0(void *state) {
	enum parser_error r = parser_parse(state, "owner:5000:Munchkin:Foo");
	struct store *s;

	eq(r, PARSE_ERROR_NONE);
	s = parser_priv(state);
	eq(s->owners->max_cost, 5000);
	require(streq(s->owners->name, "Foo"));
	ok;
}

/* Causes segfault: lookup_name() requires z_info/k_info */
int test_i0(void *state) {
	enum parser_error r = parser_parse(state, "normal:3:5");
	struct store *s;

	eq(r, PARSE_ERROR_NONE);
	s = parser_priv(state);
	require(s);
	require(s->normal_table[0]);
	require(s->normal_table[1]);
	ok;
}

const char *suite_name = "parse/store";
struct test tests[] = {
	{ "store0", test_store0 },
	{ "slots0", test_slots0 },
	{ "owner0", test_owner0 },
/*	{ "i0", test_i0 }, */
	{ NULL, NULL }
};
