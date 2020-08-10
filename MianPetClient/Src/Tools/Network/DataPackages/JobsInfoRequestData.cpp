#include "JobsInfoRequestData.h"

JobsInfoRequestData::JobsInfoRequestData()
{
    SetHint(JOBS_INFO);
    SetJobs({});
}

void JobsInfoRequestData::SetJobs(const QJsonArray &jobs)
{
    payload.insert("jobs", jobs);
}

QJsonArray JobsInfoRequestData::GetJobs() const
{
    return payload["jobs"].toArray();
}
