#include "SDMMCHelpers.h"
#include <AnalyzerHelpers.h>

static struct MMCResponse responses[64] = {
	/*  0 */ {MMC_RSP_NONE,     0,   0, false, false, false},
	/*  1 */ {MMC_RSP_R3,      32,   5, false, false, false},
	/*  2 */ {MMC_RSP_R2_CID, 128,   5, false, false, false},
	/*  3 */ {MMC_RSP_R1,      32,  64, false, false, false},
	/*  4 */ {MMC_RSP_NONE,     0,   0, false, false, false},
	/*  5 */ {MMC_RSP_R1,      32,  64, true, false, false},
	/*  6 */ {MMC_RSP_R1,      32,  64, true, false, false},
	/*  7 */ {MMC_RSP_R1,      32,  64, true, false, false},
	/*  8 */ {MMC_RSP_R1,      32,  64, false, true, false},
	/*  9 */ {MMC_RSP_R2_CSD, 128,  64, false, false, false},
	/* 10 */ {MMC_RSP_R2_CID, 128,  64, false, false, false},
	/* 11 */ {MMC_RSP_R1,      32,  64, false, false, false},
	/* 12 */ {MMC_RSP_R1,      32,  64, false, false, false},
	/* 13 */ {MMC_RSP_R1,      32,  64, false, false, false},
	/* 14 */ {MMC_RSP_R1,      32,  64, false, false, false},
	/* 15 */ {MMC_RSP_NONE,     0,   0, false, false, false},
	/* 16 */ {MMC_RSP_R1,      32,  64, false, false, false},
	/* 17 */ {MMC_RSP_R1,      32,  64, false, true, false},
	/* 18 */ {MMC_RSP_R1,      32,  64, false, true, true},
	/* 19 */ {MMC_RSP_R1,      32,  64, false, false, false},
	/* 20 */ {MMC_RSP_R1,      32,  64, false, false, false},
	/* 21 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
	/* 22 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
	/* 23 */ {MMC_RSP_R1,      32,  64, false, false, false},
	/* 24 */ {MMC_RSP_R1,      32,  64, false, true, false},
	/* 25 */ {MMC_RSP_R1,      32,  64, false, true, true},
	/* 26 */ {MMC_RSP_R1,      32,  64, false, false, false},
	/* 27 */ {MMC_RSP_R1,      32,  64, false, false, false},
	/* 28 */ {MMC_RSP_R1,      32,  64, true, false, false},
	/* 29 */ {MMC_RSP_R1,      32,  64, true, false, false},
	/* 30 */ {MMC_RSP_R1,      32,  64, false, false, false},
	/* 31 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
	/* 32 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
	/* 33 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
	/* 34 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
	/* 35 */ {MMC_RSP_R1,      32,  64, false, false, false},
	/* 36 */ {MMC_RSP_R1,      32,  64, false, false, false},
	/* 37 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
	/* 38 */ {MMC_RSP_R1,      32,  64, true, false, false},
	/* 39 */ {MMC_RSP_R4,      32,  64, false, false, false},
	/* 40 */ {MMC_RSP_R5,      32,  64, false, false, false},
	/* 41 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
	/* 42 */ {MMC_RSP_R1,      32,  64, false, false, false},
	/* 43 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
	/* 44 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
	/* 45 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
	/* 46 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
	/* 47 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
	/* 48 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
	/* 49 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
	/* 50 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
	/* 51 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
	/* 52 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
	/* 53 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
	/* 54 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
	/* 55 */ {MMC_RSP_R1,      32,  64, false, false, false},
	/* 56 */ {MMC_RSP_R1,      32,  64, false, false, false},
	/* 57 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
	/* 58 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
	/* 59 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
	/* 60 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
	/* 61 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
	/* 62 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
	/* 63 */ {MMC_RSP_NONE,     0,   0, false, false, false}, /* reserved */
};

static struct MMCResponse invalid_response = {
	MMC_RSP_NONE,  0,   0, false, false
};

struct MMCResponse SDMMCHelpers::MMCCommandResponse(unsigned int index)
{
	if (index > 63)
		return invalid_response;

	return responses[index];
}

/* According to eMMC 4.51 specs */
static struct MMCCommand commands[64] = {
		"GO_IDLE_STATE",		/* CMD00	arg:00000000 => GO_IDLE_STATE
											arg:F0F0F0F0 => GO_PRE_IDLE_STATE
											arg:FFFFFFFA => BOOT_INITIATION */
		"SEND_OP_COND",			/* CMD01 */
		"ALL_SEND_CID",			/* CMD02 */
		"SET_RELATIVE_ADDR",	/* CMD03 */
		"SET_DSR",				/* CMD04 */
		"SLEEP_AWAKE",			/* CMD05 */
		"SWITCH",				/* CMD06 */
		"SELECT/DESELECT_CARD",	/* CMD07 */
		"SEND_EXT_CSD",			/* CMD08 */
		"SEND_CSD",				/* CMD09 */
		"SEND_CID",				/* CMD10 */
		"_Obsolete",			/* CMD11 */
		"STOP_TRANSMISSION",	/* CMD12 */
		"SEND_STATUS",			/* CMD13 */
		"BUSTEST_R",			/* CMD14 */
		"GO_INACTIVE_STATE",	/* CMD15 */
		"SET_BLOCKLEN",			/* CMD16 */
		"READ_SINGLE_BLOCK",	/* CMD17 */
		"READ_MULTIPLE_BLOCK",	/* CMD18 */
		"BUSTEST_W",			/* CMD19 */
		"_Obsolete",			/* CMD20 */
		"SEND_TUNING_BLOCK",	/* CMD21 */
		"_Reserved",			/* CMD22 */
		"SET_BLOCK_COUNT",		/* CMD23 */
		"WRITE_BLOCK",			/* CMD24 */
		"WRITE_MULTIPLE_BLOCK",	/* CMD25 */
		"PROGRAM_CID",			/* CMD26 */
		"PROGRAM_CSD",			/* CMD27 */
		"SET_WRITE_PROT",		/* CMD28 */
		"CLR_WRITE_PROT",		/* CMD29 */
		"SEND_WRITE_PROT",		/* CMD30 */
		"SEND_WRITE_PROT_TYPE",	/* CMD31 */
		"_Reserved",			/* CMD32 */
		"_Reserved",			/* CMD33 */
		"_Reserved",			/* CMD34 */
		"ERASE_GROUP_START",	/* CMD35 */
		"ERASE_GROUP_END",		/* CMD36 */
		"_Reserved",			/* CMD37 */
		"ERASE",				/* CMD38 */
		"FAST_IO",				/* CMD39 */
		"GO_IRQ_STATE",			/* CMD40 */
		"_Reserved",			/* CMD41 */
		"LOCK_UNLOCK",			/* CMD42 */
		"_Reserved",			/* CMD43 */
		"_Reserved",			/* CMD44 */
		"_Reserved",			/* CMD45 */
		"_Reserved",			/* CMD46 */
		"_Reserved",			/* CMD47 */
		"_Reserved",			/* CMD48 */
		"SET_TIME",				/* CMD49 */
		"_Reserved",			/* CMD50 */
		"_Reserved",			/* CMD51 */
		"_Reserved",			/* CMD52 */
		"PROTOCOL_RD",			/* CMD53 */
		"PROTOCOL_WR",			/* CMD54 */
		"APP_CMD",				/* CMD55 */
		"GEN_CMD",				/* CMD56 */
		"_Reserved",			/* CMD57 */
		"_Reserved",			/* CMD58 */
		"_Reserved",			/* CMD59 */
		"_ReservedMFR",			/* CMD60 */
		"_ReservedMFR",			/* CMD61 */
		"_ReservedMFR",			/* CMD62 */
		"_ReservedMFR",			/* CMD63 */
};

static struct MMCCommand invalid_response_cmd = {
	"_INVALID_"
};

const char * SDMMCHelpers::MMCCommandDescription(unsigned int index, unsigned int args)
{
	if (index > 63)
		return invalid_response_cmd.desc;

	if (index == 0) {
		if (args == 0x00000000)
			return "GO_IDLE_STATE";
		else if (args == 0xF0F0F0F0)
			return "GO_PRE_IDLE_STATE";
		else if (args == 0xFFFFFFFA)
			return "BOOT_INITIATION";
		else
			return "_INVALID_CMD01_";
	}

	return commands[index].desc;
}

/*
  Polynomial = 0x89 (2^7 + 2^3 + 1)
  width      = 7 bit
*/
static unsigned char crc7_table[256] = {
	0x00, 0x09, 0x12, 0x1b, 0x24, 0x2d, 0x36, 0x3f,
	0x48, 0x41, 0x5a, 0x53, 0x6c, 0x65, 0x7e, 0x77,
	0x19, 0x10, 0x0b, 0x02, 0x3d, 0x34, 0x2f, 0x26,
	0x51, 0x58, 0x43, 0x4a, 0x75, 0x7c, 0x67, 0x6e,
	0x32, 0x3b, 0x20, 0x29, 0x16, 0x1f, 0x04, 0x0d,
	0x7a, 0x73, 0x68, 0x61, 0x5e, 0x57, 0x4c, 0x45,
	0x2b, 0x22, 0x39, 0x30, 0x0f, 0x06, 0x1d, 0x14,
	0x63, 0x6a, 0x71, 0x78, 0x47, 0x4e, 0x55, 0x5c,
	0x64, 0x6d, 0x76, 0x7f, 0x40, 0x49, 0x52, 0x5b,
	0x2c, 0x25, 0x3e, 0x37, 0x08, 0x01, 0x1a, 0x13,
	0x7d, 0x74, 0x6f, 0x66, 0x59, 0x50, 0x4b, 0x42,
	0x35, 0x3c, 0x27, 0x2e, 0x11, 0x18, 0x03, 0x0a,
	0x56, 0x5f, 0x44, 0x4d, 0x72, 0x7b, 0x60, 0x69,
	0x1e, 0x17, 0x0c, 0x05, 0x3a, 0x33, 0x28, 0x21,
	0x4f, 0x46, 0x5d, 0x54, 0x6b, 0x62, 0x79, 0x70,
	0x07, 0x0e, 0x15, 0x1c, 0x23, 0x2a, 0x31, 0x38,
	0x41, 0x48, 0x53, 0x5a, 0x65, 0x6c, 0x77, 0x7e,
	0x09, 0x00, 0x1b, 0x12, 0x2d, 0x24, 0x3f, 0x36,
	0x58, 0x51, 0x4a, 0x43, 0x7c, 0x75, 0x6e, 0x67,
	0x10, 0x19, 0x02, 0x0b, 0x34, 0x3d, 0x26, 0x2f,
	0x73, 0x7a, 0x61, 0x68, 0x57, 0x5e, 0x45, 0x4c,
	0x3b, 0x32, 0x29, 0x20, 0x1f, 0x16, 0x0d, 0x04,
	0x6a, 0x63, 0x78, 0x71, 0x4e, 0x47, 0x5c, 0x55,
	0x22, 0x2b, 0x30, 0x39, 0x06, 0x0f, 0x14, 0x1d,
	0x25, 0x2c, 0x37, 0x3e, 0x01, 0x08, 0x13, 0x1a,
	0x6d, 0x64, 0x7f, 0x76, 0x49, 0x40, 0x5b, 0x52,
	0x3c, 0x35, 0x2e, 0x27, 0x18, 0x11, 0x0a, 0x03,
	0x74, 0x7d, 0x66, 0x6f, 0x50, 0x59, 0x42, 0x4b,
	0x17, 0x1e, 0x05, 0x0c, 0x33, 0x3a, 0x21, 0x28,
	0x5f, 0x56, 0x4d, 0x44, 0x7b, 0x72, 0x69, 0x60,
	0x0e, 0x07, 0x1c, 0x15, 0x2a, 0x23, 0x38, 0x31,
	0x46, 0x4f, 0x54, 0x5d, 0x62, 0x6b, 0x70, 0x79
};

U8 SDMMCHelpers::crc7(const U8 *data, unsigned int size)
{
	U8 crc7_accum = 0;
	int i;

	for (i=0;  i < size; i++) {
		crc7_accum = crc7_table[(crc7_accum << 1) ^ data[i]];
	}
	return crc7_accum;
}

std::string  SDMMCHelpers::MMCResponseDescription(unsigned int mFlags, unsigned int mData1, unsigned int mData2, DisplayBase display_base)
{
    char desc[256];
    char str_32[33];
    switch (mFlags) {
        case MMC_RSP_R1:
        {
            const char *str_state = "reserved";
            std::string str_flags("");
            switch ((mData1 >> 9) & 0xf) {
                case 0: str_state = "Idle"; break;
                case 1: str_state = "Ready"; break;
                case 2: str_state = "Ident"; break;
                case 3: str_state = "Stby"; break;
                case 4: str_state = "Tran"; break;
                case 5: str_state = "Data"; break;
                case 6: str_state = "Rcv"; break;
                case 7: str_state = "Prg"; break;
                case 8: str_state = "Dis"; break;
                case 9: str_state = "Btst"; break;
                case 10: str_state = "Slp "; break;
            }
            AnalyzerHelpers::GetNumberString(mData1, display_base, 32, str_32, sizeof(str_32));
            if (mData1 & (1 << 31))
                str_flags += " ADDRESS_OUT_OF_RANGE";
            if (mData1 & (1 << 30))
                str_flags += " ADDRESS_MISALIGN";
            if (mData1 & (1 << 29))
                str_flags += " BLOCK_LEN_ERROR";
            if (mData1 & (1 << 28))
                str_flags += " ERASE_SEQ_ERROR";
            if (mData1 & (1 << 27))
                str_flags += " ERASE_PARAM";
            if (mData1 & (1 << 26))
                str_flags += " WP_VIOLATION";
            if (mData1 & (1 << 25))
                str_flags += " CARD_IS_LOCKED";
            if (mData1 & (1 << 24))
                str_flags += " LOCK_UNLOCK_FAILED";
            if (mData1 & (1 << 23))
                str_flags += " COM_CRC_ERROR";
            if (mData1 & (1 << 22))
                str_flags += " ILLEGAL_COMMAND";
            if (mData1 & (1 << 21))
                str_flags += " CARD_ECC_FAILED";
            if (mData1 & (1 << 20))
                str_flags += " CC_ERROR";
            if (mData1 & (1 << 19))
                str_flags += " ERROR";
            if (mData1 & (1 << 18))
                str_flags += " UNDERRUN";
            if (mData1 & (1 << 17))
                str_flags += " OVERRUN";
            if (mData1 & (1 << 16))
                str_flags += " CID/CSD_OVERWRITE";
            if (mData1 & (1 << 15))
                str_flags += " WP_ERASE_SKIP";
            if (mData1 & (1 << 13))
                str_flags += " ERASE_RESET";
            if (mData1 & (1 << 8))
                str_flags += " READY_FOR_DATA";
            if (mData1 & (1 << 7))
                str_flags += " SWITCH_ERROR";
            if (mData1 & (1 << 5))
                str_flags += " APP_CMD";
            sprintf(desc, "R1, %s, rsp=%s", str_state, str_32);
            return std::string(desc);
            break;
        }
    //    case MMC_RSP_R2_CID:
    //    {
    //        std::string res("R2");
    //        char pname[7], prv_str[4], psn_str[12];
    //        char rsp_str[64];
    //        res += " [CID]";
    //        res += " rsp=";
    //        AnalyzerHelpers::GetNumberString(mData1 >> 32, display_base, 32, rsp_str, sizeof(rsp_str));
    //        res += rsp_str;
    //        res += " ";
    //        AnalyzerHelpers::GetNumberString(mData1 & 0xffffffffull, display_base, 32, rsp_str, sizeof(rsp_str));
    //        res += rsp_str;
    //        res += " ";
    //        AnalyzerHelpers::GetNumberString(mData2 >> 32, display_base, 32, rsp_str, sizeof(rsp_str));
    //        res += rsp_str;
    //        res += " ";
    //        AnalyzerHelpers::GetNumberString(mData2 & 0xffffffffull, display_base, 32, rsp_str, sizeof(rsp_str));
    //        res += rsp_str;
    //        pname[0] = (mData1 >> 32) & 0xff;
    //        pname[1] = (mData1 >> 24) & 0xff;
    //        pname[2] = (mData1 >> 16) & 0xff;
    //        pname[3] = (mData1 >>  8) & 0xff;
    //        pname[4] = (mData1 >>  0) & 0xff;
    //        pname[5] = (mData2 >> 56) & 0xff;
    //        pname[6] = 0;
    //        unsigned prv = (unsigned)((mData2 >> 48) & 0xff);
    //        prv_str[0] = '0' + ((prv >> 4) & 0x0f);
    //        prv_str[1] = '.';
    //        prv_str[2] = '0' + (prv & 0x0f);
    //        prv_str[3] = 0;
    //        unsigned psn = (unsigned)((mData2 >> 16) & 0xfffffffful);
    //        AnalyzerHelpers::GetNumberString(psn, Decimal, 32, psn_str, sizeof(psn_str));
    //        res += " pnm='";
    //        res += pname;
    //        res += "' prv=";
    //        res += prv_str;
    //        res += " psn=";
    //        res += psn_str;
    //        file_stream << res.c_str() << std::endl;
    //        break;
    //    }
    //    case MMC_RSP_R2_CSD:
    //    {
    //        std::string res("R2");
    //        char rsp_str[64];
    //        res += " [CSD]";
    //        res += " rsp=";
    //        AnalyzerHelpers::GetNumberString(mData1 >> 32, display_base, 32, rsp_str, sizeof(rsp_str));
    //        res += rsp_str;
    //        res += " ";
    //        AnalyzerHelpers::GetNumberString(mData1 & 0xffffffffull, display_base, 32, rsp_str, sizeof(rsp_str));
    //        res += rsp_str;
    //        res += " ";
    //        AnalyzerHelpers::GetNumberString(mData2 >> 32, display_base, 32, rsp_str, sizeof(rsp_str));
    //        res += rsp_str;
    //        res += " ";
    //        AnalyzerHelpers::GetNumberString(mData2 & 0xffffffffull, display_base, 32, rsp_str, sizeof(rsp_str));
    //        res += rsp_str;
    //        file_stream << res.c_str() << std::endl;
    //        break;
    //    }
        case MMC_RSP_R3:
            AnalyzerHelpers::GetNumberString(mData1, Hexadecimal, 32, str_32, sizeof(str_32));
            sprintf(desc, "R3, ocr=%s", str_32);
            return std::string(desc);
            break;
        case MMC_RSP_R4:
            sprintf(desc, "R4");
            return std::string(desc);
            break;
        case MMC_RSP_R5:
            sprintf(desc, "R5");
            return std::string(desc);
            break; 
        default:
            return std::string("ron::Invalid");
    }
}
