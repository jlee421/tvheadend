/*
 *  TVheadend
 *  Copyright (C) 2013 Adam Sutton
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "input.h"
#include "notify.h"

tvh_input_list_t    tvh_inputs;
tvh_hardware_list_t tvh_hardware;

/*
 * Create entry
 */
void *
tvh_hardware_create0
  ( void *o, const idclass_t *idc, const char *uuid, htsmsg_t *conf )
{
  tvh_hardware_t *th = o;

  /* Create node */
  if (idnode_insert(&th->th_id, uuid, idc)) {
    free(o);
    return NULL;
  }
  
  /* Update list */
  LIST_INSERT_HEAD(&tvh_hardware, th, th_link);
  
  /* Load config */
  if (conf)
    idnode_load(&th->th_id, conf);

  /* Update */
  notify_reload("hardware");
  
  return o;
}

/*
 * Delete hardware entry
 */

void
tvh_hardware_delete ( tvh_hardware_t *th )
{
  // TODO
  LIST_REMOVE(th, th_link);
  idnode_unlink(&th->th_id);
  notify_reload("hardware");
}

/*
 * Input status handling
 */

htsmsg_t *
tvh_input_stream_create_msg
  ( tvh_input_stream_t *st )
{
  htsmsg_t *m = htsmsg_create_map();
  htsmsg_add_str(m, "uuid", st->uuid);
  if (st->input_name)
    htsmsg_add_str(m, "input",  st->input_name);
  if (st->stream_name)
    htsmsg_add_str(m, "stream", st->stream_name);
  htsmsg_add_u32(m, "subs", st->subs_count);
  htsmsg_add_u32(m, "weight", st->max_weight);
  htsmsg_add_u32(m, "signal", st->stats.signal);
  htsmsg_add_u32(m, "ber", st->stats.ber);
  htsmsg_add_u32(m, "snr", st->stats.snr);
  htsmsg_add_u32(m, "unc", st->stats.unc);
  htsmsg_add_u32(m, "bps", st->stats.bps);
  return m;
}

void
tvh_input_stream_destroy
  ( tvh_input_stream_t *st )
{
  free(st->uuid);
  free(st->input_name);
  free(st->stream_name);
}
