#include "WorkEndRequest.h"

WorkEndRequest::WorkEndRequest()
{
    SetHint(WORKEND);
    SetJob({});
}

void WorkEndRequest::SetJob(const QString &job)
{
    payload.insert("job", job);
}

QString WorkEndRequest::Getjob() const
{
    return payload["job"].toString();
}
