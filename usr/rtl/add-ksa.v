`default_nettype none

module black(
  input  wire i_pj,
  input  wire i_gj,
  input  wire i_pk,
  input  wire i_gk,
  output wire o_g,
  output wire o_p
);

assign o_g = i_gk | (i_gj & i_pk);
assign o_p = i_pk & i_pj;

endmodule
`default_nettype none

module grey(
  input  wire i_gj,
  input  wire i_pk,
  input  wire i_gk,
  output wire o_g
);

assign o_g = i_gk | (i_gj & i_pk);

endmodule
`default_nettype none

module ks_1(
  input i_c0,
  input  wire [31:0]i_a,
  input  wire [31:0]i_b,
  output wire [31:0]o_pk_1,
  output wire [31:0]o_gk_1,
  output o_c0_1
);

assign o_c0_1 = i_c0;

pg pg_0(i_a[0], i_b[0], o_pk_1[0], o_gk_1[0]);
pg pg_1(i_a[1], i_b[1], o_pk_1[1], o_gk_1[1]);
pg pg_2(i_a[2], i_b[2], o_pk_1[2], o_gk_1[2]);
pg pg_3(i_a[3], i_b[3], o_pk_1[3], o_gk_1[3]);
pg pg_4(i_a[4], i_b[4], o_pk_1[4], o_gk_1[4]);
pg pg_5(i_a[5], i_b[5], o_pk_1[5], o_gk_1[5]);
pg pg_6(i_a[6], i_b[6], o_pk_1[6], o_gk_1[6]);
pg pg_7(i_a[7], i_b[7], o_pk_1[7], o_gk_1[7]);
pg pg_8(i_a[8], i_b[8], o_pk_1[8], o_gk_1[8]);
pg pg_9(i_a[9], i_b[9], o_pk_1[9], o_gk_1[9]);
pg pg_10(i_a[10], i_b[10], o_pk_1[10], o_gk_1[10]);
pg pg_11(i_a[11], i_b[11], o_pk_1[11], o_gk_1[11]);
pg pg_12(i_a[12], i_b[12], o_pk_1[12], o_gk_1[12]);
pg pg_13(i_a[13], i_b[13], o_pk_1[13], o_gk_1[13]);
pg pg_14(i_a[14], i_b[14], o_pk_1[14], o_gk_1[14]);
pg pg_15(i_a[15], i_b[15], o_pk_1[15], o_gk_1[15]);
pg pg_16(i_a[16], i_b[16], o_pk_1[16], o_gk_1[16]);
pg pg_17(i_a[17], i_b[17], o_pk_1[17], o_gk_1[17]);
pg pg_18(i_a[18], i_b[18], o_pk_1[18], o_gk_1[18]);
pg pg_19(i_a[19], i_b[19], o_pk_1[19], o_gk_1[19]);
pg pg_20(i_a[20], i_b[20], o_pk_1[20], o_gk_1[20]);
pg pg_21(i_a[21], i_b[21], o_pk_1[21], o_gk_1[21]);
pg pg_22(i_a[22], i_b[22], o_pk_1[22], o_gk_1[22]);
pg pg_23(i_a[23], i_b[23], o_pk_1[23], o_gk_1[23]);
pg pg_24(i_a[24], i_b[24], o_pk_1[24], o_gk_1[24]);
pg pg_25(i_a[25], i_b[25], o_pk_1[25], o_gk_1[25]);
pg pg_26(i_a[26], i_b[26], o_pk_1[26], o_gk_1[26]);
pg pg_27(i_a[27], i_b[27], o_pk_1[27], o_gk_1[27]);
pg pg_28(i_a[28], i_b[28], o_pk_1[28], o_gk_1[28]);
pg pg_29(i_a[29], i_b[29], o_pk_1[29], o_gk_1[29]);
pg pg_30(i_a[30], i_b[30], o_pk_1[30], o_gk_1[30]);
pg pg_31(i_a[31], i_b[31], o_pk_1[31], o_gk_1[31]);

endmodule
`default_nettype none

module ks_2(
  input  wire        i_c0,
  input  wire [31:0] i_pk,
  input  wire [31:0] i_gk,
  output wire        o_c0,
  output wire [30:0] o_pk,
  output wire [31:0] o_gk,
  output wire [31:0] o_p_save
);

wire [31:0] gkj;
wire [30:0] pkj;

assign o_c0      = i_c0;
assign o_p_save  = i_pk[31:0];
assign gkj[0]    = i_c0;
assign gkj[31:1] = i_gk[30:0];
assign pkj       = i_pk[30:0];

grey gc_0(gkj[0], i_pk[0], i_gk[0], o_gk[0]);
black bc_0(pkj[0], gkj[1], i_pk[1], i_gk[1], o_gk[1], o_pk[0]);
black bc_1(pkj[1], gkj[2], i_pk[2], i_gk[2], o_gk[2], o_pk[1]);
black bc_2(pkj[2], gkj[3], i_pk[3], i_gk[3], o_gk[3], o_pk[2]);
black bc_3(pkj[3], gkj[4], i_pk[4], i_gk[4], o_gk[4], o_pk[3]);
black bc_4(pkj[4], gkj[5], i_pk[5], i_gk[5], o_gk[5], o_pk[4]);
black bc_5(pkj[5], gkj[6], i_pk[6], i_gk[6], o_gk[6], o_pk[5]);
black bc_6(pkj[6], gkj[7], i_pk[7], i_gk[7], o_gk[7], o_pk[6]);
black bc_7(pkj[7], gkj[8], i_pk[8], i_gk[8], o_gk[8], o_pk[7]);
black bc_8(pkj[8], gkj[9], i_pk[9], i_gk[9], o_gk[9], o_pk[8]);
black bc_9(pkj[9], gkj[10], i_pk[10], i_gk[10], o_gk[10], o_pk[9]);
black bc_10(pkj[10], gkj[11], i_pk[11], i_gk[11], o_gk[11], o_pk[10]);
black bc_11(pkj[11], gkj[12], i_pk[12], i_gk[12], o_gk[12], o_pk[11]);
black bc_12(pkj[12], gkj[13], i_pk[13], i_gk[13], o_gk[13], o_pk[12]);
black bc_13(pkj[13], gkj[14], i_pk[14], i_gk[14], o_gk[14], o_pk[13]);
black bc_14(pkj[14], gkj[15], i_pk[15], i_gk[15], o_gk[15], o_pk[14]);
black bc_15(pkj[15], gkj[16], i_pk[16], i_gk[16], o_gk[16], o_pk[15]);
black bc_16(pkj[16], gkj[17], i_pk[17], i_gk[17], o_gk[17], o_pk[16]);
black bc_17(pkj[17], gkj[18], i_pk[18], i_gk[18], o_gk[18], o_pk[17]);
black bc_18(pkj[18], gkj[19], i_pk[19], i_gk[19], o_gk[19], o_pk[18]);
black bc_19(pkj[19], gkj[20], i_pk[20], i_gk[20], o_gk[20], o_pk[19]);
black bc_20(pkj[20], gkj[21], i_pk[21], i_gk[21], o_gk[21], o_pk[20]);
black bc_21(pkj[21], gkj[22], i_pk[22], i_gk[22], o_gk[22], o_pk[21]);
black bc_22(pkj[22], gkj[23], i_pk[23], i_gk[23], o_gk[23], o_pk[22]);
black bc_23(pkj[23], gkj[24], i_pk[24], i_gk[24], o_gk[24], o_pk[23]);
black bc_24(pkj[24], gkj[25], i_pk[25], i_gk[25], o_gk[25], o_pk[24]);
black bc_25(pkj[25], gkj[26], i_pk[26], i_gk[26], o_gk[26], o_pk[25]);
black bc_26(pkj[26], gkj[27], i_pk[27], i_gk[27], o_gk[27], o_pk[26]);
black bc_27(pkj[27], gkj[28], i_pk[28], i_gk[28], o_gk[28], o_pk[27]);
black bc_28(pkj[28], gkj[29], i_pk[29], i_gk[29], o_gk[29], o_pk[28]);
black bc_29(pkj[29], gkj[30], i_pk[30], i_gk[30], o_gk[30], o_pk[29]);
black bc_30(pkj[30], gkj[31], i_pk[31], i_gk[31], o_gk[31], o_pk[30]);

endmodule
`default_nettype none

module ks_3(
  input  wire        i_c0,
  input  wire [30:0] i_pk,
  input  wire [31:0] i_gk,
  input  wire [31:0] i_p_save,
  output wire        o_c0,
  output wire [28:0] o_pk,
  output wire [31:0] o_gk,
  output wire [31:0] o_p_save
);

wire [30:0] gkj;
wire [28:0] pkj;

assign o_c0      = i_c0;
assign o_p_save  = i_p_save[31:0];
assign gkj[0]    = i_c0;
assign gkj[30:1] = i_gk[29:0];
assign pkj       = i_pk[28:0];
assign o_gk[0]   = i_gk[0];

grey gc_0(gkj[0], i_pk[0], i_gk[1], o_gk[1]);
grey gc_1(gkj[1], i_pk[1], i_gk[2], o_gk[2]);
black bc_0(pkj[0], gkj[2], i_pk[2], i_gk[3], o_gk[3], o_pk[0]);
black bc_1(pkj[1], gkj[3], i_pk[3], i_gk[4], o_gk[4], o_pk[1]);
black bc_2(pkj[2], gkj[4], i_pk[4], i_gk[5], o_gk[5], o_pk[2]);
black bc_3(pkj[3], gkj[5], i_pk[5], i_gk[6], o_gk[6], o_pk[3]);
black bc_4(pkj[4], gkj[6], i_pk[6], i_gk[7], o_gk[7], o_pk[4]);
black bc_5(pkj[5], gkj[7], i_pk[7], i_gk[8], o_gk[8], o_pk[5]);
black bc_6(pkj[6], gkj[8], i_pk[8], i_gk[9], o_gk[9], o_pk[6]);
black bc_7(pkj[7], gkj[9], i_pk[9], i_gk[10], o_gk[10], o_pk[7]);
black bc_8(pkj[8], gkj[10], i_pk[10], i_gk[11], o_gk[11], o_pk[8]);
black bc_9(pkj[9], gkj[11], i_pk[11], i_gk[12], o_gk[12], o_pk[9]);
black bc_10(pkj[10], gkj[12], i_pk[12], i_gk[13], o_gk[13], o_pk[10]);
black bc_11(pkj[11], gkj[13], i_pk[13], i_gk[14], o_gk[14], o_pk[11]);
black bc_12(pkj[12], gkj[14], i_pk[14], i_gk[15], o_gk[15], o_pk[12]);
black bc_13(pkj[13], gkj[15], i_pk[15], i_gk[16], o_gk[16], o_pk[13]);
black bc_14(pkj[14], gkj[16], i_pk[16], i_gk[17], o_gk[17], o_pk[14]);
black bc_15(pkj[15], gkj[17], i_pk[17], i_gk[18], o_gk[18], o_pk[15]);
black bc_16(pkj[16], gkj[18], i_pk[18], i_gk[19], o_gk[19], o_pk[16]);
black bc_17(pkj[17], gkj[19], i_pk[19], i_gk[20], o_gk[20], o_pk[17]);
black bc_18(pkj[18], gkj[20], i_pk[20], i_gk[21], o_gk[21], o_pk[18]);
black bc_19(pkj[19], gkj[21], i_pk[21], i_gk[22], o_gk[22], o_pk[19]);
black bc_20(pkj[20], gkj[22], i_pk[22], i_gk[23], o_gk[23], o_pk[20]);
black bc_21(pkj[21], gkj[23], i_pk[23], i_gk[24], o_gk[24], o_pk[21]);
black bc_22(pkj[22], gkj[24], i_pk[24], i_gk[25], o_gk[25], o_pk[22]);
black bc_23(pkj[23], gkj[25], i_pk[25], i_gk[26], o_gk[26], o_pk[23]);
black bc_24(pkj[24], gkj[26], i_pk[26], i_gk[27], o_gk[27], o_pk[24]);
black bc_25(pkj[25], gkj[27], i_pk[27], i_gk[28], o_gk[28], o_pk[25]);
black bc_26(pkj[26], gkj[28], i_pk[28], i_gk[29], o_gk[29], o_pk[26]);
black bc_27(pkj[27], gkj[29], i_pk[29], i_gk[30], o_gk[30], o_pk[27]);
black bc_28(pkj[28], gkj[30], i_pk[30], i_gk[31], o_gk[31], o_pk[28]);

endmodule
`default_nettype none

module ks_4(
  input  wire        i_c0,
  input  wire [28:0] i_pk,
  input  wire [31:0] i_gk,
  input  wire [31:0] i_p_save,
  output wire        o_c0,
  output wire [24:0] o_pk,
  output wire [31:0] o_gk,
  output wire [31:0] o_p_save
);

wire [28:0] gkj;
wire [24:0] pkj;

assign o_c0      = i_c0;
assign o_p_save  = i_p_save[31:0];
assign gkj[0]    = i_c0;
assign gkj[28:1] = i_gk[27:0];
assign pkj       = i_pk[24:0];
assign o_gk[2:0] = i_gk[2:0];

grey gc_0(gkj[0], i_pk[0], i_gk[3], o_gk[3]);
grey gc_1(gkj[1], i_pk[1], i_gk[4], o_gk[4]);
grey gc_2(gkj[2], i_pk[2], i_gk[5], o_gk[5]);
grey gc_3(gkj[3], i_pk[3], i_gk[6], o_gk[6]);
black bc_0(pkj[0], gkj[4], i_pk[4], i_gk[7], o_gk[7], o_pk[0]);
black bc_1(pkj[1], gkj[5], i_pk[5], i_gk[8], o_gk[8], o_pk[1]);
black bc_2(pkj[2], gkj[6], i_pk[6], i_gk[9], o_gk[9], o_pk[2]);
black bc_3(pkj[3], gkj[7], i_pk[7], i_gk[10], o_gk[10], o_pk[3]);
black bc_4(pkj[4], gkj[8], i_pk[8], i_gk[11], o_gk[11], o_pk[4]);
black bc_5(pkj[5], gkj[9], i_pk[9], i_gk[12], o_gk[12], o_pk[5]);
black bc_6(pkj[6], gkj[10], i_pk[10], i_gk[13], o_gk[13], o_pk[6]);
black bc_7(pkj[7], gkj[11], i_pk[11], i_gk[14], o_gk[14], o_pk[7]);
black bc_8(pkj[8], gkj[12], i_pk[12], i_gk[15], o_gk[15], o_pk[8]);
black bc_9(pkj[9], gkj[13], i_pk[13], i_gk[16], o_gk[16], o_pk[9]);
black bc_10(pkj[10], gkj[14], i_pk[14], i_gk[17], o_gk[17], o_pk[10]);
black bc_11(pkj[11], gkj[15], i_pk[15], i_gk[18], o_gk[18], o_pk[11]);
black bc_12(pkj[12], gkj[16], i_pk[16], i_gk[19], o_gk[19], o_pk[12]);
black bc_13(pkj[13], gkj[17], i_pk[17], i_gk[20], o_gk[20], o_pk[13]);
black bc_14(pkj[14], gkj[18], i_pk[18], i_gk[21], o_gk[21], o_pk[14]);
black bc_15(pkj[15], gkj[19], i_pk[19], i_gk[22], o_gk[22], o_pk[15]);
black bc_16(pkj[16], gkj[20], i_pk[20], i_gk[23], o_gk[23], o_pk[16]);
black bc_17(pkj[17], gkj[21], i_pk[21], i_gk[24], o_gk[24], o_pk[17]);
black bc_18(pkj[18], gkj[22], i_pk[22], i_gk[25], o_gk[25], o_pk[18]);
black bc_19(pkj[19], gkj[23], i_pk[23], i_gk[26], o_gk[26], o_pk[19]);
black bc_20(pkj[20], gkj[24], i_pk[24], i_gk[27], o_gk[27], o_pk[20]);
black bc_21(pkj[21], gkj[25], i_pk[25], i_gk[28], o_gk[28], o_pk[21]);
black bc_22(pkj[22], gkj[26], i_pk[26], i_gk[29], o_gk[29], o_pk[22]);
black bc_23(pkj[23], gkj[27], i_pk[27], i_gk[30], o_gk[30], o_pk[23]);
black bc_24(pkj[24], gkj[28], i_pk[28], i_gk[31], o_gk[31], o_pk[24]);

endmodule
module ks_5(
  input  wire        i_c0,
  input  wire [24:0] i_pk,
  input  wire [31:0] i_gk,
  input  wire [31:0] i_p_save,
  output wire        o_c0,
  output wire [16:0] o_pk,
  output wire [31:0] o_gk,
  output wire [31:0] o_p_save
);

wire [24:0] gkj;
wire [16:0] pkj;

assign o_c0      = i_c0;
assign o_p_save  = i_p_save[31:0];
assign gkj[0]    = i_c0;
assign gkj[24:1] = i_gk[23:0];
assign pkj       = i_pk[16:0];
assign o_gk[6:0] = i_gk[6:0];

grey gc_0(gkj[0], i_pk[0], i_gk[7], o_gk[7]);
grey gc_1(gkj[1], i_pk[1], i_gk[8], o_gk[8]);
grey gc_2(gkj[2], i_pk[2], i_gk[9], o_gk[9]);
grey gc_3(gkj[3], i_pk[3], i_gk[10], o_gk[10]);
grey gc_4(gkj[4], i_pk[4], i_gk[11], o_gk[11]);
grey gc_5(gkj[5], i_pk[5], i_gk[12], o_gk[12]);
grey gc_6(gkj[6], i_pk[6], i_gk[13], o_gk[13]);
grey gc_7(gkj[7], i_pk[7], i_gk[14], o_gk[14]);
black bc_0(pkj[0], gkj[8], i_pk[8], i_gk[15], o_gk[15], o_pk[0]);
black bc_1(pkj[1], gkj[9], i_pk[9], i_gk[16], o_gk[16], o_pk[1]);
black bc_2(pkj[2], gkj[10], i_pk[10], i_gk[17], o_gk[17], o_pk[2]);
black bc_3(pkj[3], gkj[11], i_pk[11], i_gk[18], o_gk[18], o_pk[3]);
black bc_4(pkj[4], gkj[12], i_pk[12], i_gk[19], o_gk[19], o_pk[4]);
black bc_5(pkj[5], gkj[13], i_pk[13], i_gk[20], o_gk[20], o_pk[5]);
black bc_6(pkj[6], gkj[14], i_pk[14], i_gk[21], o_gk[21], o_pk[6]);
black bc_7(pkj[7], gkj[15], i_pk[15], i_gk[22], o_gk[22], o_pk[7]);
black bc_8(pkj[8], gkj[16], i_pk[16], i_gk[23], o_gk[23], o_pk[8]);
black bc_9(pkj[9], gkj[17], i_pk[17], i_gk[24], o_gk[24], o_pk[9]);
black bc_10(pkj[10], gkj[18], i_pk[18], i_gk[25], o_gk[25], o_pk[10]);
black bc_11(pkj[11], gkj[19], i_pk[19], i_gk[26], o_gk[26], o_pk[11]);
black bc_12(pkj[12], gkj[20], i_pk[20], i_gk[27], o_gk[27], o_pk[12]);
black bc_13(pkj[13], gkj[21], i_pk[21], i_gk[28], o_gk[28], o_pk[13]);
black bc_14(pkj[14], gkj[22], i_pk[22], i_gk[29], o_gk[29], o_pk[14]);
black bc_15(pkj[15], gkj[23], i_pk[23], i_gk[30], o_gk[30], o_pk[15]);
black bc_16(pkj[16], gkj[24], i_pk[24], i_gk[31], o_gk[31], o_pk[16]);

endmodule
`default_nettype none

module ks_6(
  input  wire        i_c0,
  input  wire [16:0] i_pk,
  input  wire [31:0] i_gk,
  input  wire [31:0] i_p_save,
  output wire        o_c0,
  output wire [31:0] o_pk,
  output wire [31:0] o_gk
);

wire [16:0] gkj;

assign o_c0       = i_c0;
assign o_pk       = i_p_save[31:0];
assign gkj[0]     = i_c0;
assign gkj[16:1]  = i_gk[15:0];
assign o_gk[15:0] = i_gk[15:0];

grey gc_0(gkj[0], i_pk[0], i_gk[15], o_gk[15]);
grey gc_1(gkj[1], i_pk[1], i_gk[16], o_gk[16]);
grey gc_2(gkj[2], i_pk[2], i_gk[17], o_gk[17]);
grey gc_3(gkj[3], i_pk[3], i_gk[18], o_gk[18]);
grey gc_4(gkj[4], i_pk[4], i_gk[19], o_gk[19]);
grey gc_5(gkj[5], i_pk[5], i_gk[20], o_gk[20]);
grey gc_6(gkj[6], i_pk[6], i_gk[21], o_gk[21]);
grey gc_7(gkj[7], i_pk[7], i_gk[22], o_gk[22]);
grey gc_8(gkj[8], i_pk[8], i_gk[23], o_gk[23]);
grey gc_9(gkj[9], i_pk[9], i_gk[24], o_gk[24]);
grey gc_10(gkj[10], i_pk[10], i_gk[25], o_gk[25]);
grey gc_11(gkj[11], i_pk[11], i_gk[26], o_gk[26]);
grey gc_12(gkj[12], i_pk[12], i_gk[27], o_gk[27]);
grey gc_13(gkj[13], i_pk[13], i_gk[28], o_gk[28]);
grey gc_14(gkj[14], i_pk[14], i_gk[29], o_gk[29]);
grey gc_15(gkj[15], i_pk[15], i_gk[30], o_gk[30]);
grey gc_16(gkj[16], i_pk[16], i_gk[31], o_gk[31]);

endmodule
`default_nettype none

module ks_7(
  input  wire        i_c0,
  input  wire [31:0] i_pk,
  input  wire [31:0] i_gk,
  output wire [31:0] o_s,
  output wire        o_carry
);

assign o_carry   = i_gk[31];
assign o_s[0]    = i_c0 ^ i_pk[0];
assign o_s[31:1] = i_gk[30:0] ^ i_pk[31:1];

endmodule
`default_nettype none

module ksa_top(
  input  wire        c0,
  input  wire [31:0] i_a,
  input  wire [31:0] i_b,
  output wire [31:0] o_s,
  output wire        o_carry
);

wire [31:0] p1;
wire [31:0] g1;
wire        c1;

wire [30:0] p2;
wire [31:0] g2;
wire        c2;
wire [31:0] ps1;

wire [28:0] p3;
wire [31:0] g3;
wire        c3;
wire [31:0] ps2;

wire [24:0] p4;
wire [31:0] g4;
wire        c4;
wire [31:0] ps3;

wire [16:0] p5;
wire [31:0] g5;
wire        c5;
wire [31:0] ps4;

wire [31:0] p6;
wire [31:0] g6;
wire        c6;

ks_1 s1(c0, i_a, i_b, p1, g1, c1);
ks_2 s2(c1, p1, g1, c2, p2, g2, ps1);
ks_3 s3(c2, p2, g2, ps1, c3, p3, g3, ps2);
ks_4 s4(c3, p3, g3, ps2, c4, p4, g4, ps3);
ks_5 s5(c4, p4, g4, ps3, c5, p5, g5, ps4);
ks_6 s6(c5, p5, g5, ps4, c6, p6, g6);
ks_7 s7(c6, p6, g6, o_s, o_carry);

endmodule
`default_nettype none

module pg(
  input  wire i_a,
  input  wire i_b,
  output wire o_p,
  output wire o_g
);

assign o_p = i_a ^ i_b;
assign o_g = i_a & i_b;

endmodule
