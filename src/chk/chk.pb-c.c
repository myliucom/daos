/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: chk.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "chk.pb-c.h"
void   chk__check_report__init
                     (Chk__CheckReport         *message)
{
  static const Chk__CheckReport init_value = CHK__CHECK_REPORT__INIT;
  *message = init_value;
}
size_t chk__check_report__get_packed_size
                     (const Chk__CheckReport *message)
{
  assert(message->base.descriptor == &chk__check_report__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t chk__check_report__pack
                     (const Chk__CheckReport *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &chk__check_report__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t chk__check_report__pack_to_buffer
                     (const Chk__CheckReport *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &chk__check_report__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Chk__CheckReport *
       chk__check_report__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Chk__CheckReport *)
     protobuf_c_message_unpack (&chk__check_report__descriptor,
                                allocator, len, data);
}
void   chk__check_report__free_unpacked
                     (Chk__CheckReport *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &chk__check_report__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor chk__check_report__field_descriptors[16] =
{
  {
    "seq",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT64,
    0,   /* quantifier_offset */
    offsetof(Chk__CheckReport, seq),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "class",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_ENUM,
    0,   /* quantifier_offset */
    offsetof(Chk__CheckReport, class_),
    &chk__check_inconsist_class__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "action",
    3,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_ENUM,
    0,   /* quantifier_offset */
    offsetof(Chk__CheckReport, action),
    &chk__check_inconsist_action__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "result",
    4,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_INT32,
    0,   /* quantifier_offset */
    offsetof(Chk__CheckReport, result),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "rank",
    5,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(Chk__CheckReport, rank),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "target",
    6,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(Chk__CheckReport, target),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "pool_uuid",
    7,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Chk__CheckReport, pool_uuid),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "cont_uuid",
    8,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Chk__CheckReport, cont_uuid),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "objid",
    9,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Chk__CheckReport, objid),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "dkey",
    10,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Chk__CheckReport, dkey),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "akey",
    11,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Chk__CheckReport, akey),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "timestamp",
    12,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Chk__CheckReport, timestamp),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "msg",
    13,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Chk__CheckReport, msg),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "act_choices",
    14,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_ENUM,
    offsetof(Chk__CheckReport, n_act_choices),
    offsetof(Chk__CheckReport, act_choices),
    &chk__check_inconsist_action__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_PACKED,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "act_details",
    15,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_STRING,
    offsetof(Chk__CheckReport, n_act_details),
    offsetof(Chk__CheckReport, act_details),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "act_msgs",
    16,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_STRING,
    offsetof(Chk__CheckReport, n_act_msgs),
    offsetof(Chk__CheckReport, act_msgs),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned chk__check_report__field_indices_by_name[] = {
  13,   /* field[13] = act_choices */
  14,   /* field[14] = act_details */
  15,   /* field[15] = act_msgs */
  2,   /* field[2] = action */
  10,   /* field[10] = akey */
  1,   /* field[1] = class */
  7,   /* field[7] = cont_uuid */
  9,   /* field[9] = dkey */
  12,   /* field[12] = msg */
  8,   /* field[8] = objid */
  6,   /* field[6] = pool_uuid */
  4,   /* field[4] = rank */
  3,   /* field[3] = result */
  0,   /* field[0] = seq */
  5,   /* field[5] = target */
  11,   /* field[11] = timestamp */
};
static const ProtobufCIntRange chk__check_report__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 16 }
};
const ProtobufCMessageDescriptor chk__check_report__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "chk.CheckReport",
  "CheckReport",
  "Chk__CheckReport",
  "chk",
  sizeof(Chk__CheckReport),
  16,
  chk__check_report__field_descriptors,
  chk__check_report__field_indices_by_name,
  1,  chk__check_report__number_ranges,
  (ProtobufCMessageInit) chk__check_report__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCEnumValue chk__check_inconsist_class__enum_values_by_number[22] =
{
  { "CIC_NONE", "CHK__CHECK_INCONSIST_CLASS__CIC_NONE", 0 },
  { "CIC_POOL_LESS_SVC_WITH_QUORUM", "CHK__CHECK_INCONSIST_CLASS__CIC_POOL_LESS_SVC_WITH_QUORUM", 1 },
  { "CIC_POOL_LESS_SVC_WITHOUT_QUORUM", "CHK__CHECK_INCONSIST_CLASS__CIC_POOL_LESS_SVC_WITHOUT_QUORUM", 2 },
  { "CIC_POOL_MORE_SVC", "CHK__CHECK_INCONSIST_CLASS__CIC_POOL_MORE_SVC", 3 },
  { "CIC_POOL_NONEXIST_ON_MS", "CHK__CHECK_INCONSIST_CLASS__CIC_POOL_NONEXIST_ON_MS", 4 },
  { "CIC_POOL_NONEXIST_ON_ENGINE", "CHK__CHECK_INCONSIST_CLASS__CIC_POOL_NONEXIST_ON_ENGINE", 5 },
  { "CIC_POOL_BAD_SVCL", "CHK__CHECK_INCONSIST_CLASS__CIC_POOL_BAD_SVCL", 6 },
  { "CIC_POOL_BAD_LABEL", "CHK__CHECK_INCONSIST_CLASS__CIC_POOL_BAD_LABEL", 7 },
  { "CIC_ENGINE_NONEXIST_IN_MAP", "CHK__CHECK_INCONSIST_CLASS__CIC_ENGINE_NONEXIST_IN_MAP", 8 },
  { "CIC_ENGINE_DOWN_IN_MAP", "CHK__CHECK_INCONSIST_CLASS__CIC_ENGINE_DOWN_IN_MAP", 9 },
  { "CIC_ENGINE_HAS_NO_STORAGE", "CHK__CHECK_INCONSIST_CLASS__CIC_ENGINE_HAS_NO_STORAGE", 10 },
  { "CIC_CONT_NONEXIST_ON_PS", "CHK__CHECK_INCONSIST_CLASS__CIC_CONT_NONEXIST_ON_PS", 11 },
  { "CIC_CONT_BAD_LABEL", "CHK__CHECK_INCONSIST_CLASS__CIC_CONT_BAD_LABEL", 12 },
  { "CIC_DTX_CORRUPTED", "CHK__CHECK_INCONSIST_CLASS__CIC_DTX_CORRUPTED", 13 },
  { "CIC_DTX_ORPHAN", "CHK__CHECK_INCONSIST_CLASS__CIC_DTX_ORPHAN", 14 },
  { "CIC_CSUM_LOST", "CHK__CHECK_INCONSIST_CLASS__CIC_CSUM_LOST", 15 },
  { "CIC_CSUM_FAILURE", "CHK__CHECK_INCONSIST_CLASS__CIC_CSUM_FAILURE", 16 },
  { "CIC_OBJ_LOST_REP", "CHK__CHECK_INCONSIST_CLASS__CIC_OBJ_LOST_REP", 17 },
  { "CIC_OBJ_LOST_EC_SHARD", "CHK__CHECK_INCONSIST_CLASS__CIC_OBJ_LOST_EC_SHARD", 18 },
  { "CIC_OBJ_LOST_EC_DATA", "CHK__CHECK_INCONSIST_CLASS__CIC_OBJ_LOST_EC_DATA", 19 },
  { "CIC_OBJ_DATA_INCONSIST", "CHK__CHECK_INCONSIST_CLASS__CIC_OBJ_DATA_INCONSIST", 20 },
  { "CIC_UNKNOWN", "CHK__CHECK_INCONSIST_CLASS__CIC_UNKNOWN", 100 },
};
static const ProtobufCIntRange chk__check_inconsist_class__value_ranges[] = {
{0, 0},{100, 21},{0, 22}
};
static const ProtobufCEnumValueIndex chk__check_inconsist_class__enum_values_by_name[22] =
{
  { "CIC_CONT_BAD_LABEL", 12 },
  { "CIC_CONT_NONEXIST_ON_PS", 11 },
  { "CIC_CSUM_FAILURE", 16 },
  { "CIC_CSUM_LOST", 15 },
  { "CIC_DTX_CORRUPTED", 13 },
  { "CIC_DTX_ORPHAN", 14 },
  { "CIC_ENGINE_DOWN_IN_MAP", 9 },
  { "CIC_ENGINE_HAS_NO_STORAGE", 10 },
  { "CIC_ENGINE_NONEXIST_IN_MAP", 8 },
  { "CIC_NONE", 0 },
  { "CIC_OBJ_DATA_INCONSIST", 20 },
  { "CIC_OBJ_LOST_EC_DATA", 19 },
  { "CIC_OBJ_LOST_EC_SHARD", 18 },
  { "CIC_OBJ_LOST_REP", 17 },
  { "CIC_POOL_BAD_LABEL", 7 },
  { "CIC_POOL_BAD_SVCL", 6 },
  { "CIC_POOL_LESS_SVC_WITHOUT_QUORUM", 2 },
  { "CIC_POOL_LESS_SVC_WITH_QUORUM", 1 },
  { "CIC_POOL_MORE_SVC", 3 },
  { "CIC_POOL_NONEXIST_ON_ENGINE", 5 },
  { "CIC_POOL_NONEXIST_ON_MS", 4 },
  { "CIC_UNKNOWN", 21 },
};
const ProtobufCEnumDescriptor chk__check_inconsist_class__descriptor =
{
  PROTOBUF_C__ENUM_DESCRIPTOR_MAGIC,
  "chk.CheckInconsistClass",
  "CheckInconsistClass",
  "Chk__CheckInconsistClass",
  "chk",
  22,
  chk__check_inconsist_class__enum_values_by_number,
  22,
  chk__check_inconsist_class__enum_values_by_name,
  2,
  chk__check_inconsist_class__value_ranges,
  NULL,NULL,NULL,NULL   /* reserved[1234] */
};
static const ProtobufCEnumValue chk__check_inconsist_action__enum_values_by_number[13] =
{
  { "CIA_DEFAULT", "CHK__CHECK_INCONSIST_ACTION__CIA_DEFAULT", 0 },
  { "CIA_INTERACT", "CHK__CHECK_INCONSIST_ACTION__CIA_INTERACT", 1 },
  { "CIA_IGNORE", "CHK__CHECK_INCONSIST_ACTION__CIA_IGNORE", 2 },
  { "CIA_DISCARD", "CHK__CHECK_INCONSIST_ACTION__CIA_DISCARD", 3 },
  { "CIA_READD", "CHK__CHECK_INCONSIST_ACTION__CIA_READD", 4 },
  { "CIA_TRUST_MS", "CHK__CHECK_INCONSIST_ACTION__CIA_TRUST_MS", 5 },
  { "CIA_TRUST_PS", "CHK__CHECK_INCONSIST_ACTION__CIA_TRUST_PS", 6 },
  { "CIA_TRUST_TARGET", "CHK__CHECK_INCONSIST_ACTION__CIA_TRUST_TARGET", 7 },
  { "CIA_TRUST_MAJORITY", "CHK__CHECK_INCONSIST_ACTION__CIA_TRUST_MAJORITY", 8 },
  { "CIA_TRUST_LATEST", "CHK__CHECK_INCONSIST_ACTION__CIA_TRUST_LATEST", 9 },
  { "CIA_TRUST_OLDEST", "CHK__CHECK_INCONSIST_ACTION__CIA_TRUST_OLDEST", 10 },
  { "CIA_TRUST_EC_PARITY", "CHK__CHECK_INCONSIST_ACTION__CIA_TRUST_EC_PARITY", 11 },
  { "CIA_TRUST_EC_DATA", "CHK__CHECK_INCONSIST_ACTION__CIA_TRUST_EC_DATA", 12 },
};
static const ProtobufCIntRange chk__check_inconsist_action__value_ranges[] = {
{0, 0},{0, 13}
};
static const ProtobufCEnumValueIndex chk__check_inconsist_action__enum_values_by_name[13] =
{
  { "CIA_DEFAULT", 0 },
  { "CIA_DISCARD", 3 },
  { "CIA_IGNORE", 2 },
  { "CIA_INTERACT", 1 },
  { "CIA_READD", 4 },
  { "CIA_TRUST_EC_DATA", 12 },
  { "CIA_TRUST_EC_PARITY", 11 },
  { "CIA_TRUST_LATEST", 9 },
  { "CIA_TRUST_MAJORITY", 8 },
  { "CIA_TRUST_MS", 5 },
  { "CIA_TRUST_OLDEST", 10 },
  { "CIA_TRUST_PS", 6 },
  { "CIA_TRUST_TARGET", 7 },
};
const ProtobufCEnumDescriptor chk__check_inconsist_action__descriptor =
{
  PROTOBUF_C__ENUM_DESCRIPTOR_MAGIC,
  "chk.CheckInconsistAction",
  "CheckInconsistAction",
  "Chk__CheckInconsistAction",
  "chk",
  13,
  chk__check_inconsist_action__enum_values_by_number,
  13,
  chk__check_inconsist_action__enum_values_by_name,
  1,
  chk__check_inconsist_action__value_ranges,
  NULL,NULL,NULL,NULL   /* reserved[1234] */
};
static const ProtobufCEnumValue chk__check_flag__enum_values_by_number[8] =
{
  { "CF_NONE", "CHK__CHECK_FLAG__CF_NONE", 0 },
  { "CF_DRYRUN", "CHK__CHECK_FLAG__CF_DRYRUN", 1 },
  { "CF_RESET", "CHK__CHECK_FLAG__CF_RESET", 2 },
  { "CF_FAILOUT", "CHK__CHECK_FLAG__CF_FAILOUT", 4 },
  { "CF_AUTO", "CHK__CHECK_FLAG__CF_AUTO", 8 },
  { "CF_DANGLING_POOL", "CHK__CHECK_FLAG__CF_DANGLING_POOL", 16 },
  { "CF_NO_FAILOUT", "CHK__CHECK_FLAG__CF_NO_FAILOUT", 32 },
  { "CF_NO_AUTO", "CHK__CHECK_FLAG__CF_NO_AUTO", 64 },
};
static const ProtobufCIntRange chk__check_flag__value_ranges[] = {
{0, 0},{4, 3},{8, 4},{16, 5},{32, 6},{64, 7},{0, 8}
};
static const ProtobufCEnumValueIndex chk__check_flag__enum_values_by_name[8] =
{
  { "CF_AUTO", 4 },
  { "CF_DANGLING_POOL", 5 },
  { "CF_DRYRUN", 1 },
  { "CF_FAILOUT", 3 },
  { "CF_NONE", 0 },
  { "CF_NO_AUTO", 7 },
  { "CF_NO_FAILOUT", 6 },
  { "CF_RESET", 2 },
};
const ProtobufCEnumDescriptor chk__check_flag__descriptor =
{
  PROTOBUF_C__ENUM_DESCRIPTOR_MAGIC,
  "chk.CheckFlag",
  "CheckFlag",
  "Chk__CheckFlag",
  "chk",
  8,
  chk__check_flag__enum_values_by_number,
  8,
  chk__check_flag__enum_values_by_name,
  6,
  chk__check_flag__value_ranges,
  NULL,NULL,NULL,NULL   /* reserved[1234] */
};
static const ProtobufCEnumValue chk__check_inst_status__enum_values_by_number[7] =
{
  { "CIS_INIT", "CHK__CHECK_INST_STATUS__CIS_INIT", 0 },
  { "CIS_RUNNING", "CHK__CHECK_INST_STATUS__CIS_RUNNING", 1 },
  { "CIS_COMPLETED", "CHK__CHECK_INST_STATUS__CIS_COMPLETED", 2 },
  { "CIS_STOPPED", "CHK__CHECK_INST_STATUS__CIS_STOPPED", 3 },
  { "CIS_FAILED", "CHK__CHECK_INST_STATUS__CIS_FAILED", 4 },
  { "CIS_PAUSED", "CHK__CHECK_INST_STATUS__CIS_PAUSED", 5 },
  { "CIS_IMPLICATED", "CHK__CHECK_INST_STATUS__CIS_IMPLICATED", 6 },
};
static const ProtobufCIntRange chk__check_inst_status__value_ranges[] = {
{0, 0},{0, 7}
};
static const ProtobufCEnumValueIndex chk__check_inst_status__enum_values_by_name[7] =
{
  { "CIS_COMPLETED", 2 },
  { "CIS_FAILED", 4 },
  { "CIS_IMPLICATED", 6 },
  { "CIS_INIT", 0 },
  { "CIS_PAUSED", 5 },
  { "CIS_RUNNING", 1 },
  { "CIS_STOPPED", 3 },
};
const ProtobufCEnumDescriptor chk__check_inst_status__descriptor =
{
  PROTOBUF_C__ENUM_DESCRIPTOR_MAGIC,
  "chk.CheckInstStatus",
  "CheckInstStatus",
  "Chk__CheckInstStatus",
  "chk",
  7,
  chk__check_inst_status__enum_values_by_number,
  7,
  chk__check_inst_status__enum_values_by_name,
  1,
  chk__check_inst_status__value_ranges,
  NULL,NULL,NULL,NULL   /* reserved[1234] */
};
static const ProtobufCEnumValue chk__check_pool_status__enum_values_by_number[8] =
{
  { "CPS_UNCHECKED", "CHK__CHECK_POOL_STATUS__CPS_UNCHECKED", 0 },
  { "CPS_CHECKING", "CHK__CHECK_POOL_STATUS__CPS_CHECKING", 1 },
  { "CPS_CHECKED", "CHK__CHECK_POOL_STATUS__CPS_CHECKED", 2 },
  { "CPS_FAILED", "CHK__CHECK_POOL_STATUS__CPS_FAILED", 3 },
  { "CPS_PAUSED", "CHK__CHECK_POOL_STATUS__CPS_PAUSED", 4 },
  { "CPS_PENDING", "CHK__CHECK_POOL_STATUS__CPS_PENDING", 5 },
  { "CPS_STOPPED", "CHK__CHECK_POOL_STATUS__CPS_STOPPED", 6 },
  { "CPS_IMPLICATED", "CHK__CHECK_POOL_STATUS__CPS_IMPLICATED", 7 },
};
static const ProtobufCIntRange chk__check_pool_status__value_ranges[] = {
{0, 0},{0, 8}
};
static const ProtobufCEnumValueIndex chk__check_pool_status__enum_values_by_name[8] =
{
  { "CPS_CHECKED", 2 },
  { "CPS_CHECKING", 1 },
  { "CPS_FAILED", 3 },
  { "CPS_IMPLICATED", 7 },
  { "CPS_PAUSED", 4 },
  { "CPS_PENDING", 5 },
  { "CPS_STOPPED", 6 },
  { "CPS_UNCHECKED", 0 },
};
const ProtobufCEnumDescriptor chk__check_pool_status__descriptor =
{
  PROTOBUF_C__ENUM_DESCRIPTOR_MAGIC,
  "chk.CheckPoolStatus",
  "CheckPoolStatus",
  "Chk__CheckPoolStatus",
  "chk",
  8,
  chk__check_pool_status__enum_values_by_number,
  8,
  chk__check_pool_status__enum_values_by_name,
  1,
  chk__check_pool_status__value_ranges,
  NULL,NULL,NULL,NULL   /* reserved[1234] */
};
static const ProtobufCEnumValue chk__check_scan_phase__enum_values_by_number[11] =
{
  { "CSP_PREPARE", "CHK__CHECK_SCAN_PHASE__CSP_PREPARE", 0 },
  { "CSP_POOL_LIST", "CHK__CHECK_SCAN_PHASE__CSP_POOL_LIST", 1 },
  { "CSP_POOL_MBS", "CHK__CHECK_SCAN_PHASE__CSP_POOL_MBS", 2 },
  { "CSP_POOL_CLEANUP", "CHK__CHECK_SCAN_PHASE__CSP_POOL_CLEANUP", 3 },
  { "CSP_CONT_LIST", "CHK__CHECK_SCAN_PHASE__CSP_CONT_LIST", 4 },
  { "CSP_CONT_CLEANUP", "CHK__CHECK_SCAN_PHASE__CSP_CONT_CLEANUP", 5 },
  { "CSP_DTX_RESYNC", "CHK__CHECK_SCAN_PHASE__CSP_DTX_RESYNC", 6 },
  { "CSP_OBJ_SCRUB", "CHK__CHECK_SCAN_PHASE__CSP_OBJ_SCRUB", 7 },
  { "CSP_REBUILD", "CHK__CHECK_SCAN_PHASE__CSP_REBUILD", 8 },
  { "OSP_AGGREGATION", "CHK__CHECK_SCAN_PHASE__OSP_AGGREGATION", 9 },
  { "DSP_DONE", "CHK__CHECK_SCAN_PHASE__DSP_DONE", 10 },
};
static const ProtobufCIntRange chk__check_scan_phase__value_ranges[] = {
{0, 0},{0, 11}
};
static const ProtobufCEnumValueIndex chk__check_scan_phase__enum_values_by_name[11] =
{
  { "CSP_CONT_CLEANUP", 5 },
  { "CSP_CONT_LIST", 4 },
  { "CSP_DTX_RESYNC", 6 },
  { "CSP_OBJ_SCRUB", 7 },
  { "CSP_POOL_CLEANUP", 3 },
  { "CSP_POOL_LIST", 1 },
  { "CSP_POOL_MBS", 2 },
  { "CSP_PREPARE", 0 },
  { "CSP_REBUILD", 8 },
  { "DSP_DONE", 10 },
  { "OSP_AGGREGATION", 9 },
};
const ProtobufCEnumDescriptor chk__check_scan_phase__descriptor =
{
  PROTOBUF_C__ENUM_DESCRIPTOR_MAGIC,
  "chk.CheckScanPhase",
  "CheckScanPhase",
  "Chk__CheckScanPhase",
  "chk",
  11,
  chk__check_scan_phase__enum_values_by_number,
  11,
  chk__check_scan_phase__enum_values_by_name,
  1,
  chk__check_scan_phase__value_ranges,
  NULL,NULL,NULL,NULL   /* reserved[1234] */
};
