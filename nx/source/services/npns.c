#include "types.h"
#include "result.h"
#include "kernel/ipc.h"
#include "services/sm.h"
#include "services/npns.h"

Service g_npnsSrv;
Service g_iNRecSrv;

Result _getINotifRec() {
    IpcCommand c;
	ipcInitialize(&c);
	struct rq{
		u64 magic;
		u64 cmd_id;
	} *raw;

	raw = serviceIpcPrepareHeader(&g_npnsSrv, &c, sizeof(*raw));
	raw->magic = SFCI_MAGIC;
	raw->cmd_id = 120;

	Result rc = serviceIpcDispatch(&g_npnsSrv);

    if (R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        struct {
            u64 magic;
            u64 result;
        } *resp;

        serviceIpcParse(&g_npnsSrv, &r, sizeof(*resp));
        resp = r.Raw;

		serviceCreateSubservice(&g_iNRecSrv, &g_npnsSrv, &r, 0);
		rc = resp->result;

	}
    return rc;
}

Result npnsInitialize() {
    Result rc = smGetService(&g_npnsSrv, "npns:u");

    if (R_SUCCEEDED(rc)) {
        rc = _getINotifRec();
    }

    if (R_FAILED(rc)) {
        npnsExit();
    }

    return rc;
}

/*Result npnsINRecListenTo()
{

}*/

void npnsExit() {
    serviceClose(&g_npnsSrv);
    serviceClose(&g_iNRecSrv);
}