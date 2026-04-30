/*
 * File:    Ex_16_RPC.c
 * Author:  Alexander130892
 * Date:    15-4-2026
 *
 * Description:
 *   This file implements an RPC client that polls a remote host to
 *   retrieve system statistics including boot time and current time,
 *   printing the results to stdout if the RPC call succeeds.
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

