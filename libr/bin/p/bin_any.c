/* radare - LGPL - Copyright 2009-2017 - pancake, nibble */

#include <r_types.h>
#include <r_util.h>
#include <r_lib.h>
#include <r_bin.h>
#include <r_magic.h>

static char *get_filetype(RBinFile *bf) {
	ut8 buf[4096] = {
		0
	};
	char *res = NULL;
	RMagic *ck;
	if (!bf) {
		return NULL;
	}
	ck = r_magic_new (0);
	if (ck && bf && bf->buf) {
		const char *tmp = NULL;
		// TODO: dir.magic not honored here
		char *pfx = r_str_newf (R_JOIN_2_PATHS ("%s", R2_SDB_MAGIC), r_sys_prefix (NULL));
		r_magic_load (ck, R2_SDB_MAGIC);
		r_buf_read_at (bf->buf, 0, buf, sizeof (buf));
		tmp = r_magic_buffer (ck, buf, sizeof (buf));
		if (tmp) {
			res = strdup (tmp);
		}
		free (pfx);
	}
	r_magic_free (ck);
	return res;
}

static RBinInfo *info(RBinFile *bf) {
	RBinInfo *ret = R_NEW0 (RBinInfo);
	if (!ret) {
		return NULL;
	}
	ret->lang = "";
	ret->file = bf->file? strdup (bf->file): NULL;
	ret->type = get_filetype (bf);
	ret->has_pi = 0;
	ret->has_canary = 0;
	ret->has_retguard = -1;
	if (R_SYS_BITS & R_SYS_BITS_64) {
		ret->bits = 64;
	} else {
		ret->bits = 32;
	}
	ret->big_endian = 0;
	ret->has_va = 0;
	ret->has_nx = 0;
	ret->dbg_info = 0;
	ret->dbg_info = 0;
	ret->dbg_info = 0;
	return ret;
}

static bool load(RBinFile *bf) {
	return true;
}

static int destroy(RBinFile *bf) {
	return true;
}

static ut64 baddr(RBinFile *bf) {
	return 0LL;
}

RBinPlugin r_bin_plugin_any = {
	.name = "any",
	.desc = "Dummy format r_bin plugin",
	.license = "LGPL3",
	.load = &load,
	.destroy = &destroy,
	.baddr = &baddr,
	.info = info,
	.minstrlen = 0,
};

#ifndef CORELIB
RLibStruct radare_plugin = {
	.type = R_LIB_TYPE_BIN,
	.data = &r_bin_plugin_any,
	.version = R2_VERSION
};
#endif
