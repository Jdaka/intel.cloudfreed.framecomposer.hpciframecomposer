#pragma once


/**
* h264 Input picture type
*/
typedef enum _NV_ENC_PIC_TYPE
{
	NV_ENC_PIC_TYPE_P = 0x0,     /**< Forward predicted */
	NV_ENC_PIC_TYPE_B = 0x01,    /**< Bi-directionally predicted picture */
	NV_ENC_PIC_TYPE_I = 0x02,    /**< Intra predicted picture */
	NV_ENC_PIC_TYPE_IDR = 0x03,    /**< IDR picture */
	NV_ENC_PIC_TYPE_BI = 0x04,    /**< Bi-directionally predicted with only Intra MBs */
	NV_ENC_PIC_TYPE_SKIPPED = 0x05,    /**< Picture is skipped */
	NV_ENC_PIC_TYPE_INTRA_REFRESH = 0x06,    /**< First picture in intra refresh cycle */
	NV_ENC_PIC_TYPE_UNKNOWN = 0xFF     /**< Picture type unknown */
} NV_ENC_PIC_TYPE;
