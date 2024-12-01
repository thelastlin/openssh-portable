#include "includes.h"

#include <stdlib.h>
#include <string.h>
#include "log.h"

#include <fido.h>

void __attach(void) __attribute__((constructor));

int ssh_sk_fido2_initflags = 0;

void
__attach(void)
{
	const char *verbosity = getenv("SK_STANDALONE");
	SyslogFacility log_facility = SYSLOG_FACILITY_AUTH;
	if (verbosity == NULL)
		return;

	LogLevel log_level = SYSLOG_LEVEL_INFO;
	int print_stderr = 0;
	for (size_t idx = 0; idx < strlen(verbosity); idx++) {
		switch (verbosity[idx]) {
		case 'v': {
			if (log_level == SYSLOG_LEVEL_INFO)
				log_level = SYSLOG_LEVEL_DEBUG1;
			else if (log_level < SYSLOG_LEVEL_DEBUG3)
				log_level++;
			break;
		}
		case 'c': {
			print_stderr = 1;
			break;
		}
		case 'f': {
			ssh_sk_fido2_initflags = FIDO_DEBUG;
		}
		case '\0':
			break;
		default: {
			fprintf(stderr,
				"usage SK_STANDALONE=\"[v][c][f]\"\n");
			return;
		}
		}
	}
	log_init("sk-libfido2", log_level, log_facility, print_stderr);
}
