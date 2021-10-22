/*
 * Copyright(C) 2011-2016 Pedro H. Penna <pedrohenriquepenna@gmail.com>
 * 
 * This file is part of Nanvix.
 * 
 * Nanvix is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Nanvix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Nanvix. If not, see <http://www.gnu.org/licenses/>.
 */

//#include <nanvix/const.h>
//#include <sys/types.h>
//#include <nanvix/region.h>

#include <nanvix/klib.h>
#include <nanvix/mm.h>

PRIVATE void prepareValue(char* s, int value, int padding)
{
	int i,len,size;

	size = itoa(s, value, 'd');
	len = padding - size;
	size = itoa(s + len, value, 'd');

	for(i=0; i<len; i++) 
        {
		char* s1 = s + i;
		//char* s2 = s1 + len;
		//char c = *(s1);
		*(s1) = ' ';
		//*(s2) = c;
	}
	
	char* s1 = s + len + size; 

	*(s1) = '\0';
}

PUBLIC int sys_free(void)
{
	//kprintf("\t\ttotal\tused\tfree\tshared\tbuff/cache\tavailable\n");
	//kprintf("Entrei no sys_free\n");

	struct process *p;

	char total    [26];
	char used     [26];
	char free     [26];
	char shared   [26];
	char buff     [26];
	char available[26];
	total    [0] = '\0';
	used     [0] = '\0';
	free     [0] = '\0';
	shared   [0] = '\0';
	buff     [0] = '\0';
	available[0] = '\0';

	for (p = IDLE; p <= LAST_PROC; p++)
	{
		/* Skip invalid processes. */
		if (!IS_VALID(p))
			continue;


	}

        size_t _total     = 100;
        size_t _used      =  get_used_memory();
        size_t _free      =  60;
        size_t _shared    =   0;
        size_t _buff      =  10;
        size_t _available =  70;

	/* Total */ 
	prepareValue(total, _total, 10);

	///* Used */
	prepareValue(used, _used, 10);

	///* Free */
	prepareValue(free, _free, 10);

	///* Shared */
	prepareValue(shared, _shared, 10);

	///* Buff */
	prepareValue(buff, _buff, 10);

	///* Available */
	prepareValue(available, _available, 10);

	//kprintf("%s%s%s%s%s%s%s%s",name, pid, 
	//	uid, priority, nice, utime, ktime, states[(int)p->state] );

	kprintf("\ntotal: %s\nused:  %s\nfree:  %s\n", total, used, free);
//, shared, buff, available

	return 0;
}

