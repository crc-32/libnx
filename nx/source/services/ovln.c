#include <string.h>
#include "services/ovln.h"
#include "services/sm.h"
#include "types.h"
#include "result.h"
#include "kernel/ipc.h"

static Service g_ovlnSrv;
static Service g_irecSrv;

Result _getIRec() {
	IpcCommand c;
	ipcInitialize(&c);
	struct rq{
		u64 magic;
		u64 cmd_id;
	} *raw;

	raw = serviceIpcPrepareHeader(&g_ovlnSrv, &c, sizeof(*raw));
	raw->magic = SFCI_MAGIC;
	raw->cmd_id = 0;

	Result rc = serviceIpcDispatch(&g_ovlnSrv);

    if (R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        struct {
            u64 magic;
            u64 result;
        } *resp;

        serviceIpcParse(&g_ovlnSrv, &r, sizeof(*resp));
        resp = r.Raw;

		serviceCreateSubservice(&g_irecSrv, &g_ovlnSrv, &r, 0);
		rc = resp->result;

	}
    return rc;
}

Result _regIRec() {
	IpcCommand c;
	ipcInitialize(&c);

	struct {
		u64 magic;
		u64 cmd_id;
		char unk[10];
	} *raw;

	raw = serviceIpcPrepareHeader(&g_irecSrv, &c, sizeof(*raw));
	raw->magic = SFCI_MAGIC;
	raw->cmd_id = 0;
	strcpy(raw->unk, "overlay");
	Result rc = serviceIpcDispatch(&g_irecSrv);

    if (R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        struct {
            u64 magic;
            u64 result;
        } *resp;

        serviceIpcParse(&g_irecSrv, &r, sizeof(*resp));
        resp = r.Raw;
		rc = resp->result;
	}
    return rc;
}

Result ovlnInitialize() {
    Result rc = smGetService(&g_ovlnSrv, "ovln:rcv");

    if (R_SUCCEEDED(rc)) {
        rc = _getIRec();
        if (R_SUCCEEDED(rc)) {
            rc = _regIRec();
        }
    }

    if (R_FAILED(rc)) {
        ovlnExit();
    }

    return rc;
}

void ovlnExit() {
    serviceClose(&g_ovlnSrv);
    serviceClose(&g_irecSrv);
}

Result ovlnIReceiverGetEvent(Event *out) {
	IpcCommand c;
	ipcInitialize(&c);
	struct {
		u64 magic;
		u64 cmd_id;
	} *raw;

	raw = serviceIpcPrepareHeader(&g_irecSrv, &c, sizeof(*raw));
	raw->magic = SFCI_MAGIC;
	raw->cmd_id = 2;
	Result rc = serviceIpcDispatch(&g_irecSrv);

    if (R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        struct {
            u64 magic;
            u64 result;
        } *resp;

        serviceIpcParse(&g_irecSrv, &r, sizeof(*resp));
        resp = r.Raw;
		eventLoadRemote(out, r.Handles[0], true);
		rc = resp->result;
	}
    return rc;
}

Result ovlnIReceiverGetNotification(IReceiverNotification *out)
{
	IpcCommand c;
	ipcInitialize(&c);
	struct {
		u64 magic;
		u64 cmd_id;
	} *raw;

	raw = serviceIpcPrepareHeader(&g_irecSrv, &c, sizeof(*raw));
	raw->magic = SFCI_MAGIC;
	raw->cmd_id = 4;
	Result rc = serviceIpcDispatch(&g_irecSrv);

	if (R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        struct {
            u64 magic;
            u64 result;
			u16 type;
			u8 off[6];
			u16 content;
        } *resp;
        serviceIpcParse(&g_irecSrv, &r, sizeof(*resp));
        resp = r.Raw;
		out->content = resp->content;
		out->type = (resp->type>>8) | (resp->type<<8);
		rc = resp->result;
	}
    return rc;
}