#include "WorkBeginRequest.h"

WorkBeginRequest::WorkBeginRequest()
{
    SetHint(WORKBEGIN);
    SetJob({});
}

void WorkBeginRequest::SetJob(const QString &job)
{
    payload.insert("job", job);
}

QString WorkBeginRequest::Getjob() const
{
    return payload["job"].toString();
}
