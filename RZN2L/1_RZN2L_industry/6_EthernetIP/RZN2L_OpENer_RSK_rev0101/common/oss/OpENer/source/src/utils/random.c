/*******************************************************************************
 * Copyright (c) 2017, Rockwell Automation, Inc.
 * All rights reserved.
 *
 ******************************************************************************/

#include "random.h"

#include <stdlib.h>

#if 1
#include "opener_api.h"
#endif

Random *RandomNew(SetSeed set_seed,
                  GetNextUInt32 get_next_uint32) {
#if 0
  Random *random = malloc( sizeof(Random) );
#endif
#if 1
  Random *random = CipCalloc( 1, sizeof(Random) );
#endif
  *random =
    (Random ) { .set_seed = set_seed, .get_next_uint32 = get_next_uint32 };
  return random;
}

void RandomDelete(Random **random) {
#if 0
  free(*random);
#endif
#if 1
  CipFree(*random);
#endif
  *random = NULL;
}
