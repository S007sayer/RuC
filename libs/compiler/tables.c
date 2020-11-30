/*
 *	Copyright 2020 Andrey Terekhov, Maxim Menshikov
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 */

#include "tables.h"
#include "defs.h"
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int toreprtab(analyzer *context, char str[])
{
	int i;
	int oldrepr = REPRTAB_LEN;

	context->hash = 0;
	compiler_table_expand(&context->sx->reprtab, 2);

	REPRTAB_LEN += 2;
	for (i = 0; str[i] != 0; i++)
	{
		compiler_table_expand(&context->sx->reprtab, 1);
		context->hash += str[i];
		REPRTAB[REPRTAB_LEN++] = str[i];
	}
	context->hash &= 255;
	compiler_table_expand(&context->sx->reprtab, 1);

	REPRTAB[REPRTAB_LEN++] = 0;
	compiler_table_ensure_allocated(&context->sx->reprtab, oldrepr + 1);

	REPRTAB[oldrepr] = context->hashtab[context->hash];
	REPRTAB[oldrepr + 1] = 1;
	return context->hashtab[context->hash] = oldrepr;
}

/** Инициализация modetab */
void init_modetab(analyzer *context)
{
	// занесение в modetab описателя struct {int numTh; int inf; }
	context->sx->modetab[1] = 0;
	context->sx->modetab[2] = MSTRUCT;
	context->sx->modetab[3] = 2;
	context->sx->modetab[4] = 4;
	context->sx->modetab[5] = context->sx->modetab[7] = LINT;
	context->sx->modetab[6] = toreprtab(context, "numTh");
	context->sx->modetab[8] = toreprtab(context, "data");

	// занесение в modetab описателя функции void t_msg_send(struct msg_info m)
	context->sx->modetab[9] = 1;
	context->sx->modetab[10] = MFUNCTION;
	context->sx->modetab[11] = LVOID;
	context->sx->modetab[12] = 1;
	context->sx->modetab[13] = 2;

	// занесение в modetab описателя функции void* interpreter(void* n)
	context->sx->modetab[14] = 9;
	context->sx->modetab[15] = MFUNCTION;
	context->sx->modetab[16] = LVOIDASTER;
	context->sx->modetab[17] = 1;
	context->sx->modetab[18] = LVOIDASTER;
	context->sx->modetab[19] = context->startmode = 14;
	context->sx->md = 19;
	context->keywordsnum = 0;
	context->line = 1;
	context->charnum = 1;
	context->kw = 1;
	context->sx->tc = 0;
}
