#ifndef CT_DEFS_H
#define CT_DEFS_H

// Definitions related to data transmission
#define CT_PKTDATA 0xFFFFFFF0
#define CT_RESDATA 0xFFFFFFF1
#define CT_DBSDATA 0xFFFFFFF2
#define CT_CHUNKSIZE 512

enum { ST_TYPE, ST_SIZE, ST_DATA };
enum Data{T_Patient, T_Worklogs, T_Scenarios, T_NoOne};

#endif // CT_DEFS_H
