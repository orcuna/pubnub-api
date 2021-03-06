#include <ctype.h>
#include <Ethernet.h>
#include "PubNub.h"

class PubNub PubNub;

bool PubNub::begin(char *publish_key_, char *subscribe_key_, char *origin_)
{
	publish_key = publish_key_;
	subscribe_key = subscribe_key_;
	origin = origin_;
}

EthernetClient *PubNub::publish(char *channel, char *message)
{
	EthernetClient &client = publish_client;
	if (!client.connect(origin, 80)) {
		client.stop();
		return NULL;
	}

	client.print("GET /publish/");
	client.print(publish_key);
	client.print("/");
	client.print(subscribe_key);
	client.print("/0/");
	client.print(channel);
	client.print("/0/");

	/* Inject message, URI-escaping it in the process.
	 * We are careful to save RAM by not using any copies
	 * of the string or explicit buffers. */
	char *pmessage = message;
	while (pmessage[0]) {
		/* RFC 3986 Unreserved characters plus few
		 * safe reserved ones. */
		size_t okspan = strspn(pmessage, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_.~" ",=:;@[]");
		if (okspan > 0) {
			client.write((const uint8_t *) pmessage, okspan);
			pmessage += okspan;
		}
		if (pmessage[0]) {
			/* %-encode a non-ok character. */
			char enc[3] = {'%'};
			enc[1] = "0123456789ABCDEF"[pmessage[0] / 16];
			enc[2] = "0123456789ABCDEF"[pmessage[0] % 16];
			client.write((const uint8_t *) enc, 3);
			pmessage++;
		}
	}

	if (this->_request_bh(client)) {
		/* Success and reached body, return handle to the client
		 * for further perusal. */
		return &client;
	} else {
		/* Failure. */
		client.stop();
		return NULL;
	}
}

PubSubClient *PubNub::subscribe(char *channel)
{
	PubSubClient &client = subscribe_client;
	if (!client.connect(origin, 80)) {
		client.stop();
		return NULL;
	}

	client.print("GET /subscribe/");
	client.print(subscribe_key);
	client.print("/");
	client.print(channel);
	client.print("/0/");
	client.print(client.server_timetoken());

	if (this->_request_bh(client)) {
		/* Success and reached body. We need to eat '[' first,
		 * as our API contract is to return only the "message body"
		 * part of reply from subscribe. */
		while (client.connected() && !client.available()) /* wait */;
		if (!client.connected() || client.read() != '[') {
			/* Something unexpected. */
			client.stop();
			return NULL;
		}
		/* Now return handle to the client for further perusal.
		 * PubSubClient class will make sure that the client does
		 * not see the time token but we stop right after the
		 * message body. */
		client.start_body();
		return &client;
	} else {
		/* Failure. */
		client.stop();
		return NULL;
	}
}

EthernetClient *PubNub::history(char *channel, int limit)
{
	EthernetClient &client = history_client;
	if (!client.connect(origin, 80)) {
		client.stop();
		return NULL;
	}

	client.print("GET /history/");
	client.print(subscribe_key);
	client.print("/");
	client.print(channel);
	client.print("/0/");
	client.print(limit, DEC);

	if (this->_request_bh(client, false)) {
		/* Success and reached body, return handle to the client
		 * for further perusal. */
		return &client;
	} else {
		/* Failure. */
		client.stop();
		return NULL;
	}
}

bool PubNub::_request_bh(EthernetClient &client, bool chunked)
{
	/* Finish the first line of the request. */
	client.print(" HTTP/1.1\r\n");
	/* Finish HTTP request. */
	client.print("Host: ");
	client.print(origin);
	client.print("\r\nUser-Agent: PubNub-Arduino/1.0\r\nConnection: close\r\n\r\n");

#define WAIT() do { \
	while (client.connected() && !client.available()) /* wait */; \
	if (!client.connected()) { \
		/* Oops, connection interrupted. */ \
		return false; \
	} \
} while (0)

	/* Read first line with HTTP code. */
	/* "HTTP/1.x " */
	do {
		WAIT();
	} while (client.read() != ' ');
	/* Now, first digit of HTTP code. */
	WAIT();
	char c = client.read();
	if (c != '2') {
		/* HTTP code that is NOT 2xx means trouble.
		 * kthxbai */
		return false;
	}

	/* Skip the rest of headers. */
	while (client.connected()) {
		/* Wait until we get "\r\n\r\n" sequence (i.e., empty line
		 * that separates HTTP header and body). */
		WAIT();
		if (client.read() != '\r') continue;
		WAIT();
		if (client.read() != '\n') continue;
		WAIT();
		if (client.read() != '\r') continue;
		WAIT();
		if (client.read() != '\n') continue;
		if (chunked) {
			/* There is one extra line due to
			 * Transfer-encoding: chunked.
			 * Skip this line. */
			do {
				WAIT();
			} while (client.read() != '\r');
			WAIT();
			client.read(); /* '\n' */
		}

		/* Good! Body begins now. */
		return true;
	}
	/* No body means error. */
	return false;
}


int PubSubClient::read()
{
	int c = EthernetClient::read();
	if (!json_enabled || c == -1)
		return c;

	this->_state_input(c, NULL, 0);
	return c;
}

int PubSubClient::read(uint8_t *buf, size_t size)
{
	int len = EthernetClient::read(buf, size);
	if (!json_enabled || len <= 0)
		return len;
	for (int i = 0; i < len; i++) {
		this->_state_input(buf[i], &buf[i+1], len - i - 1);
		if (!connected()) {
			/* We have hit the end somewhere in this buffer.
			 * From user perspective, only characters up to
			 * index i are valid. */
			return i + 1;
		}
	}
	return len;
}

void PubSubClient::stop()
{
	if (!connected() || !json_enabled) {
		EthernetClient::stop();
		return;
	}
	/* We are still connected. Read the rest of the stream so that
	 * we catch the timetoken. */
	while (connected()) {
		while (connected() && !available()) /* wait */;
		char ch = read();
		this->_state_input(ch, NULL, 0);
	}
	json_enabled = false;
}

void PubSubClient::start_body()
{
	json_enabled = true;
	in_string = after_backslash = false;
	braces_depth = 0;
}

void PubSubClient::_state_input(uint8_t ch, uint8_t *nextbuf, size_t nextsize)
{
	/* Process a single character on input, updating the JSON
	 * state machine. If we reached the last character of input
	 * (just before expected ","), we will eat the rest of the body,
	 * update timetoken and close the connection. */
	if (in_string) {
		if (after_backslash) {
			/* Whatever this is... */
			after_backslash = false;
			return;
		}
		switch (ch) {
		case '"':
			in_string = false;
			if (braces_depth == 0)
				goto body_end;
			return;
		case '\\':
			after_backslash = true;
			return;
		default:
			return;
		}

	} else {
		switch (ch) {
		case '"':
			in_string = true;
			return;
		case '{':
		case '[':
			braces_depth++;
			return;
		case '}':
		case ']':
			braces_depth--;
			if (braces_depth <= 0)
				goto body_end;
			return;
		default:
			return;
		}
	}

	return;
body_end:
	/* End of data here. */
	this->_grab_timetoken(nextbuf, nextsize);
}

void PubSubClient::_grab_timetoken(uint8_t *nextbuf, size_t nextsize)
{
	char new_timetoken[22];
	size_t new_timetoken_len = 0;

	/* Expected followup now is:
	 * 	,"13511688131075270"]
	 */
	/* Somewhat unwieldy macros that will first exhaust nextbuf[],
	 * then read directly from socket. */
#define WAIT() do { \
	if (nextsize > 0) \
		break; \
	while (connected() && !available()) /* wait */; \
	if (!connected()) { \
		/* Oops, connection interrupted. */ \
		return; \
	} \
} while (0)
#define GETCHAR() (nextsize > 0 ? (nextsize--, *nextbuf++) : read())
	do { WAIT(); } while (GETCHAR() != ',');
	do { WAIT(); } while (GETCHAR() != '"');
	do {
		WAIT();
		char ch = GETCHAR();
		if (ch == '"')
			break;
		new_timetoken[new_timetoken_len++] = ch;
		if (new_timetoken_len >= sizeof(new_timetoken) - 1)
			break;
	} while (1);
	memcpy(timetoken, new_timetoken, new_timetoken_len);
	timetoken[new_timetoken_len] = 0;
}
