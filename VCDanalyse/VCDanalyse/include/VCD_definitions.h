#ifndef VCD_DEFINITIONS_H_INCLUDED
#define VCD_DEFINITIONS_H_INCLUDED

#define VCD_VAR_SYMBOLE         "$var"
#define VCD_TIMESCALE_SYMBOLE   "$timescale"
#define VCD_END_SYMBOLE         "$end"
#define VCD_DATE_SYMBOLE        "$date"
#define VCD_VERSION_SYMBOLE     "$version"
#define VCD_COMMENT_SYMBOLE     "$comment"
#define VCD_SCOPE_SYMBOLE       "$scope"
#define VCD_ENDDEFINITION_SYMBOLE "$enddefinitions"
#define VCD_DUMPVAR_SYMBOLE     "$dumpvars"

#define VCD_TIMESTAMP_SYMBOLE   '#'

#define VCD_NUMBER_BASE         10

#define VCD_ps                  "ps"
#define VCD_ns                  "ns"
#define VCD_us                  "us"
#define VCD_ms                  "ms"
#define VCD_s                   "s"

#define VCD_ps_value            (float)(1.0/1000000000000)
#define VCD_ns_value            (float)(1.0/1000000000)
#define VCD_us_value            (float)(1.0/1000000)
#define VCD_ms_value            (float)(1.0/1000)
#define VCD_s_value             (float)(1.0)

#endif // VCD_DEFINITIONS_H_INCLUDED
