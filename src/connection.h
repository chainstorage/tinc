/*
    connection.h -- header for connection.c
    Copyright (C) 2000-2012 Guus Sliepen <guus@tinc-vpn.org>,
                  2000-2005 Ivo Timmermans

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef __TINC_CONNECTION_H__
#define __TINC_CONNECTION_H__

#include "buffer.h"
#include "cipher.h"
#include "digest.h"
#include "rsa.h"
#include "splay_tree.h"
#include "sptps.h"

#define OPTION_INDIRECT		0x0001
#define OPTION_TCPONLY		0x0002
#define OPTION_PMTU_DISCOVERY	0x0004
#define OPTION_CLAMP_MSS	0x0008
#define OPTION_VERSION(x) ((x) >> 24) /* Top 8 bits are for protocol minor version */

typedef struct connection_status_t {
		unsigned int pinged:1;			/* sent ping */
		unsigned int active:1;			/* 1 if active.. */
		unsigned int connecting:1;		/* 1 if we are waiting for a non-blocking connect() to finish */
		unsigned int termreq:1;			/* the termination of this connection was requested */
		unsigned int remove_unused:1;		/* Set to 1 if you want this connection removed */
		unsigned int timeout_unused:1;		/* 1 if gotten timeout */
		unsigned int encryptout:1;		/* 1 if we can encrypt outgoing traffic */
		unsigned int decryptin:1;		/* 1 if we have to decrypt incoming traffic */
		unsigned int mst:1;			/* 1 if this connection is part of a minimum spanning tree */
		unsigned int control:1;			/* 1 if this is a control connection */
		unsigned int pcap:1;			/* 1 if this is a control connection requesting packet capture */
		unsigned int log:1;			/* 1 if this is a control connection requesting log dump */
		unsigned int unused:20;
} connection_status_t;

#include "ecdsa.h"
#include "edge.h"
#include "net.h"
#include "node.h"

typedef struct connection_t {
	char *name;					/* name he claims to have */

	union sockaddr_t address;			/* his real (internet) ip */
	char *hostname;				/* the hostname of its real ip */
	int protocol_major;		/* used protocol */
	int protocol_minor;		/* used protocol */

	int socket;					/* socket used for this connection */
	uint32_t options;			/* options for this connection */
	connection_status_t status;	/* status info */
	int estimated_weight;		/* estimation for the weight of the edge for this connection */
	struct timeval start;		/* time this connection was started, used for above estimation */
	struct outgoing_t *outgoing;	/* used to keep track of outgoing connections */

	struct node_t *node;		/* node associated with the other end */
	struct edge_t *edge;		/* edge associated with this connection */

	rsa_t rsa;			/* his public RSA key */
	ecdsa_t ecdsa;			/* his public ECDSA key */
	cipher_t incipher;		/* Cipher he will use to send data to us */
	cipher_t outcipher;		/* Cipher we will use to send data to him */
	digest_t indigest;
	digest_t outdigest;
	sptps_t sptps;

	int inmaclength;
	int outmaclength;
	int incompression;
	int outcompression;

	char *hischallenge;		/* The challenge we sent to him */

	struct buffer_t inbuf;
	struct buffer_t outbuf;
	struct event inevent;				/* input event on this metadata connection */
	struct event outevent;				/* output event on this metadata connection */
	int tcplen;					/* length of incoming TCPpacket */
	int allow_request;			/* defined if there's only one request possible */

	time_t last_ping_time;		/* last time we saw some activity from the other end or pinged them */

	splay_tree_t *config_tree;	/* Pointer to configuration tree belonging to him */
} connection_t;

extern splay_tree_t *connection_tree;
extern connection_t *everyone;

extern void init_connections(void);
extern void exit_connections(void);
extern connection_t *new_connection(void) __attribute__ ((__malloc__));
extern void free_connection(connection_t *);
extern void free_connection_partially(connection_t *);
extern void connection_add(connection_t *);
extern void connection_del(connection_t *);
extern bool dump_connections(struct connection_t *);

#endif							/* __TINC_CONNECTION_H__ */
