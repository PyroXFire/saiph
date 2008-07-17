#include "Telnet.h"

/* constructors */
Telnet::Telnet() {
	/* connect to host using telnet */
	struct hostent *he = gethostbyname(TELNET_NETHACK_URL);
	if (he == NULL) {
		herror("gethostbyname");
		exit(1);
	}
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		perror("socket");
		exit(1);
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(TELNET_NETHACK_PORT);
	addr.sin_addr = *((struct in_addr *) he->h_addr);
	memset(addr.sin_zero, '\0', sizeof (addr.sin_zero));

	if (connect(sock, (struct sockaddr *) &addr, sizeof (addr)) == -1) {
		perror("connect");
		exit(1);
	}

	/* set up ping */
	ping[0] = 0xff; // IAC
	ping[1] = 0xfd; // DO
	ping[2] = 0x63; // decimal 99, which is our "ping"

	/* tell server our terminal (xterm-color) and size (80x24) and set some other things */
	char data[] = {0xff, 0xfb, 0x18, 0xff, 0xfa, 0x18, 0x00, 'x', 't', 'e', 'r', 'm', 0xff, 0xf0, 0xff, 0xfc, 0x20, 0xff, 0xfc, 0x23, 0xff, 0xfc, 0x27, 0xff, 0xfe, 0x03, 0xff, 0xfb, 0x01, 0xff, 0xfd, 0x05, 0xff, 0xfb, 0x21, 0xff, 0xfb, 0x1f, 0xff, 0xfa, 0x1f, 0x00, 0x50, 0x00, 0x18, 0xff, 0xf0}; 
	transmit(data, 47);

	/* that last command we sent to will send us data back.
	 * let's discard it */
	char discard[4096];
	retrieve(discard, 4096);

	/* and let's log in */
	start();
}

/* destructor */
Telnet::~Telnet() {
	stop();
	close(sock);
}

/* methods */
int Telnet::retrieve(char *buffer, int count) {
	/* this is borrowed from TAEB:
	 * we can send a "ping" by transmitting [0xff, 0xfd, 0x63].
	 * then we'll just read until last bytes equal [?, ?, ?] */
	transmit(ping, 3);
	int retrieved = 0;
	int more_data = true;
	int tries = 5;
	while (more_data && tries >= 0) {
		retrieved += recv(sock, &buffer[retrieved], count, 0);
		for (int a = retrieved - 3; a >= 0; --a) {
			/* search for pong backwards (we're expecting pong to be last).
			 * we do it like this because if we find it somewhere else than
			 * at the end of the stream, we should send a new ping */
			if ((unsigned char) buffer[a] == 0xff && (unsigned char) buffer[a + 1] == 0xfc && (unsigned char) buffer[a + 2] == 0x63) {
				/* found pong, but is it at end of stream? */
				if (a == retrieved - 3) {
					/* yes, it is.
					 * however, did we receive something else than the pong? */
					if (retrieved > 3) {
						/* yes, we did, good! */
						more_data = false;
						/* remove pong */
						buffer[--retrieved] = '\0';
						buffer[--retrieved] = '\0';
						buffer[--retrieved] = '\0';
					} else {
						/* no, we didn't. hmmm */
						retrieved = 0;
						transmit(ping, 3);
						--tries; // also decrease this, in case we're not getting any data */
					}
				} else {
					/* no, it isn't.
					 * we should send a new ping */
					transmit(ping, 3);
					/* and we'll have to remove the pong reply, gah */
					for (int b = a; b < retrieved - 3; ++b)
						buffer[b] = buffer[b + 3];
					retrieved -= 3;
					break;
				}
			}
		}
	}
	if (tries < 0)
		cerr << "WARNING: We were expecting data, but got none. Fix this bug, please!" << endl;
	return retrieved;
}

int Telnet::transmit(const string &data) {
	return transmit(data.c_str(), data.length());
}

void Telnet::start() {
	ifstream account;
	account.open(".account");
	if (!account) {
		cerr << "Unable to read .account (which should contain username and password on separate lines)" << endl;
		exit(1);
	}
	string username;
	string password;
	account >> username;
	account >> password;
	account.close();
	username.append("\n");
	password.append("\n");
	char buffer[TELNET_BUFFER_SIZE];
	transmit("l");
	int size = retrieve(buffer, TELNET_BUFFER_SIZE);
	transmit(username);
	size = retrieve(buffer, TELNET_BUFFER_SIZE);
	transmit(password);
	size = retrieve(buffer, TELNET_BUFFER_SIZE);
	transmit("p");
	cerr << "Done logging in, a game should appear for the bot now" << endl;
}

void Telnet::stop() {
}

/* private methods */
int Telnet::transmit(const char *data, int length) {
	cerr << "Sending: ";
	for (int a = 0; a < length; ++a)
		cerr << data[a] << endl;
	return send(sock, data, length, 0);
}