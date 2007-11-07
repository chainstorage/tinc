/*
    control_protocol.h -- control socket protocol.
    Copyright (C) 2007 Scott Lamb <slamb@slamb.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    $Id$
*/

#ifndef __TINC_CONTROL_PROTOCOL_H__
#define __TINC_CONTROL_PROTOCOL_H__

enum request_type {
	REQ_STOP,
	REQ_RELOAD,
	REQ_RESTART,
	REQ_DUMP_NODES,
	REQ_DUMP_EDGES,
	REQ_DUMP_SUBNETS,
	REQ_DUMP_CONNECTIONS,
	REQ_DUMP_GRAPH,
	REQ_PURGE,
	REQ_SET_DEBUG,
	REQ_RETRY,
};

#define TINC_CTL_VERSION_CURRENT 0

/* This greeting is sent by the server on socket open. */
typedef struct tinc_ctl_greeting_t {
	int version;
} tinc_ctl_greeting_t;

/* A single request or response header. */
typedef struct tinc_ctl_request_t {
	size_t length; /* total length, including the header */
	enum request_type type;
	int id;
	int res_errno; /* used only for responses */
} tinc_ctl_request_t;

#endif
