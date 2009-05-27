// file meant to be included in midg.c, NOT compiled on its own.

// MIDG messages not enabled
static unsigned char midgMsgID_1[] = MSG_OFF(1);
static unsigned char midgMsgID_2[] = MSG_OFF(2);
static unsigned char midgMsgID_3[] = MSG_OFF(3);
static unsigned char midgMsgID_15[] = MSG_OFF(15);
static unsigned char midgMsgID_20[] = MSG_OFF(20);
static unsigned char midgMsgID_21[] = MSG_OFF(21);
static unsigned char midgMsgID_22[] = MSG_OFF(22);
static unsigned char midgMsgID_23[] = MSG_OFF(23);
static unsigned char midgMsgID_25[] = MSG_OFF(25);
static unsigned char midgMsgID_26[] = MSG_OFF(26);
static unsigned char midgMsgID_27[] = MSG_OFF(27);

// Messages at 50 Hz
static unsigned char midgMsgID_10[] = MSG_DIV(10,1);
static unsigned char midgMsgID_12[] = MSG_DIV(12,1);

static unsigned char* midgConfigMsgs[] = {   midgMsgID_1
                                            ,midgMsgID_2
                                            ,midgMsgID_3
                                            ,midgMsgID_10
                                            ,midgMsgID_12
                                            ,midgMsgID_15
                                            ,midgMsgID_20
                                            ,midgMsgID_21
                                            ,midgMsgID_22
                                            ,midgMsgID_23
                                            ,midgMsgID_25
                                            ,midgMsgID_26
                                            ,midgMsgID_27
                                            };