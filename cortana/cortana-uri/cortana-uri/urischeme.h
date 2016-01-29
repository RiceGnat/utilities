#pragma once

#define URI_PROTOCOL "cortana:"
#define ARG_DELIM '-'
#define COMMAND_INDEX 0
#define COMMAND_PARAMINDEX(X) COMMAND_INDEX + X

#define CMD_WOL "wol"
#define CMD_SHUTDOWN "shutdown"
#define CMD_SLEEP "sleep"
#define CMD_RESTART "restart"
#define MATCHES(X) compare(X) == 0