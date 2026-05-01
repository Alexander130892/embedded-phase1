/*
 * File:    Ex_16_RPC.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file implements an RPC (Remote Procedure Call) client that
 *   polls a remote host to retrieve system statistics including boot
 *   time and current time using the rstat service. It makes a remote
 *   call to the specified host and prints the retrieved timing data if
 *   the call succeeds.
 */
//Libraries
#include <stdio.h>
#include <rpc/rpc.h>
#include <rpcsvc/rstat.h>
//Poll the host

void do_poll(char* host){
    int     stat;
    struct  statstime   result_stats;

    stat = callrpc(host, RSTATPROG, RSTATVERS_TIME, RSTATPROC_STATS,
                    xdr_void, 0, xdr_statstime, &result_stats);
    if (stat == RPC_SUCCESS)
    {
        fprintf(stdout, "DATA %s %ld %ld\n", host, 
        result_stats.boottime.tv_sec, result_stats.curtime.tv_sec);
    }

}

