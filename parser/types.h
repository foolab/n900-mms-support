#ifndef TYPES_H
#define TYPES_H

/*
 * Stolen from mms_decode.c mmsdec-0.5.2.tgz
 */

/*
 * Header field values
 */
/* MMS 1.0 */
#define MM_BCC_HDR		0x81	/* Bcc			*/
#define MM_CC_HDR		0x82	/* Cc			*/
#define MM_CLOCATION_HDR	0x83	/* X-Mms-Content-Location	*/
#define MM_CTYPE_HDR		0x84	/* Content-Type		*/
#define MM_DATE_HDR		0x85	/* Date				*/
#define MM_DREPORT_HDR		0x86	/* X-Mms-Delivery-Report	*/
#define MM_DTIME_HDR		0x87	/* X-Mms-Delivery-Time		*/
#define MM_EXPIRY_HDR		0x88	/* X-Mms-Expiry			*/
#define MM_FROM_HDR		0x89	/* From				*/
#define MM_MCLASS_HDR		0x8A	/* X-Mms-Message-Class		*/
#define MM_MID_HDR		0x8B	/* Message-ID			*/
#define MM_MTYPE_HDR		0x8C	/* X-Mms-Message-Type		*/
#define MM_VERSION_HDR		0x8D	/* X-Mms-MMS-Version		*/
#define MM_MSIZE_HDR		0x8E	/* X-Mms-Message-Size		*/
#define MM_PRIORITY_HDR		0x8F	/* X-Mms-Priority		*/
#define MM_RREPLY_HDR		0x90	/* X-Mms-Read-Reply		*/
#define MM_RALLOWED_HDR		0x91	/* X-Mms-Report-Allowed		*/
#define MM_RSTATUS_HDR		0x92	/* X-Mms-Response-Status	*/
#define MM_RTEXT_HDR		0x93	/* X-Mms-Response-Text		*/
#define MM_SVISIBILITY_HDR	0x94	/* X-Mms-Sender-Visibility	*/
#define MM_STATUS_HDR		0x95	/* X-Mms-Status			*/
#define MM_SUBJECT_HDR		0x96	/* Subject			*/
#define MM_TO_HDR		0x97	/* To				*/
#define MM_TID_HDR		0x98	/* X-Mms-Transaction-Id		*/
/* MMS 1.1 */
#define MM_RETRIEVE_STATUS_HDR	0x99	/* X-Mms-Retrieve-Status	*/
#define MM_RETRIEVE_TEXT_HDR	0x9A	/* X-Mms-Retrieve-Text		*/
#define MM_READ_STATUS_HDR	0x9B	/* X-Mms-Read-Status		*/
#define MM_REPLY_CHARGING_HDR	0x9C	/* X-Mms-Reply-Charging		*/
#define MM_REPLY_CHARGING_DEADLINE_HDR	\
				0x9D	/* X-Mms-Reply-Charging-Deadline*/
#define MM_REPLY_CHARGING_ID_HDR	\
				0x9E	/* X-Mms-Reply-Charging-ID	*/
#define MM_REPLY_CHARGING_SIZE_HDR	\
				0x9F	/* X-Mms-Reply-Charging-Size	*/
#define MM_PREV_SENT_BY_HDR	0xA0	/* X-Mms-Previously-Sent-By	*/
#define MM_PREV_SENT_DATE_HDR	0xA1	/* X-Mms-Previously-Sent-Date	*/
/* MMS 1.2 */
#define MM_STORE_HDR		0xA2	/* X-Mms-Store			*/
#define MM_MM_STATE_HDR		0xA3	/* X-Mms-MM-State		*/
#define MM_MM_FLAGS_HDR		0xA4	/* X-Mms-MM-Flags		*/
#define MM_STORE_STATUS_HDR	0xA5	/* X-Mms-Store-Status		*/
#define MM_STORE_STATUS_TEXT_HDR	\
				0xA6	/* X-Mms-Store-Status-Text	*/
#define MM_STORED_HDR		0xA7	/* X-Mms-Stored			*/
#define MM_ATTRIBUTES_HDR	0xA8	/* X-Mms-Attributes		*/
#define MM_TOTALS_HDR		0xA9	/* X-Mms-Totals			*/
#define MM_MBOX_TOTALS_HDR	0xAA	/* X-Mms-Mbox-Totals		*/
#define MM_QUOTAS_HDR		0xAB	/* X-Mms-Quotas			*/
#define MM_MBOX_QUOTAS_HDR	0xAC	/* X-Mms-Mbox-Quotas		*/
#define MM_MBOX_MSG_COUNT_HDR	0xAD	/* X-Mms-Message-Count		*/
#define MM_CONTENT_HDR		0xAE	/* Content			*/
#define MM_START_HDR		0xAF	/* X-Mms-Start			*/
#define MM_ADDITIONAL_HDR	0xB0	/* Additional-headers		*/
#define MM_DISTRIBUION_IND_HDR	0xB1	/* X-Mms-Distribution-Indcator	*/
#define MM_ELEMENT_DESCR_HDR	0xB2	/* X-Mms-Element-Descriptor	*/
#define MM_LIMIT_HDR		0xB3	/* X-Mms-Limit			*/

/*
 * Valuestrings for PDU types
 */
/* MMS 1.0 */
#define PDU_M_SEND_REQ		0x80
#define PDU_M_SEND_CONF		0x81
#define PDU_M_NOTIFICATION_IND	0x82
#define PDU_M_NOTIFYRESP_IND	0x83
#define PDU_M_RETRIEVE_CONF	0x84
#define PDU_M_ACKNOWLEDGE_IND	0x85
#define PDU_M_DELIVERY_IND	0x86
/* MMS 1.1 */
#define PDU_M_READ_REC_IND	0x87
#define PDU_M_READ_ORIG_IND	0x88
#define PDU_M_FORWARD_REQ	0x89
#define PDU_M_FORWARD_CONF	0x8A
/* MMS 1.2 */
#define PDU_M_MBOX_STORE_REQ	0x8B
#define PDU_M_MBOX_STORE_CONF	0x8C
#define PDU_M_MBOX_VIEW_REQ	0x8D
#define PDU_M_MBOX_VIEW_CONF	0x8E
#define PDU_M_MBOX_UPLOAD_REQ	0x8F
#define PDU_M_MBOX_UPLOAD_CONF	0x90
#define PDU_M_MBOX_DELETE_REQ	0x91
#define PDU_M_MBOX_DELETE_CONF	0x92
#define PDU_M_MBOX_DESCR	0x93

#define MM_QUOTE                0x22
#define MM_ABSOL_TOKEN          0x80
#define MM_RELAT_TOKEN          0x81


/* Typed parameter */
#define MMT_Q                   0x00
#define MMT_CHARSET             0x01
#define MMT_LEVEL               0x02
#define MMT_TYPE                0x03
#define MMT_NAME                0x05
#define MMT_FILENAME            0x06
#define MMT_DIFFERENCES         0x07
#define MMT_PADDING             0x08
#define MMT_TYPE_SPEC           0x09
#define MMT_START               0x0A
#define MMT_START_INFO          0x0B
#define MMT_COMMENT             0x0C
#define MMT_DOMAIN              0x0D
#define MMT_MAX_AGE             0x0E
#define MMT_PATH                0x0F
#define MMT_SECURE              0x10
#define MMT_SEC                 0x11
#define MMT_MAC                 0x12
#define MMT_CREATION_DATE       0x13
#define MMT_MODIFI_DATE         0x14
#define MMT_READ_DATE           0x15
#define MMT_SIZE                0x16
#define MMT_NAME_VAL            0x17
#define MMT_FILENAME_VAL        0x18
#define MMT_START_MULTI         0x19
#define MMT_START_INFO_MULTI    0x1A
#define MMT_COMMENT_VAL         0x1B
#define MMT_DOMAIN_VAL          0x1C
#define MMT_PATH_VAL            0x1D

#endif /* TYPES_H */
