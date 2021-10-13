/* Example: parse a simple configuration file */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ini.h"
#include "../value_parsers.h"

typedef struct
{
	int version;
	bool active;
	const char* name;
	const char* email;
	float pi;
} configuration;

/*
DBJ: there is no ini file in memory
it is simply read line by line, each line given to a callback
half  processed

if ini file is empty the handler callback is not called
if there are just ections but no value name pairs this handler is not called
if there are no sections only one or more name = value pairs the thing will work,
but you have to code the callback accordingly; section arg will exist as a empty string

*/
static int handler(
	/* this can be NULL, but it is usefull to collect all the data required into a struct  */
	void* user,
	const char* section,
	const char* name,
	const char* value)
{
	configuration* pconfig = (configuration*)user;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

	/*
	unlike some other ini readers there is no inbuilt value parsing
	*/

	if (MATCH("protocol", "version")) {
		pconfig->version = atoi(value);
	}
	else if (MATCH("user", "name")) {
		pconfig->name = SUPR4996 strdup(value);
	}
	else if (MATCH("user", "email")) {
		pconfig->email = SUPR4996 strdup(value);
	}
	else if (MATCH("user", "pi")) {
#ifdef _MSC_VER
		_CRT_FLOAT cf_ = { pconfig->pi };
		int rez_ = _atoflt(&cf_, value);
		if (0 == rez_)
			pconfig->pi = cf_.f;
		else
			pconfig->pi = 0;
#else
		pconfig->pi = (float)atof(value);
#endif
	}
	else if (MATCH("user", "active")) {
		// parsing the boolean value is a bit more involved
		dbj_getboolean(&pconfig->active, value);
	}
	else {
		return 0;  /* signal the unknown section/name, error */
	}
	return 1; /* signal OK  */
}
/* ---------------------------------------------------------------------------- */
int main(int argc, char* argv[])
{
	/* set defaults */
	configuration config = { 0,0,0,0 };

	if (ini_parse("test.ini", handler, &config) < 0) {
		printf("Can't load 'test.ini'\n");
		return 1;
	}
	printf(
		"\nConfig loaded from 'test.ini'\n"
		"\n%24s : %d"
		"\n%24s : %s"
		"\n%24s : %s"
		"\n%24s : %s"
		"\n%24s : %f\n",
		"version", config.version,
		"active", config.active ? "true" : "false",
		"name", config.name,
		"email", config.email,
		"pi", config.pi);

	if (config.name)
		free((void*)config.name);
	if (config.email)
		free((void*)config.email);

	return 0;
}
